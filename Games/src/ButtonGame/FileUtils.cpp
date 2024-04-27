module ButtonGame.FileUtils;

// TODO see what to do in what regards to this macro
constexpr int CHAR_BIT = 8;

enum class FileEncoding {
	UTF8 = 0,
	UTF8_BOM,
	UTF16_LE,
	UTF16_BE,
};

// Returns file encoding and moves the stream to the start of data
static FileEncoding findFileEncoding(std::ifstream& file)
{
	FileEncoding encoding = FileEncoding::UTF8;

	auto data_begin = file.tellg();

	byte a = file.get();
	byte b = file.get();
	byte c = file.get();

	if (a == 0xEF && b == 0xBB && c == 0xBF)
	{
		encoding = FileEncoding::UTF8_BOM;
		data_begin += 3;
	}
	else if (a == 0xFE && b == 0xFF)
	{
		encoding = FileEncoding::UTF16_BE;
		data_begin += 2;
	}
	else if (a == 0xFF && b == 0xFE)
	{
		encoding = FileEncoding::UTF16_LE;
		data_begin += 2;
	}

	file.seekg(data_begin);
	return encoding;
}

static uint32_t constexpr  replacement_character = 0xFFFD;

// Returns true if byte is a valid leadbyte
// Returns a pair of the expected number of bytes in this encoding and the information bits within
// BUG Not checking UTF8 validity of lead byte see getNextCodepointFromUTF8Stream
static std::pair<short, uint32_t> parseLeadByte(byte lead)
{
	short num_bytes = 0;     // Expected number of bytes in this codepoint
	uint32_t codepoint = 0;  // Integer where we will store information bits

	// Check encoding length
	if ((lead >> 3) == 0b11110) {
		num_bytes = 4;
		codepoint = (lead & 0b111);
	}
	else if ((lead >> 4) == 0b1110)
	{
		num_bytes = 3;
		codepoint = (lead & 0b1111);
	}
	else if ((lead >> 5) == 0b110)
	{
		num_bytes = 2;
		codepoint = (lead & 0b11111);
	}
	else if (!(lead & (1 << (sizeof(byte) * CHAR_BIT - 1))))
	{
		codepoint = lead;
	}
	else
	{
		std::cerr << "Invalid UTF8 lead byte!" << std::endl;
		return std::make_pair(0, replacement_character);
	}

	return std::make_pair(num_bytes, codepoint);
}

// Attempts to read next byte or throws an excepetion if EOF is reached
static byte getNextByte(std::ifstream& file)
{
	int next_char = file.get();
	if (next_char == -1) // EOF reached
	{
		throw std::out_of_range("End of file reached, there are no more bytes to extract.");
	}
	return (byte)next_char;
}

// BUG Not checking UTF8 validity of neither lead nor continuation bytes only the lead bits are checked
// To do so we would need to implement the table 3.7 on https://www.unicode.org/versions/Unicode6.0.0/ch03.pdf
// Also check https://en.wikipedia.org/wiki/UTF-8#Invalid_sequences_and_error_handling
// MAYBE Should try catch getNextByte or let the exception freely travel upwards through the callstack
static uint32_t getNextCodepointFromUTF8Stream(std::ifstream& file)
{
	byte first_byte = getNextByte(file);
	auto [num_bytes, codepoint] = parseLeadByte(first_byte);

	// Check continuation bytes
	bool sequence_valid = true;
	for (; num_bytes > 1; num_bytes--)
	{
		byte next_byte = getNextByte(file);
			
		if ((next_byte >> 6) != 0b10) // Not a valid continuation byte
		{
			std::cout << "Invalid UTF8 continuation byte found!" << std::endl;
			sequence_valid = false;

			if (parseLeadByte(next_byte).first)
			{
				file.seekg(-1, std::istream::cur);
				return replacement_character;
			}
			else if (num_bytes == 2)
			{
				return replacement_character;
			}
		}
		int info = next_byte & 0b111111;
		codepoint = (codepoint << 6) | info;
	}

	return codepoint;
}

std::vector<uint32_t> getFileDataAsCodepoints(std::string_view path)
{
	std::vector<uint32_t> filedata;

	// Open file
	std::ifstream file;
	file.open(path.data(), std::ifstream::in | std::ifstream::binary);
	if (file.fail())
	{
		std::cout << "Could not open file at: " << path << std::endl;
	}

	// Figure out encoding
	FileEncoding encoding = findFileEncoding(file);
	if (encoding != FileEncoding::UTF8 && encoding != FileEncoding::UTF8_BOM)
	{
		std::cout << "File Encoding not supported!" << std::endl;
		return filedata;
	}

	// Grab data
	while (file.good())
	{
		try
		{
			filedata.push_back(getNextCodepointFromUTF8Stream(file));
		}
		catch (const std::out_of_range& exception)
		{
			if (!filedata.size()) 
			{
				// MAYBE Change the file reader so it doesnt throw an out of range exception when EOF is reached
				std::cout << exception.what() << std::endl;
			}
		}
	}
	file.close();
	return filedata;
}

std::vector<char> convertCodepointsToUTF8(const std::vector<uint32_t>& data)
{
	std::vector<char> text = std::vector<char>();

	for (auto codepoint : data)
	{
		if (codepoint <= 0x007F)
		{
			text.push_back((byte)codepoint);
		}
		else if (0x0080 <= codepoint && codepoint <= 0x07FF)
		{
			text.push_back((0b110 << 5) | ((codepoint >> 6) & 0b11111));
			text.push_back((0b10 << 6) | (codepoint & 0b111111));
		}
		else if (0x0800 <= codepoint && codepoint <= 0xFFFF)
		{
			text.push_back((0b1110 << 4) | ((codepoint >> 12) & 0b1111));
			text.push_back((0b10 << 6) | ((codepoint >> 6) & 0b111111));
			text.push_back((0b10 << 6) | (codepoint & 0b111111));
		}
		else if (0x10000 <= codepoint && codepoint <= 0x10FFFF)
		{
			text.push_back((0b11110 << 3) | ((codepoint >> 18) & 0b111));
			text.push_back((0b10 << 6) | ((codepoint >> 12) & 0b111111));
			text.push_back((0b10 << 6) | ((codepoint >> 6) & 0b111111));
			text.push_back((0b10 << 6) | (codepoint & 0b111111));
		}
	}

	return text;
}

std::string convertCodepointsToUTF8(uint32_t* start, uint32_t* end)
{
	std::string text;

	for (; start <= end; start++)
	{
		auto codepoint = *start;
		if (codepoint <= 0x007F)
		{
			text.push_back((byte)codepoint);
		}
		else if (0x0080 <= codepoint && codepoint <= 0x07FF)
		{
			text.push_back((0b110 << 5) | ((codepoint >> 6) & 0b11111));
			text.push_back((0b10 << 6) | (codepoint & 0b111111));
		}
		else if (0x0800 <= codepoint && codepoint <= 0xFFFF)
		{
			text.push_back((0b1110 << 4) | ((codepoint >> 12) & 0b1111));
			text.push_back((0b10 << 6) | ((codepoint >> 6) & 0b111111));
			text.push_back((0b10 << 6) | (codepoint & 0b111111));
		}
		else if (0x10000 <= codepoint && codepoint <= 0x10FFFF)
		{
			text.push_back((0b11110 << 3) | ((codepoint >> 18) & 0b111));
			text.push_back((0b10 << 6) | ((codepoint >> 12) & 0b111111));
			text.push_back((0b10 << 6) | ((codepoint >> 6) & 0b111111));
			text.push_back((0b10 << 6) | (codepoint & 0b111111));
		}
	}

	return text;
}
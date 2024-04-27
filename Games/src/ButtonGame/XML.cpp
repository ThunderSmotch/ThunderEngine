module;
#include <cassert>
module ButtonGame.XML;
import ButtonGame.FileUtils;


// XML SPEC: https://www.w3.org/TR/xml/#sec-intro

/* MAYBE
* - Parse processing instructions and optionally include them in the final tree
* - Parse escaped characters (references) (https://stackoverflow.com/questions/1091945/what-characters-do-i-need-to-escape-in-xml-documents?noredirect=1&lq=1)
* - Parse CDATA sections and entity references
*/

XMLNode::XMLNode(NodeType type) : type(type) {}

void XMLNode::AddChild(std::unique_ptr<XMLNode>&& child)
{
	if (!_child)
	{
		_child = std::move(child);
	}
	else
	{
		XMLNode* current_child = _child.get();
		while (current_child->_next_sibling)
		{
			current_child = current_child->_next_sibling.get();
		}

		child->_previous_sibling = current_child;
		current_child->_next_sibling = std::move(child);
	}
	_number_of_children++;
}

XMLNode* XMLNode::GetChild(size_t index)
{
	if (index >= _number_of_children)
	{
		return nullptr;
	}

	XMLNode* current_child = _child.get();
	for (; index; index--)
	{
		current_child = current_child->_next_sibling.get();
	}

	return current_child;
}

XMLNode* XMLNode::GetFirstChild(std::string_view child_name)
{
	XMLNode* first_child = nullptr;
	for (size_t i = 0; i < GetChildCount(); i++)
	{
		XMLNode* child = GetChild(i);
		if (child->name == child_name)
		{
			first_child = child;
			break;
		}
	}
	return first_child;
}

// Represents an XMLParsingError, which describes in which file, line and column the error occurred.
class ParsingError : public std::exception {
public:
	explicit ParsingError(std::string what = "", int line = 0, int column = 0, std::string path = "")
		: message(std::format("[XML PARSING ERROR] {}:{}:{}\n\t{}", path, line, column, what)) 
	{}

	const char* what() const noexcept final
	{
		return message.c_str();
	}

	std::string message;
};

//////////////////////// PARSING HELPER FUNCTIONS ////////////////////
std::pair<int, int> findLineAndColumn(const u32* start, const u32* current)
{
	int line   = 1;
	int column = 0;

	while (start != current)
	{
		if (*start == '\n')
		{
			line++;
			column = 0;
		}
		start++;
		column++;
	}
	return std::make_pair(line, column);
}

// Modifies the vector data to convert CRLF to LF and standalone CR to LF as well
// This erases elements from the given vector so it is costly.
void normalizeLineBreaks(std::vector<u32>& data)
{
	constexpr u32 CR = 0x0D;
	constexpr u32 LF = 0x0A;
	size_t last = data.size() - 1;

	for (size_t i = 0; i < last; i++)
	{
		if (data[i] == CR)
		{
			if (data[i + 1] == LF) // If next char is LF delete CR
			{
				data.erase(data.begin() + i);
				last--;
				i--;
			}
			else // Substitute CR with LF if the following isn't LF
			{
				data[i] = LF;
			}
		}
	}

	// Check last byte
	if(data[last] == CR)
		data[last] = LF;
}

// As defined in the spec
inline bool isWhiteSpace(u32 codepoint)
{
	return (codepoint == 0x20 || codepoint == 0x9 || codepoint == 0xD || codepoint == 0xA);
};
inline bool isNameStartChar(u32 codepoint)
{
	return (
		('a' <= codepoint && codepoint <= 'z')   ||
		('A' <= codepoint && codepoint <= 'Z')   ||
		codepoint == ':' ||
		codepoint == '_' ||
		(0xC0 <= codepoint && codepoint <= 0xD6) ||
		(0xD8 <= codepoint && codepoint <= 0xF6) ||
		(0xF8 <= codepoint && codepoint <= 0x2FF) ||
		(0x370 <= codepoint && codepoint <= 0x37D) ||
		(0x37F <= codepoint && codepoint <= 0x1FFF) ||
		(0x200C <= codepoint && codepoint <= 0x200D) ||
		(0x2070 <= codepoint && codepoint <= 0x218F) ||
		(0x2C00 <= codepoint && codepoint <= 0x2FEF) ||
		(0x3001 <= codepoint && codepoint <= 0xD7FF) ||
		(0xF900 <= codepoint && codepoint <= 0xFDCF) ||
		(0xFDF0 <= codepoint && codepoint <= 0xFFFD) ||
		(0x10000 <= codepoint && codepoint <= 0xEFFFF)
	);
}
inline bool isNameChar(u32 codepoint)
{
	return (
		isNameStartChar(codepoint) ||
		codepoint == '-' ||
		codepoint == '.' ||
		('0' <= codepoint && codepoint <= '9') ||
		codepoint == 0xB7 ||
		(0x0300 <= codepoint && codepoint <= 0x036F) ||
		(0x203F <= codepoint && codepoint <= 0x2040)
	);
}

// Utility functions to transverse the data and find specific sequences of codepoints
u32* find(u32 goal, u32* data, const u32* end)
{
	u32 codepoint = *data;
	while (codepoint != goal)
	{
		data++;
		if (data <= end)
		{
			codepoint = *data;
		}
		else
		{
			return nullptr;
		}
	}
	return data;
}
u32* find(const u32* sequence_start, const u32* sequence_end, u32* data, const u32* end)
{
	size_t sequence_length = 1 + sequence_end - sequence_start;
	u32 codepoint = *sequence_start;

	while (true)
	{
		data = find(codepoint, data, end);
		if (data == nullptr || data + sequence_length - 1 > end)
		{
			return nullptr; // Could not find beginning token or sequence does not fit
		}

		for (size_t i = 0; i < sequence_length; i++)
		{
			if ( *(sequence_start + i) != data[i])
			{
				data += i;
				break;
			}

			if (i == sequence_length - 1)
			{
				return data;
			}
		}
	}
}

template<std::size_t Extent>
u32* find(std::span<const u32, Extent> sequence, u32* data, const u32* end)
{
	assert("Sequence length is not zero" && sequence.size());
	return find(sequence.data(), &sequence.back(), data, end);
}

constexpr std::array<u32, 5> seq_XMLDECL_beg = { '<', '?', 'x', 'm', 'l'};
constexpr std::array<u32, 2> seq_PI_beg = { '<', '?', };
constexpr std::array<u32, 2> seq_PI_end = { '?', '>', };
constexpr std::array<u32, 4> seq_COMMENT_beg = { '<', '!', '-', '-'};
constexpr std::array<u32, 3> seq_COMMENT_end = { '-', '-', '>', };
//////////////////////////////////////////////////////////////////////////

[[noreturn]] void XMLParser::Throw(const char* error, const u32* at) const
{
	auto [line, column] = findLineAndColumn(_beg, at);
	throw ParsingError(error, line, column);
}

void XMLParser::ParseProcessingInstruction()
{
	u32* ending = find(std::span(seq_PI_end), _cur, _end);
	if (ending == nullptr)
	{
		Throw("Processing instruction was not closed!", _cur);
	}
	_cur = ending + seq_PI_end.size();
}

void XMLParser::ParseComment()
{
	if( find(std::span(seq_COMMENT_beg), _cur, _end) != _cur )
	{
		Throw("Expected a comment!", _cur);
	}
	
	u32* ending = find(std::span(seq_COMMENT_end), _cur, _end);
	if (ending == nullptr)
	{
		Throw("Comment was not closed!", _cur);
	}
	_cur = ending + seq_COMMENT_end.size();
}

// Skips all whitespace, _cur becomes a non whitespace character
void XMLParser::SkipWhiteSpace()
{
	u32 codepoint = *_cur;
	while (isWhiteSpace(codepoint))
	{
		if (_cur == _end)
		{
			_cur++;
			break;
		}
		else
		{
			_cur++;
			codepoint = *_cur;
		}
	}
}

void XMLParser::ParseProlog()
{
	auto p = find(std::span(seq_XMLDECL_beg), _cur, _end);
	if (p != _beg && p != nullptr)
	{
		Throw("Found an XML declaration but it was not at the beginning of file!", _cur);
	}
	else if (p == _beg)
	{
		ParseProcessingInstruction();
	}

	while (_cur <= _end)
	{
		SkipWhiteSpace();
		if (_cur >= _end)
		{
			return;
		}
		if (*_cur != '<')
		{
			Throw("Unexpected character found in prolog! Expected '<'", _cur);
		}

		if (*(_cur + 1) == '?')
		{
			ParseProcessingInstruction();
		}
		else if (*(_cur + 1) == '!')
		{
			if ((_cur + 2 <= _end) && *(_cur + 2) == 'D')
			{
				Throw("This XML Parser does not handle DOCTYPE declarations!", _cur);
			}
			ParseComment();
		}
		else if (isNameStartChar(*(_cur + 1)))
		{
			return;
		}
		else
		{
			Throw("Invalid name start character after '<'", _cur);
		}
	}
}

// Returns the beginning of the name and, after parsing the name, _cur is ahead of the name
std::string XMLParser::ParseName()
{
	u32* begining = _cur;
	if (!isNameStartChar(*_cur))
	{
		Throw("Character is not a valid starting name character.", _cur);
	}
	_cur++;
	while (isNameChar(*_cur))
	{
		_cur++;
	}
	return convertCodepointsToUTF8(begining, _cur-1);
}

void XMLParser::ParseAttribute(XMLNode* element)
{
	std::string name = ParseName();
	SkipWhiteSpace();
	if (*_cur != '=')
	{
		Throw("Expected equals sign when assigning attribute!", _cur);
	}
	_cur++;
	SkipWhiteSpace();
	
	if (_cur <= _end && *_cur != '\'' && *_cur != '"')
	{
		Throw("Expected quotes or apostrophe to start attribute value!", _cur);
	}
	u32* value_start = _cur + 1;

	// TODO Process possible references therein
	u32* end = find(*_cur, _cur + 1, _end);
	if (end == nullptr || end >= _end)
	{
		Throw("Attribute value was not closed!", _cur);
	}
	_cur = end;
	std::string value = convertCodepointsToUTF8(value_start, _cur - 1);

	element->attributes[name] = value;
	_cur = end + 1;
}

bool XMLParser::ParseStartTag(XMLNode* element)
{
	element->name = ParseName();
	
	while(true)
	{
		SkipWhiteSpace();
		if (_cur >= _end)
		{
			Throw("Element start or empty tag was not closed properly!", _cur);
		}
		if (*_cur == '>') // End of start tag
		{
			_cur++;
			break;
		}
		else if (*_cur == '/' && *(_cur + 1) == '>') // End of empty element
		{
			_cur += 2;
			return false;
		}
		else
		{
			ParseAttribute(element);
		}
	}

	return true;
}

void XMLParser::ParseEndTag(const XMLNode* element)
{
	if (_cur + 2 > _end)
	{
		Throw("Could not parse end tag, file has ended!", _cur);
	}
	_cur += 2;
	if (ParseName() != element->name)
	{
		Throw("Closing tag name does not match current open element!", _cur);
	}
	SkipWhiteSpace();
	if (_cur > _end || (*_cur) != '>')
	{
		Throw("Closing tag was not closed properly! Expected a '>'!", _cur);
	}
	_cur++;
}

std::unique_ptr<XMLNode> XMLParser::ParseElement(XMLNode* parent)
{
	u32* start_element = _cur;

	_cur++;
	if (_cur >= _end)
	{
		Throw("Could not parse element, file has ended!", _cur);
	}

	auto element = std::make_unique<XMLNode>(NodeType::ELEMENT);
	
	if (parent != nullptr)
	{
		element->parent = parent;
	}

	if (bool is_start_tag = ParseStartTag(element.get()); !is_start_tag) // Empty tag, thus has been processed
	{
		return std::move(element);
	}

	// Parse content and end tag
	while (true)
	{
		// MAYBE Not skip inner content CharData and save it as a special XML node
		u32* start_of_tag = find('<', _cur, _end);
		_cur = start_of_tag;
		
		if (start_of_tag == nullptr || start_of_tag == _end)
		{
			Throw("Element does not have a closing tag!", start_element);
		}
		else if (*(start_of_tag + 1) == '!')
		{
			ParseComment();
		}
		else if (*(start_of_tag + 1) == '?')
		{
			ParseProcessingInstruction();
		}
		else if (isNameStartChar(*(start_of_tag + 1)))
		{
			element->AddChild(ParseElement(element.get()));
		}
		else if (*(start_of_tag + 1) == '/') // Closing tag
		{
			ParseEndTag(element.get());
			return element;
		}
		else
		{
			Throw("Unexpected character after '<'", _cur);
		}
	}
}

bool XMLParser::Parse()
{
	try {
		ParseProlog();
		_root = ParseElement();
		return true;
	}
	catch (ParsingError& error)
	{
		std::cout << error.what() << std::endl;
		return false;
	}
}

XMLParser::XMLParser(std::string_view path)
	: path(path)
{
	std::vector<u32> data = getFileDataAsCodepoints(path);
	normalizeLineBreaks(data);
	_beg = data.data();
	_end = &data.back();
	_cur = _beg;

	Parse();

	// Redefine the pointers as parsing is done and they might be invalid
	_beg = nullptr;
	_end = nullptr;
	_cur = nullptr;
}

XMLParser::XMLParser(std::vector<u32>& data)
: _beg(data.data()), _end(&data.back()), _cur(_beg), path("embedded data")
{
}
export module ButtonGame.FileUtils;

import ThunderLib.Base;
import std;

export {
	std::vector<uint32_t> getFileDataAsCodepoints(std::string_view path);
	std::vector<char> convertCodepointsToUTF8(const std::vector<uint32_t>& data);

	std::string convertCodepointsToUTF8(uint32_t* start, uint32_t* end);
}
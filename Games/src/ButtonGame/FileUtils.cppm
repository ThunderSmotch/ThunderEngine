export module ButtonGame.FileUtils;

import ThunderLib.Base;
import std;

export {
	std::vector<u32> getFileDataAsCodepoints(std::string_view path);
	std::vector<char> convertCodepointsToUTF8(const std::vector<u32>& data);

	std::string convertCodepointsToUTF8(u32* start, u32* end);
}
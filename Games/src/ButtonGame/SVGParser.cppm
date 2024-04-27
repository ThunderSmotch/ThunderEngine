module;
#include <glm/glm.hpp>
export module SVG.SVGParser;

import std;
import ButtonGame.XML;
import SVG.Path;

export namespace SVG
{
	//TODO Normalize all svgs so they have an aspect ratio and ocupy the interval from 0 to 1
	// they will be scaled and translated later

	class SVGObject
	{
	public:
		Path path;
	};

	// Represents a group of SVG elements
	class SVGGroup
	{
	public:
		BBox bbox;

		std::vector<std::unique_ptr<SVGObject>> objects;
		void Scale(glm::vec2 scale);
		void CalculateBoundingBox();
	};

	std::unique_ptr<SVGGroup> parseSVG(std::string_view path);
}
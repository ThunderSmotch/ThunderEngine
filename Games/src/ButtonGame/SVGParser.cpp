module;
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
module SVG.SVGParser;

import std;

namespace SVG {

	class SVGParsingException : public std::runtime_error {
	public:
		explicit SVGParsingException(const char* what = "")
			: std::runtime_error(what) {}
	};

	class NotImplementedException : public std::exception
	{
	public:
		explicit(false) NotImplementedException(std::string_view what) : what_arg(what) { };

		const char* what() const noexcept final {
			return what_arg.c_str();
		}

		std::string what_arg;
	};

	/// Utility function

	float parseLength(std::string_view length)
	{
		size_t chars_processed;
		float numeric_length = std::stof(length.data(), &chars_processed);

		std::string_view units = length.substr(chars_processed);

		// TODO See all possible length sizes
		// https://www.w3.org/TR/CSS21/syndata.html#value-def-length
		// https://www.w3.org/TR/CSS21/visudet.html#propdef-width
		if (units == "" || units == "px")
		{
			return numeric_length;
		}
		else if (units == "ex")
		{
			return 5.0f * numeric_length;
		}
		else if (units == "%")
		{
			return numeric_length * 512.0f;
		}

		return numeric_length;
	}

	glm::mat4x4 parseTransform(std::string_view transform)
	{
		float a, b, c, d, e, f;

		// Handle matrix(...)
		size_t index = transform.find("matrix(");
		if (index == std::string::npos)
		{
			throw NotImplementedException("Other transforms other than matrix() are not yet implemented.");
		}
		else
		{
			transform = transform.substr(index + 7);

			size_t chars_processed;
			a = std::stof(transform.data(), &chars_processed);
			transform = transform.substr(chars_processed);
			b = std::stof(transform.data(), &chars_processed);
			transform = transform.substr(chars_processed);
			c = std::stof(transform.data(), &chars_processed);
			transform = transform.substr(chars_processed);
			d = std::stof(transform.data(), &chars_processed);
			transform = transform.substr(chars_processed);
			e = std::stof(transform.data(), &chars_processed);
			transform = transform.substr(chars_processed);
			f = std::stof(transform.data(), &chars_processed);
		}

		return glm::mat4x4(
			a, c, 0.0f, e,
			b, d, 0.0f, f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	void skipWhiteSpaceAndCommas(const char*& data)
	{
		char c = *data;
		while (c == 0x20 || c == 0x9 || c == 0xD || c == 0xA || c == 0xC || c == ',')
		{
			data++;
			c = *data;
		}
	}

	float readFloat(const char*& data)
	{
		skipWhiteSpaceAndCommas(data);
		size_t chars_processed;
		float a = std::stof(data, &chars_processed);
		data += chars_processed;
		skipWhiteSpaceAndCommas(data);
		return a;
	}

	bool readBool(const char*& data)
	{
		skipWhiteSpaceAndCommas(data);
		size_t chars_processed;
		auto a = (bool)std::stod(data, &chars_processed);
		data += chars_processed;
		skipWhiteSpaceAndCommas(data);
		return a;
	}

	glm::vec2 readFloat2(const char*& data)
	{
		float a = readFloat(data);
		float b = readFloat(data);
		return glm::vec2(a, b);
	}

	// Path commands
	void commandMoveTo(Path& path, glm::vec2& start_position, glm::vec2& current_position, const char*& data, bool absolute, bool& first_time)
	{
		if (path.EndSubpath() || first_time)
		{
			path.BeginSubpath();
		}

		if (first_time)
		{
			first_time = false;
		}

		current_position = absolute ? readFloat2(data) : start_position + readFloat2(data);
		start_position = current_position;

		skipWhiteSpaceAndCommas(data);

		// Any subsequent commands are implicit LineTo
		while (*data && ((*data) < 'A' || *data > 'z'))
		{
			glm::vec2 previous_position = current_position;
			glm::vec2 offset = readFloat2(data);
			current_position = absolute ? offset : current_position + offset;

			path.AddPathElement(
				std::make_unique<Line>(previous_position, current_position)
			);
		}
	}

	void commandClosePath(Path& path, glm::vec2& start_position, glm::vec2& current_position, const char*& data)
	{
		path.AddPathElement(std::make_unique<Line>(current_position, start_position));
		path.SetClosed();

		skipWhiteSpaceAndCommas(data);

		if (*data) // There's more to process
		{
			if (path.EndSubpath())
			{
				path.BeginSubpath();
			}
			else
			{
				path.SetOpen();
			}
			current_position = start_position;
		}
		else
		{
			path.EndSubpath();
		}
	}

	void commandLineTo(Path& path, glm::vec2& position, const char*& data, bool absolute)
	{
		do
		{
			glm::vec2 previous_position = position;
			glm::vec2 offset = readFloat2(data);
			position = absolute ? offset : position + offset;

			path.AddPathElement(
				std::make_unique<Line>(previous_position, position)
			);
		} while (*data && ((*data) < 'A' || *data > 'z'));
	}

	void commandHLineTo(Path& path, glm::vec2& position, const char*& data, bool absolute)
	{
		do
		{
			glm::vec2 previous_position = position;
			float offset = readFloat(data);
			position = glm::vec2(absolute ? offset : position.x + offset, position.y);

			path.AddPathElement(
				std::make_unique<Line>(previous_position, position)
			);
		} while (*data && ((*data) < 'A' || *data > 'z'));
	}

	void commandVLineTo(Path& path, glm::vec2& position, const char*& data, bool absolute)
	{
		do
		{
			glm::vec2 previous_position = position;
			float offset = readFloat(data);
			position = glm::vec2(position.x, absolute ? offset : position.y + offset);

			path.AddPathElement(
				std::make_unique<Line>(previous_position, position)
			);
		} while (*data && ((*data) < 'A' || *data > 'z'));
	}

	glm::vec2 commandCubicBezierTo(Path& path, glm::vec2& position, const char*& data, bool absolute)
	{
		// Created here because we need to return it in order to memorize the last used control point
		glm::vec2 c2;
		do
		{
			glm::vec2 previous_position = position;

			glm::vec2 offset_c1 = readFloat2(data);
			glm::vec2 offset_c2 = readFloat2(data);
			glm::vec2 offset_final = readFloat2(data);

			glm::vec2 c1 = absolute ? offset_c1 : previous_position + offset_c1;
			c2 = absolute ? offset_c2 : previous_position + offset_c2;
			position = absolute ? offset_final : previous_position + offset_final;

			path.AddPathElement(
				std::make_unique<CubicBezier>(previous_position, c1, c2, position)
			);
		} while (*data && ((*data) < 'A' || *data > 'z'));
		return c2;
	}

	glm::vec2 commandCubicBezierTo(Path& path, glm::vec2& position, glm::vec2 previous_control_point, const char*& data, bool absolute)
	{
		do
		{
			glm::vec2 previous_position = position;

			glm::vec2 c1 = 2.0f * previous_position - previous_control_point;
			glm::vec2 c2 = absolute ? readFloat2(data) : previous_position + readFloat2(data);
			position = absolute ? readFloat2(data) : previous_position + readFloat2(data);
			previous_control_point = c2;

			path.AddPathElement(
				std::make_unique<CubicBezier>(previous_position, c1, c2, position)
			);
		} while (*data && ((*data) < 'A' || *data > 'z'));
		return previous_control_point;
	}

	glm::vec2 commandQuadraticBezierTo(Path& path, glm::vec2& position, const char*& data, bool absolute)
	{
		// Created here because we need to return it in order to memorize the last used control point
		glm::vec2 c1;
		do
		{
			glm::vec2 previous_position = position;

			glm::vec2 offset_c1 = readFloat2(data);
			glm::vec2 offset_final = readFloat2(data);

			c1 = absolute ? offset_c1 : previous_position + offset_c1;
			position = absolute ? offset_final : previous_position + offset_final;

			path.AddPathElement(
				std::make_unique<QuadraticBezier>(previous_position, c1, position)
			);
		} while (*data && ((*data) < 'A' || *data > 'z'));
		return c1;
	}

	glm::vec2 commandQuadraticBezierTo(Path& path, glm::vec2& position, glm::vec2 previous_control_point, const char*& data, bool absolute)
	{
		do
		{
			glm::vec2 previous_position = position;

			glm::vec2 c1 = 2.0f * previous_position - previous_control_point;
			position = absolute ? readFloat2(data) : previous_position + readFloat2(data);
			previous_control_point = c1;

			path.AddPathElement(
				std::make_unique<QuadraticBezier>(previous_position, c1, position)
			);
		} while (*data && ((*data) < 'A' || *data > 'z'));
		return previous_control_point;
	}

	void commandArcTo(Path& path, glm::vec2& position, const char*& data, bool absolute)
	{
		// Read inputs
		glm::vec2 radius = readFloat2(data);
		float angle = readFloat(data);
		bool large_arc = readBool(data);
		bool sweep = readBool(data);
		glm::vec2 initial_point = position;
		position = absolute ? readFloat2(data) : position + readFloat2(data);

		// If the initial and final points are identical just return
		if (abs(initial_point.x - position.x) < 0.001f)
		{
			position = initial_point;
			return;
		}

		// If one of the radii is small enough just draw a line
		if (abs(radius.x) < 0.001f || abs(radius.y) < 0.001f)
		{
			path.AddPathElement(
				std::make_unique<Line>(initial_point, position)
			);
			return;
		}

		radius.x = abs(radius.x);
		radius.y = abs(radius.y);

		float angle_rad = fmodf(angle, 360.0f) * std::numbers::pi_v<float> / 180.0f;
		float cos_angle = cos(angle_rad);
		float sin_angle = sin(angle_rad);
		glm::mat2 rotation_matrix(cos_angle, sin_angle, -sin_angle, cos_angle);

		// We shall now calculate center coords as well as delta angle and initial angle
		// https://www.w3.org/TR/SVG2/implnote.html#ArcConversionEndpointToCenter

		// Step 1 - compute midpoint and rotate
		glm::vec2 midpoint = (initial_point - position) / 2.0f;
		glm::vec2 midpoint_rotated = rotation_matrix * midpoint;

		// Step 2 - computer rotated center coords
		float rx2 = radius.x * radius.x;
		float ry2 = radius.y * radius.y;
		float x12 = midpoint_rotated.x * midpoint_rotated.x;
		float y12 = midpoint_rotated.y * midpoint_rotated.y;

		// Check that radii are large enough.
		// If they are not, the spec says to scale them up so they are.
		// This is to compensate for potential rounding errors/differences between SVG implementations.
		float radiiCheck = x12 / rx2 + y12 / ry2;
		if (radiiCheck > 0.99999)
		{
			auto radiiScale = (float)(glm::sqrt(radiiCheck) * 1.00001);
			radius.x = (radiiScale * radius.x);
			radius.y = (radiiScale * radius.y);
			rx2 = radius.x * radius.x;
			ry2 = radius.y * radius.y;
		}

		float prefactor = sqrtf((rx2 * ry2 - rx2 * y12 - ry2 * x12) / (rx2 * y12 + ry2 * x12));
		float sign = (large_arc == sweep) ? -1.0f : 1.0f;

		float cx1 = sign * prefactor * radius.x * midpoint_rotated.y / radius.y;
		float cy1 = -sign * prefactor * radius.y * midpoint_rotated.x / radius.x;

		// Step 3 - compute center coords
		float cx = cos_angle * cx1 - sin_angle * cy1 + (initial_point.x + position.x) / 2.0f;
		float cy = sin_angle * cx1 + cos_angle * cy1 + (initial_point.y + position.y) / 2.0f;

		// Step 4 - compute initial angle and arc size
		float ux = (midpoint_rotated.x - cx1) / radius.x;
		float uy = (midpoint_rotated.y - cy1) / radius.y;
		float vx = (-midpoint_rotated.x - cx1) / radius.x;
		float vy = (-midpoint_rotated.y - cy1) / radius.y;

		float nu = (sqrt(ux * ux + uy * uy));
		float nv = (sqrt(vx * vx + vy * vy));
		float dp = (ux * vx + uy * vy);
		float n = nu * nv;

		float dtheta_sign = ((ux * vy - uy * vx) > 0 ? 1.0f : -1.0f);

		float theta1 = (uy > 0 ? 1.0f : -1.0f) * acosf(ux / nu);
		float dtheta = dtheta_sign * ((dp / n) < -1.0f ? std::numbers::pi_v<float> : (dp / n > 1.0f) ? 0.0f : acosf(dp / n));

		if (abs(dtheta) < 0.001f)
		{
			path.AddPathElement(
				std::make_unique<Line>(initial_point, position)
			);
			return;
		}

		if (!sweep && dtheta > 0)
		{
			dtheta -= 2.0f * std::numbers::pi_v<float>;
		}
		else if (sweep && dtheta < 0)
		{
			dtheta += 2.0f * std::numbers::pi_v<float>;
		}

		theta1 = fmodf(theta1, 2.0f * std::numbers::pi_v<float>);
		dtheta = fmodf(dtheta, 2.0f * std::numbers::pi_v<float>);

		// Step 5 - Convert elliptical arcs to cubic beziers
		auto num_curves = (int)glm::ceil(glm::abs(dtheta) * 2.0 / std::numbers::pi_v<float>);
		float angle_increment = dtheta / (float)num_curves;

		// Calculate a transformation matrix that will move and scale these bezier points to the correct location.
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(radius.x, radius.y, 0.0f));
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(cx, cy, 0.0f));
		glm::mat4 transformation = translation * rotation * scale;

		// The length of each control point vector is given by the following formula.
		// https://github.com/ambrosiogabe/MathAnimation/blob/master/Animations/src/svg/Svg.cpp#L283
		// Thanks ambrosiogabe!
		auto control_length = (float)(4.0 / 3.0 * glm::sin(angle_increment / 2.0) / (1.0 + glm::cos(angle_increment / 2.0)));

		for (short i = 0; i < num_curves; i++)
		{
			float curve_angle = theta1 + i * angle_increment;

			float dx = cos(curve_angle);
			float dy = sin(curve_angle);

			auto c1 = transformation * glm::vec4(dx - control_length * dy, dy + control_length * dx, 0.0f, 1.0f);

			curve_angle += angle_increment;
			dx = cos(curve_angle);
			dy = sin(curve_angle);
			auto c2 = transformation * glm::vec4(dx + control_length * dy, dy - control_length * dx, 0.0f, 1.0f);

			auto endpoint = transformation * glm::vec4(dx, dy, 0.0f, 1.0f);

			if (i == num_curves - 1) // In the last curve just use the expected final point
			{
				endpoint = { position.x, position.y, 0.0f, 0.0f };
			}

			path.AddPathElement(
				std::make_unique<CubicBezier>(initial_point, glm::vec2(c1.x, c1.y), glm::vec2(c2.x, c2.y), glm::vec2(endpoint.x, endpoint.y))
			);

			initial_point = glm::vec2(endpoint.x, endpoint.y);
		}

	}

	// TODO: make it so we do not have dangling subpaths after closing a path
	void parsePathData(Path& path, const char* data)
	{
		glm::vec2 start_position(0.0f);    // Needed if closing the curve
		glm::vec2 current_position(0.0f);
		bool first_time = true;

		// The below three variables are needed to process the smooth commands
		glm::vec2 previous_control_point(0.0f);
		bool previous_was_cubic = false;
		bool previous_was_quadratic = false;

		while (*data)
		{
			skipWhiteSpaceAndCommas(data);

			char command = *data;
			data++;
			switch (command)
			{
			case 'M':
				commandMoveTo(path, start_position, current_position, data, true, first_time);
				break;
			case 'm':
				commandMoveTo(path, start_position, current_position, data, false, first_time);
				break;
			case 'L':
				commandLineTo(path, current_position, data, true);
				break;
			case 'l':
				commandLineTo(path, current_position, data, false);
				break;
			case 'H':
				commandHLineTo(path, current_position, data, true);
				break;
			case 'h':
				commandHLineTo(path, current_position, data, false);
				break;
			case 'V':
				commandVLineTo(path, current_position, data, true);
				break;
			case 'v':
				commandVLineTo(path, current_position, data, false);
				break;
			case 'C':
				previous_control_point = commandCubicBezierTo(path, current_position, data, true);
				break;
			case 'c':
				previous_control_point = commandCubicBezierTo(path, current_position, data, false);
				break;
			case 'S':
				previous_control_point = commandCubicBezierTo(path, current_position,
					previous_was_cubic ? previous_control_point : current_position, data, true);
				break;
			case 's':
				previous_control_point = commandCubicBezierTo(path, current_position,
					previous_was_cubic ? previous_control_point : current_position, data, false);
				break;
			case 'Q':
				previous_control_point = commandQuadraticBezierTo(path, current_position, data, true);
				break;
			case 'q':
				previous_control_point = commandQuadraticBezierTo(path, current_position, data, false);
				break;
			case 'T':
				previous_control_point = commandQuadraticBezierTo(path, current_position,
					previous_was_quadratic ? previous_control_point : current_position, data, true);
				break;
			case 't':
				previous_control_point = commandQuadraticBezierTo(path, current_position,
					previous_was_quadratic ? previous_control_point : current_position, data, false);
				break;
			case 'A':
				commandArcTo(path, current_position, data, true);
				break;
			case 'a':
				commandArcTo(path, current_position, data, false);
				break;
			case 'Z':
			case 'z':
				commandClosePath(path, start_position, current_position, data);
				break;
			default:
				throw SVGParsingException("Unexpected token appeared in path data!");
				break;
			}

			// Handle memory of previous command
			if (command == 'C' || command == 'c' || command == 'S' || command == 's')
			{
				previous_was_cubic = true;
			}
			else
			{
				previous_was_cubic = false;
			}

			if (command == 'Q' || command == 'q' || command == 'T' || command == 't')
			{
				previous_was_quadratic = true;
			}
			else
			{
				previous_was_quadratic = false;
			}
		}

		std::cout << data << std::endl;
	}

	void parseTagSVG()
	{
		// TODO Parse viewbox here
		/*
		float height = parseLength(_xml_parser.GetRootElement()->attributes["height"]);
		float width = parseLength(_xml_parser.GetRootElement()->attributes["width"]);
		_svg.SetDimensions(height, width);
		*/
	}

	void parseTagPath(SVGObject& svg, XMLNode* path_element)
	{
		//TODO apply transform to path
		std::string transform = path_element->attributes["transform"];
		if (!transform.empty()) {
			glm::mat4 transform_matrix = parseTransform(transform);
		}

		std::string data = path_element->attributes["d"];
		parsePathData(svg.path, data.c_str());
		svg.path.CalculateBoundingBox();
	}

	void copy(SVGObject& from, SVGObject& to, glm::vec2 offset = glm::vec2(0.0f))
	{
		// TODO instead of recreating the entire subpath, just copy values
		// this avoids us recalculating length and bboxes
		size_t subpaths = from.path.GetSubpathCount();
		for (size_t i = 0; i < subpaths; i++)
		{
			// Build subpath in child
			to.path.BeginSubpath();
			SubPath& subpath = to.path.GetSubpath(i);
			SubPath& from_subpath = from.path.GetSubpath(i);

			size_t num_path_elements = from_subpath.GetPathElementsCount();
			for (size_t j = 0; j < num_path_elements; j++)
			{
				PathElement* element = from_subpath.GetPathElement(j);
				switch (element->type)
				{
				using enum PathElementType;
				case LINE:
				{
					const auto line = (Line*)element;
					subpath.AddPathElement(std::make_unique<Line>(
						line->_c0 + offset,
						line->_c1 + offset
						));
					break;
				}
				case QUADRATIC:
				{
					const auto quadratic = (QuadraticBezier*)element;
					subpath.AddPathElement(std::make_unique<QuadraticBezier>(
						quadratic->_c0 + offset,
						quadratic->_c1 + offset,
						quadratic->_c2 + offset
						));
					break;
				}
				case CUBIC:
				{
					const auto cubic = (CubicBezier*)element;
					subpath.AddPathElement(std::make_unique<CubicBezier>(
						cubic->_c0 + offset,
						cubic->_c1 + offset,
						cubic->_c2 + offset,
						cubic->_c3 + offset
						));
					break;
				}
				default:
					break;
				}
			}
			to.path.EndSubpath();
		}
		to.path.CalculateBoundingBox();
	}

	void parseUseTag(SVGGroup& svg_group, XMLNode* child, std::unordered_map<std::string, SVGObject>& defs)
	{
		// parse x y and xlink:href
		float x = std::stof(child->attributes["x"]);
		float y = std::stof(child->attributes["y"]);

		glm::vec2 offset(x, y);

		std::string link = child->attributes["xlink:href"];

		if (link[0] != '#')
		{
			throw SVGParsingException("Use tag has an improper id attribute.");
		}

		if (!defs.contains(link.substr(1)))
		{
			throw SVGParsingException("Defs element did not contain an id that was later referenced");
		}

		svg_group.objects.emplace_back(std::make_unique<SVGObject>());

		copy(defs[link.substr(1)], *svg_group.objects.back(), offset);
	}

	void parseSVGContent(SVGGroup& svg_group, XMLNode* element, std::unordered_map<std::string, SVGObject>& defs)
	{
		for (size_t i = 0; i < element->GetChildCount(); i++)
		{
			XMLNode* child = element->GetChild(i);

			if (child->name == "path")
			{
				svg_group.objects.emplace_back(std::make_unique<SVGObject>());
				parseTagPath(*(svg_group.objects.back()), child);
			}
			else if (child->name == "g")
			{
				// In doing so, we are ignoring every single metadata from g
				parseSVGContent(svg_group, child, defs);
			}
			else if (child->name == "use")
			{
				parseUseTag(svg_group, child, defs);
			}
			else if (child->name == "defs")
			{
				// Do nothing, defs has been handled
			}
			else
			{
				std::cout << child->name << " has not yet been implemented!" << std::endl;
				throw NotImplementedException("Element not handled yet!");
			}
		}
	}

	void parseDefsTag(std::unordered_map<std::string, SVGObject>& defs, XMLNode* defs_element)
	{
		for (size_t i = 0; i < defs_element->GetChildCount(); i++)
		{
			XMLNode* child = defs_element->GetChild(i);
			if (child->name == "path")
			{
				std::string id = child->attributes["id"];
				if (id == "")
				{
					throw SVGParsingException("Can't have a path element without id inside defs!");
				}
				parsePathData(defs[id].path, child->attributes["d"].c_str());
			}
			else
			{
				throw SVGParsingException("Found an unexpected element inside <defs>");
			}
		}
	}

	std::unique_ptr<SVGGroup> SVG::parseSVG(std::string_view path)
	{
		XMLParser xml(path);
		XMLNode* root = xml.GetRootElement();

		if (root == nullptr)
		{
			std::cout << "Failed to parse SVG file at:" << path << std::endl;
			return nullptr;
		}

		auto svg_group = std::make_unique<SVGGroup>();

		try
		{
			parseTagSVG();

			std::unordered_map<std::string, SVGObject> defs;
			XMLNode* defs_element = root->GetFirstChild("defs");
			if (defs_element)
			{
				parseDefsTag(defs, defs_element);
			}

			parseSVGContent(*svg_group, root, defs);
		}
		catch (SVGParsingException& exception)
		{
			std::cout << "[SVG PARSER] " << exception.what() << std::endl;
		}

		svg_group->Scale({ 1.0f, -1.0f });
		svg_group->CalculateBoundingBox();

		//TODO normalize and shift to zero
		// this implies implementing some sort of Translate function to the curves

		return svg_group;
	}

	void SVGGroup::Scale(glm::vec2 scale)
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			SVGObject& object = *objects[i];
			for (size_t j = 0; j < object.path.GetSubpathCount(); j++)
			{
				SubPath& subpath = object.path.GetSubpath(j);

				for (size_t k = 0; k < subpath.GetPathElementsCount(); k++)
				{
					subpath.GetPathElement(k)->Scale(scale);
				}
			}
		}
	}

	void SVGGroup::CalculateBoundingBox()
	{
		for (const auto& object : objects)
		{
			bbox.min = glm::min(object->path.bbox.min, bbox.min);
			bbox.max = glm::max(object->path.bbox.max, bbox.max);
		}
	}

}
module;
#include <glm/glm.hpp>
module SVG.Path;

inline void PathElement::ComparePathPointToBoundingBox(float t, BBox& box) const
{
	if (0.0f < t && t < 1.0f)
	{
		glm::vec2 value = operator()(t);
		box.min = glm::min(value, box.min);
		box.max = glm::max(value, box.max);
	}
}

/////////////// Line ///////////////

Line::Line(glm::vec2 c0, glm::vec2 c1) : _c0(c0), _c1(c1)
{
	type = PathElementType::LINE;
	approximate_length = CalculateApproximateLength();
}

glm::vec2 Line::operator()(float t) const
{
	return _c0 * (1 - t) + t * _c1;
}

float Line::CalculateApproximateLength()
{
	return glm::length(_c1 - _c0);
}

BBox Line::CalculateBoundingBox()
{
	BBox box;
	box.min = { fminf(_c0.x, _c1.x), fminf(_c0.y, _c1.y) };
	box.max = { fmaxf(_c0.x, _c1.x), fmaxf(_c0.y, _c1.y) };
	return box;
}

void Line::Scale(glm::vec2 scale)
{
	approximate_length *= scale.x * scale.y;
	_c0 = (scale * _c0);
	_c1 = (scale * _c1);
}

/////////////// Quadratic Bezier ///////////////

QuadraticBezier::QuadraticBezier(glm::vec2 c0, glm::vec2 c1, glm::vec2 c2) : _c0(c0), _c1(c1), _c2(c2)
{
	type = PathElementType::QUADRATIC;
	approximate_length = CalculateApproximateLength();
}

glm::vec2 QuadraticBezier::operator()(float t) const
{
	float t2 = t * t;
	float mt = 1 - t;
	float mt2 = mt * mt;
	return _c0 * mt2 + _c1 * mt * 2.0f * t + _c2 * t2;
}

float QuadraticBezier::CalculateApproximateLength()
{
	float lower_bound = glm::length(_c2 - _c0);
	float upper_bound = glm::length(_c2 - _c1) + glm::length(_c1 - _c0);
	return (lower_bound + upper_bound) / 2.0f;
}

BBox QuadraticBezier::CalculateBoundingBox()
{
	BBox box;

	glm::vec2 denom = (_c0 + _c2);

	// X solution
	if (abs(denom.x) > 0.001f)
	{
		float t = ((_c0 + _c1) / denom).x;
		ComparePathPointToBoundingBox(t, box);
	}

	// Y solution
	if (abs(denom.y) > 0.001f)
	{
		float t = ((_c0 + _c1) / denom).y;
		ComparePathPointToBoundingBox(t, box);
	}

	// t = 0
	box.min = glm::min(_c0, box.min);
	box.max = glm::max(_c0, box.max);

	box.min = glm::min(_c2, box.min);
	box.max = glm::max(_c2, box.max);

	return box;
}

void QuadraticBezier::Scale(glm::vec2 scale)
{
	approximate_length *= scale.x * scale.y;
	_c0 = (scale * _c0);
	_c1 = (scale * _c1);
	_c2 = (scale * _c2);
}

/////////////// Cubic Bezier ///////////////

CubicBezier::CubicBezier(glm::vec2 c0, glm::vec2 c1, glm::vec2 c2, glm::vec2 c3) : _c0(c0), _c1(c1), _c2(c2), _c3(c3)
{
	type = PathElementType::CUBIC;
	approximate_length = CalculateApproximateLength();
}

glm::vec2 CubicBezier::operator()(float t) const
{
	float t2 = t * t;
	float t3 = t2 * t;
	float mt = 1 - t;
	float mt2 = mt * mt;
	float mt3 = mt2 * mt;
	return _c0 * mt3 + _c1 * 3.0f * mt2 * t + _c2 * 3.0f * mt * t2 + _c3 * t3;
}

float CubicBezier::CalculateApproximateLength()
{
	float lower_bound = glm::length(_c3 - _c0);
	float upper_bound = glm::length(_c3 - _c2) + glm::length(_c2 - _c1) + glm::length(_c1 - _c0);
	return (lower_bound + upper_bound) / 2.0f;
}

void CubicBezier::Scale(glm::vec2 scale)
{
	approximate_length *= scale.x * scale.y;
	_c0 = (scale * _c0);
	_c1 = (scale * _c1);
	_c2 = (scale * _c2);
	_c3 = (scale * _c3);
}

BBox CubicBezier::CalculateBoundingBox()
{
	BBox box;

	// Cubic solutions
	glm::vec2 a = (-_c0 + 3.0f * _c1 - 3.0f * _c2 + _c3);
	glm::vec2 b = (2.0f * _c0 - 4.0f * _c1 + 2.0f * _c2);
	glm::vec2 c = (_c1 - _c0);

	glm::vec2 delta = b * b - 4.0f * a * c;

	// For X
	if (delta.x > 0.0f) // two sols
	{
		float t = ( - b.x + sqrtf(delta.x) ) / (2.0f * a.x);
		ComparePathPointToBoundingBox(t, box);
		t = (-b.x - sqrtf(delta.x)) / (2.0f * a.x);
		ComparePathPointToBoundingBox(t, box);
	}
	else if (delta.x == 0.0f) // one sol
	{
		float t = -b.x / (2.0f * a.x);
		ComparePathPointToBoundingBox(t, box);
	}

	// For Y
	if (delta.y > 0.0f) // two sols
	{
		float t = (-b.y + sqrtf(delta.y)) / (2.0f * a.y);
		ComparePathPointToBoundingBox(t, box);
		t = (-b.y - sqrtf(delta.y)) / (2.0f * a.y);
		ComparePathPointToBoundingBox(t, box);
	}
	else if (delta.y == 0.0f) // one sol
	{
		float t = -b.y / (2.0f * a.y);
		ComparePathPointToBoundingBox(t, box);
	}

	// t = 0
	box.min = glm::min(_c0, box.min);
	box.max = glm::max(_c0, box.max);
	// t = 1
	box.min = glm::min(_c2, box.min);
	box.max = glm::max(_c2, box.max);

	return box;
}

/////////////////////////////////////////////

float SubPath::CalculateApproximateLength() 
{
	float length = 0.0f;

	for (size_t i = 0; i < _path_elements.size(); i++)
	{
		length += _path_elements[i]->approximate_length;
	}
	approximate_length = length;
	return approximate_length;
}

BBox SubPath::CalculateBoundingBox()
{
	for (const auto& element : _path_elements)
	{
		BBox box = element->CalculateBoundingBox();
		this->bbox.min = glm::min(box.min, bbox.min);
		this->bbox.max = glm::max(box.max, bbox.max);
	}
	return bbox;
}

void Path::BeginSubpath()
{
	_subpaths.emplace_back();
	_active_subpath = _subpaths.size() - 1;
}

bool Path::EndSubpath()
{
	if (_active_subpath == -1) 
	{ 
		return false; 
	}

	if (_subpaths[_active_subpath].IsEmpty())
	{
		return false;
	}

	_subpaths[_active_subpath].CalculateApproximateLength();
	_subpaths[_active_subpath].CalculateBoundingBox();
	if (_active_subpath)
	{
		_subpaths[_active_subpath].is_hole = true;
	}
	return true;
}

BBox Path::CalculateBoundingBox()
{
	for (const auto& subpath : _subpaths)
	{
		BBox subbox = subpath.bbox;
		bbox.min = glm::min(subbox.min, bbox.min);
		bbox.max = glm::max(subbox.max, bbox.max);
	}
	return bbox;
}
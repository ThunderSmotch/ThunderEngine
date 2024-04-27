module;
#include <glm/glm.hpp>
#include "imgui/imgui.h"
module Animation.LineTest;

import std;

using namespace ThunderLib;

glm::vec2 PolyLine::GetPoint(int index)
{
	if (index < 0 || index >= _points.size())
		return glm::vec2();

	return _points[index];
}


void PolyLine::SetPoint(glm::vec2 point, int index)
{
	if (index < 0 || index >= _points.size())
		return;

	_points[index] = point;
}

void PolyLine::ClosePath()
{
	if (_points.size() < 2) // Should log a warning
		return;
	_closed = true; 
	this->AddPoint(_points[0]);
}

// Draw feathered line segment (6 tris per segment)
void DrawLineSegment(const glm::vec2 start, const glm::vec2 end, const glm::vec2 normal, float width, float aa_width = 1.0f)
{
	if (width < 4.0f)
	{
		aa_width = width/4;
	}

	glm::vec2 width_normal = normal * (width / 2);

	// Draw first segment
	Renderer2D::DrawTriangle(start - width_normal, end - width_normal, end + width_normal, Color::Black);
	Renderer2D::DrawTriangle(start - width_normal, end + width_normal, start + width_normal, Color::Black);

	// TODO  Feathering -> This should really be optimized
	Renderer2D::DrawTriangle(
		start - width_normal, Color::Black,
		start - width_normal - normal * aa_width, glm::vec4(0.0f),
		end - width_normal, Color::Black
	);
	Renderer2D::DrawTriangle(
		start - width_normal - normal * aa_width, glm::vec4(0.0f),
		end - width_normal - normal * aa_width, glm::vec4(0.0f),
		end - width_normal, Color::Black
	);

	Renderer2D::DrawTriangle(
		start + width_normal, Color::Black,
		end + width_normal, Color::Black,
		end + width_normal + normal * aa_width, glm::vec4(0.0f)
	);
	Renderer2D::DrawTriangle(start + width_normal, Color::Black,
		end + width_normal + normal * aa_width, glm::vec4(0.0f),
		start + width_normal + normal * aa_width, glm::vec4(0.0f)
	);
}

// Intersects lines A and B
std::pair<bool, glm::vec2> intersect(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2)
{
	float denominator = (a1.x - a2.x) * (b1.y - b2.y) - (a1.y - a2.y) * (b1.x - b2.x);

	if (abs(denominator) < 0.001f)
	{
		return std::make_pair<bool, glm::vec2>(false, glm::vec2());
	}

	float nx = (a1.x * a2.y - a1.y * a2.x) * (b1.x - b2.x) - (a1.x - a2.x) * (b1.x * b2.y - b1.y * b2.x);
	float ny = (a1.x * a2.y - a1.y * a2.x) * (b1.y - b2.y) - (a1.y - a2.y) * (b1.x * b2.y - b1.y * b2.x);
	
	return std::make_pair<bool, glm::vec2>(true, glm::vec2(nx/denominator, ny/denominator));
}

// Draws bevel at the joint of segments
void DrawBevel(const glm::vec2 intersection, const glm::vec2 previous_direction, const glm::vec2 direction, float width, float aa_width = 1.0f)
{
	if (width < 4.0f)
	{
		aa_width = width / 4;
	}

	auto previous_normal = glm::vec2({ -previous_direction.y, previous_direction.x });
	auto normal = glm::vec2({ -direction.y, direction.x });

	glm::vec2 previous_width_normal = previous_normal * (width / 2);
	glm::vec2 width_normal = normal * (width / 2);
	
	glm::vec2 bevel_normal = glm::normalize(previous_direction - direction);

	// If positive then, when facing direction, the bevel should be on the right side of the line, aka negative normal.
	float cross_product = previous_direction.x * direction.y - previous_direction.y * direction.x;
	float sign = (cross_product > 0.0f ? 1.0f : -1.0f);

	// Miter intersection point
	glm::vec2 outer_previous = intersection - sign * previous_width_normal;
	glm::vec2 outer = intersection - sign * width_normal;

	auto [intersected, miter_intersection] = intersect(
		outer_previous,
		outer_previous + previous_direction,
		outer,
		outer + direction
		);

	// If the angle between directions is too large, dont bother with the miter
	if (direction.x * previous_direction.x + direction.y * previous_direction.y < -0.5f)
	{
		intersected = false;
	}

	// Always draw inner bevel
	Renderer2D::DrawTriangle(intersection, outer_previous, outer, Color::Black);

	if (intersected)
	{
		// Draw miter
		Renderer2D::DrawTriangle(outer_previous, outer, miter_intersection, Color::Black);

		Renderer2D::DrawTriangle(outer_previous, Color::Black,
			miter_intersection, Color::Black,
			miter_intersection - sign * previous_normal * aa_width, glm::vec4(0.0f)
			);
		Renderer2D::DrawTriangle(outer_previous, Color::Black,
			miter_intersection - sign * previous_normal * aa_width, glm::vec4(0.0f),
			outer_previous - sign * previous_normal * aa_width, glm::vec4(0.0f)
		);

		Renderer2D::DrawTriangle(
			outer, Color::Black,
			miter_intersection, Color::Black,
			miter_intersection - sign * normal * aa_width, glm::vec4(0.0f)
		);
		Renderer2D::DrawTriangle(outer, Color::Black,
			miter_intersection - sign * normal * aa_width, glm::vec4(0.0f),
			outer - sign * normal * aa_width, glm::vec4(0.0f)
		);

		Renderer2D::DrawTriangle(
			miter_intersection, Color::Black,
			miter_intersection - sign * previous_normal * aa_width, glm::vec4(0.0f),
			miter_intersection - sign * (previous_normal  + normal) * aa_width, glm::vec4(0.0f)
		);
		Renderer2D::DrawTriangle(
			miter_intersection, Color::Black,
			miter_intersection - sign * normal * aa_width, glm::vec4(0.0f),
			miter_intersection - sign * (previous_normal + normal) * aa_width, glm::vec4(0.0f)
		);
	}
	else // Bevel feathering
	{
		// TODO The below works but triangles are either drawn CCW or CW depending on sign (this may have implications with Face Culling in the future!)
		Renderer2D::DrawTriangle(
			outer_previous, Color::Black,
			outer_previous + bevel_normal * aa_width, glm::vec4(0.0f),
			outer, Color::Black
		);
		Renderer2D::DrawTriangle(
			outer_previous + bevel_normal * aa_width, glm::vec4(0.0f),
			outer + bevel_normal * aa_width, glm::vec4(0.0f),
			outer, Color::Black
		);
	}
}

// TODO move polyline render code to a shader implementation
void PolyLine::Render(float end_percentage, float start_percentage) const
{
	if (_points.size() < 2 || end_percentage <= start_percentage)
		return;

	// Percentage related calculations
	int num_of_segments = (int) _points.size() - 1;
	int start_incomplete_segments = static_cast<int>(start_percentage * num_of_segments);
	int end_complete_segments = static_cast<int>(end_percentage * num_of_segments);
	
	int segments = end_complete_segments - start_incomplete_segments;
	float start_segment_percentage = (start_percentage * num_of_segments) - start_incomplete_segments;
	float end_segment_percentage = (end_percentage * num_of_segments) - end_complete_segments;

	// TODO Start cap

	// Middle section
	glm::vec2 start = _points[start_incomplete_segments];
	glm::vec2 end = _points[start_incomplete_segments + 1];

	// FIXME end and start depend on previous end and start
	if (segments == 0)
	{
		end = start + (end - start) * end_segment_percentage;
	}
	start = start + (_points[start_incomplete_segments + 1] - start) * start_segment_percentage;

	glm::vec2 direction = glm::normalize(end - start);
	glm::vec2 normal = { -direction.y, direction.x };

	DrawLineSegment(start, end, normal, _width);
	glm::vec previous_direction = direction;

	int segments_to_draw = segments + (end_segment_percentage > 0.0f ? 1 : 0) - 1;
	
	for (int i = 0; i < segments_to_draw; i++)
	{
		start = end;
		end = _points[start_incomplete_segments + i + 2];

		if (i == segments_to_draw - 1 && end_segment_percentage > 0.0f)
		{
			end = start + (end - start) * end_segment_percentage;
		}

		direction = glm::normalize(end - start);
		normal = { -direction.y, direction.x };

		DrawLineSegment(start, end, normal, _width);
		DrawBevel(start, previous_direction, direction, _width);

		previous_direction = direction;
	}

	// Draw last bevel if curve is closed
	if (_closed && segments == num_of_segments)
	{
		start = _points[start_incomplete_segments];
		end = _points[start_incomplete_segments + 1];
		direction = glm::normalize(end - start);
		DrawBevel(start, previous_direction, direction, _width);
	}

	// End cap

}

void BuildPolyLineFromQuadraticBezier(PolyLine& polyline, QuadraticBezier& bezier)
{
	int num_segments = 20;
	float range_dt = 1.0f / num_segments;

	float t = 0.0f;
	for (size_t i = 0; i <= num_segments; i++)
	{
		polyline.AddPoint(bezier(t));
		t += range_dt;
	}
}

void BuildPolyLineFromCubicBezier(PolyLine& polyline, CubicBezier& bezier)
{
	int num_segments = 20;
	float range_dt = 1.0f / num_segments;

	float t = 0.0f;
	for (size_t i = 0; i <= num_segments; i++)
	{
		polyline.AddPoint(bezier(t));
		t += range_dt;
	}
}

void PolyLine::BuildFromSubPath(SubPath& subpath)
{
	this->AddPoint(subpath._path_elements[0]->operator()(0.0f));

	size_t num_elements = subpath._path_elements.size();
	if (subpath.closed)
		num_elements--;

	for (size_t i = 0; i < num_elements; i++)
	{
		PathElement* element = subpath._path_elements[i].get();
		switch (element->type)
		{
		using enum PathElementType;
		case LINE:
			this->AddPoint(((Line*)element)->operator()(1.0f));
			break;
		case QUADRATIC:
			BuildPolyLineFromQuadraticBezier(*this, *(QuadraticBezier*)element);
			break;
		case CUBIC:
			BuildPolyLineFromCubicBezier(*this, *(CubicBezier*)element);
			break;
		}
	}

	if(subpath.closed)
		this->ClosePath();

	this->SetWidth(1.0f);
}

LineTest::LineTest()
{
	// Testing XML parser

	//printFile("res/xml/hex.svg");
	//printFile("res/xml/bomtest.xml");
	//SVGParser svg_parser("res/xml/triangle.svg");
	svg = SVG::parseSVG("res/xml/cala.svg");

	for (const auto& object : svg->objects)
	{
		for (size_t i = 0; i < object->path.GetSubpathCount(); i++)
		{
			PolyLine& poly = polylines.emplace_back();
			poly.BuildFromSubPath(object->path.GetSubpath(i));
		}
	}

	Path path;
	path.BeginSubpath();
	path.AddPathElement(std::make_unique<CubicBezier>(
		glm::vec2(-300.0, 300.0),
		glm::vec2(-300.0, 200.0),
		glm::vec2(-200.0, 300.0),
		glm::vec2(-200.0, 200.0)
	));
	path.AddPathElement(std::make_unique<Line>(
		glm::vec2(-200.0, 200.0),
			glm::vec2(-200.0, 100.0)
	));
	path.AddPathElement(std::make_unique<Line>(
		glm::vec2(-200.0, 100.0),
		glm::vec2(-300.0, 300.0)
	));
	path.SetClosed();

	T.BuildFromSubPath(path.GetSubpath());
	T.SetWidth(2.0f);

	L.SetWidth(20.0f);
	L.AddPoint({ -400.0, -200.0 });
	L.AddPoint({ -500.0, -200.0 });
	L.AddPoint({ -500.0, -300.0 });
	L.AddPoint({ -400.0, -300.0 });
	L.ClosePath();

	auto bezier = QuadraticBezier({ -200.0f, 100.0f }, { 0.0f, 400.0f }, { 200.0f, 100.0f });
	BuildPolyLineFromQuadraticBezier(Q, bezier);
	Q.SetWidth(6.0f);

	auto bezier2 = CubicBezier(
		{ -200.0f, -100.0f },
		{200.0f, -100.0f},
		{-200.0f, -300.0f},
		{200.0f, -300.0f}
	);
	BuildPolyLineFromCubicBezier(C, bezier2);
	C.SetWidth(20.0f);
}

void LineTest::Render(float dt)
{
	static float render_start_percentage = 0.60f;
	static float render_end_percentage = 0.80f;

	for (const auto& poly : polylines)
	{
		poly.Render(render_end_percentage, render_start_percentage);
	}

	ImGui::Begin("Test Line Draw");
	ImGui::SliderFloat("Start Percentage", &render_start_percentage, 0.0f, 1.0f);
	ImGui::SliderFloat("End Percentage", &render_end_percentage, 0.0f, 1.0f);
	ImGui::End();

	L.Render();
	T.Render();

	// Weird hardcoded state machine to loop start to end and back
	static Timer timer;
	static int stage = 0;

	static float prev_value = 0.0f;
	float start_percentage = 0.0f;
	float end_percentage = 0.0f;

	switch (stage) {
	case 0:
		start_percentage = 0.0f;
		end_percentage = abs(sin(timer.Elapsed()));
		if (prev_value > end_percentage)
		{
			timer.Reset();
			stage = 1;
			prev_value = 0.0f;
			break;
		}
		prev_value = end_percentage;
		break;
	case 1:
		end_percentage = 1.0f;
		start_percentage = abs(sin(timer.Elapsed()));
		if (prev_value > start_percentage)
		{
			timer.Reset();
			stage = 2;
			prev_value = 1.0f;
			break;
		}
		prev_value = start_percentage;
		break;
	case 2:
		end_percentage = 1.0f;
		start_percentage = abs(cos(timer.Elapsed()));
		if (prev_value < start_percentage)
		{
			timer.Reset();
			stage = 3;
			prev_value = 1.0f;
			break;
		}
		prev_value = start_percentage;
		break;
	case 3:
		start_percentage = 0.0f;
		end_percentage = abs(cos(timer.Elapsed()));
		if (prev_value < end_percentage)
		{
			timer.Reset();
			stage = 0;
			prev_value = 0.0f;
			break;
		}
		prev_value = end_percentage;
		break;
	}
	Q.Render(end_percentage, start_percentage);
	C.Render(end_percentage, start_percentage);
	// start a 0 end from 0 to 1
	// end a 1 start from 0 to 1
	// end a 1 start from 1 to 0
	// start a 0 end from 1 to 0

}

void LineTest::Update(float dt)
{

}

void LineTest::ProcessInput(ThunderLib::MouseInput& mouse)
{
	if (mouse.GetIsKeyDown(MouseKeyCode::MOUSE_1))
	{
		//RendererCommand::SetWireframeMode(true);
	}
	return;
	static float angle = 0.0f;
	if (mouse.GetIsKeyDown(MouseKeyCode::MOUSE_LEFT))
	{
		angle += 0.01f;
		glm::vec2 origin = L.GetPoint(1);
		L.SetPoint({ origin.x + 100.0f*sin(angle), origin.y + 100.0*cos(angle)}, 2);
	}
	else if (mouse.GetIsKeyDown(MouseKeyCode::MOUSE_RIGHT))
	{
		angle -= 0.01f;
		glm::vec2 origin = L.GetPoint(1);
		L.SetPoint({ origin.x + 100.0f * sin(angle), origin.y + 100.0 * cos(angle) }, 2);
	}
}
module;
#include <glm/glm.hpp>
export module Animation.LineTest;

import std;
import ThunderLib;
import Animation.Scene;
import SVG.SVGParser;
import SVG.Path;

export{

	class PolyLine
	{
	private:
		float _width = 10.0f;
		bool _closed = false;
		std::vector<glm::vec2> _points;

	public:
		void AddPoint(glm::vec2 point) { _points.push_back(point); }
		glm::vec2 GetPoint(int index);
		void SetPoint(glm::vec2 point, int index);
		void SetWidth(float width) { _width = width; }
		void ClosePath();
		void Render(float end_percentage = 1.0f, float start_percentage = 0.0f) const;

		void BuildFromSubPath(SubPath&);
	};

	class LineTest : public Scene {
	private:
		PolyLine L;
		PolyLine T;
		PolyLine Q;
		PolyLine C;

		std::unique_ptr<SVG::SVGGroup> svg;

		std::vector<PolyLine> polylines;
		PolyLine poly1;
		PolyLine poly2;

	public:
		LineTest();
		void Render(float dt) override;
		void Update(float dt) override;
		void ProcessInput(ThunderLib::MouseInput& mouse) override;
	};
}
module;
#include <glm/glm.hpp>
export module SVG.Path;

import std;

export {

	enum class PathElementType
	{
		LINE,
		QUADRATIC,
		CUBIC,
	};

	struct BBox
	{
		glm::vec2 min = glm::vec2(1e30f);
		glm::vec2 max = glm::vec2(-1e30f);
	};

	class PathElement
	{
	public:
		PathElementType type = PathElementType::LINE;
		float approximate_length = 0.0f;

		PathElement() = default;
		virtual ~PathElement() = default;
		virtual glm::vec2 operator()(float t) const = 0;
		virtual float CalculateApproximateLength() = 0;
		virtual BBox CalculateBoundingBox() = 0;
		virtual void Scale(glm::vec2 scale = { 1.0f, 1.0f }) = 0;

	protected:
		inline void ComparePathPointToBoundingBox(float t, BBox& bbox) const;
	};

	class Line : public PathElement
	{
	public:
		glm::vec2 _c0;
		glm::vec2 _c1;

		Line(glm::vec2 c0, glm::vec2 c1);

		glm::vec2 operator()(float t) const final;
		float CalculateApproximateLength() final;
		BBox CalculateBoundingBox() final;

		void Scale(glm::vec2 scale = { 1.0f, 1.0f }) final;
	};

	class QuadraticBezier : public PathElement
	{
	public:
		glm::vec2 _c0;
		glm::vec2 _c1;
		glm::vec2 _c2;

		QuadraticBezier(glm::vec2 c0, glm::vec2 c1, glm::vec2 c2);

		//TODO Create quadratic Bezier from three points

		glm::vec2 operator()(float t) const final;
		float CalculateApproximateLength() final;
		BBox CalculateBoundingBox() final;

		void Scale(glm::vec2 scale = { 1.0f, 1.0f }) final;
	};

	class CubicBezier : public PathElement
	{
	public:
		glm::vec2 _c0;
		glm::vec2 _c1;
		glm::vec2 _c2;
		glm::vec2 _c3;

		CubicBezier(glm::vec2 c0, glm::vec2 c1, glm::vec2 c2, glm::vec2 c3);

		//TODO Create cubic Bezier from three points

		virtual glm::vec2 operator()(float t) const final;
		float CalculateApproximateLength() final;
		BBox CalculateBoundingBox() final;

		void Scale(glm::vec2 scale = { 1.0f, 1.0f }) final;
	};

	class SubPath
	{
	public:
		float approximate_length = 0.0f;
		bool closed = false;
		bool is_hole = false;
		BBox bbox;

	private:
		std::vector<std::unique_ptr<PathElement>> _path_elements;
	public:
		friend class PolyLine;

		void AddPathElement(std::unique_ptr<PathElement>&& path_element) { _path_elements.push_back(std::move(path_element)); }

		size_t GetPathElementsCount() const { return _path_elements.size(); }
		PathElement* GetPathElement(size_t index) { return _path_elements[index].get(); }
		bool IsEmpty() const { return _path_elements.empty(); }

		float CalculateApproximateLength();
		BBox CalculateBoundingBox();
	};

	class Path
	{
	private:
		std::vector<SubPath> _subpaths;
		size_t _active_subpath = -1;

	public:
		BBox bbox;

		void BeginSubpath();
		bool EndSubpath();

		void AddPathElement(std::unique_ptr<PathElement>&& path_element) { _subpaths[_active_subpath].AddPathElement(std::move(path_element)); }
		void SetClosed() { _subpaths[_active_subpath].closed = true; }
		void SetOpen() { _subpaths[_active_subpath].closed = false; }

		size_t GetSubpathCount() const { return _subpaths.size(); }
		SubPath& GetSubpath(size_t i = 0) { return _subpaths[i]; }
		BBox CalculateBoundingBox();
	};

}
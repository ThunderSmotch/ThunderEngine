#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

import std;

namespace ThunderEngine 
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: projection_matrix_(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
	{
		view_projection_matrix_ = projection_matrix_ * view_matrix_;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		projection_matrix_ = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		view_projection_matrix_ = projection_matrix_ * view_matrix_;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		// This is like a model matrix for the camera
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position_) * 
			glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(0, 0, 1));

		// Note: (Carlos) How View Matrices are calculated
		// To pass world coords to view coords we need to invert the transformation that we apply to the camera.
		// We "translate" the world to the camera and then "rotate it", this is what the inverse matrix does.
		view_matrix_ = glm::inverse(transform); 
		// PERF Do we need to numerically invert the matrix? Isn't this solvable analitically?
		// Maybe GLM already does that.
		view_projection_matrix_ = projection_matrix_ * view_matrix_;
	}
}


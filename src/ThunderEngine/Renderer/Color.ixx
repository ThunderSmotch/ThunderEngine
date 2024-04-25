module;
#include <glm/glm.hpp>
export module ThunderEngine.Color;

export namespace Color {
	glm::vec4 const Black     = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 const White     = glm::vec4(1.0f);
	glm::vec4 const Red       = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 const Green     = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 const Blue	  = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 const Yellow    = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 const Grey      = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	glm::vec4 const DarkGrey  = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
	glm::vec4 const LightGrey = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);
}
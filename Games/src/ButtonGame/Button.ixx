module;
#include <glm/glm.hpp>
export module ButtonGame.Button;

import ThunderEngine;
import Animation.Scene;

export class ButtonMachine : public Scene
{
private:
	
	// Button Machine
	float width;
	float height;
	float border_size;
	float rotation;

	// Button
	float position; // From -1.0f to 1.0f percentage of the way through
	glm::vec2 button_position;
	glm::vec4 button_color;

public:
	ButtonMachine();

	void Render(float dt) override;
	void Update(float dt) override;
	void ProcessInput(ThunderEngine::MouseInput& mouse) override;
};
export module Animation.Intro;

import ThunderLib;
import Animation.Scene;

export class IntroScene: public Scene {
private:
	float text_y = 300.0f;
	bool test_set = false;
	float text_move_speed = 100.0f;
	float text_alpha = 1.0f;
public:
	IntroScene();
	
	void Render(float dt) override;
	void Update(float dt) override;
	void ProcessInput(ThunderLib::MouseInput& mouse) override;
};
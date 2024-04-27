export module Animation.AnimationTest;

import ThunderEngine;
import Animation.Animation;
import Animation.Scene;

export class AnimationScene: public Scene {
private:
	Animatable box;
	Cutscene box_fade_in;
public:
	AnimationScene();

	void Render(float dt) override;
	void Update(float dt) override;
	void ProcessInput(ThunderEngine::MouseInput& mouse) override;
};
export module Animation.Scene;

import ThunderEngine;

export class Scene {
private:

public:
	virtual ~Scene() = default;
	virtual void Render(float dt) = 0;
	virtual void Update(float dt) = 0;
	
	// TODO: Rethink how to pass inputs to the processInput func
	virtual void ProcessInput(ThunderEngine::MouseInput& mouse) = 0;
};
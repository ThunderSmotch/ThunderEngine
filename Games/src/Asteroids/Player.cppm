module;
#include <glm/glm.hpp>
export module Asteroids.Player;

import ThunderLib;

export {
	class Player
	{
	public:
		Player();

		void Render() const;
		void HandleInput(float dt);

		void SetTexture(ThunderLib::Ref<ThunderLib::Texture2D> texture) { texture_ = texture; };

		glm::vec2 GetPosition() const { return position_; }
		void SetPosition(glm::vec2 new_position) { position_ = new_position; }

	private:
		glm::vec2 position_ = glm::vec2(400.0f);
		glm::vec2 size_ = glm::vec2(40, 40);
		float rotation_ = 0.0f;
		float rotation_speed_ = 5.0f;
		float speed_ = 200.0f;

		ThunderLib::Ref<ThunderLib::Texture2D> texture_;
		ThunderLib::Ref<ThunderLib::KeyInput> input_;
	};
}
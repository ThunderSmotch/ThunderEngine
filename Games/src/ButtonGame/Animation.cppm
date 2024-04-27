module;
#include <glm/glm.hpp>
#define ANIMATE(animation, ...) Add(CreateRef<animation>(__VA_ARGS__))
export module Animation.Animation;

import std;
import ThunderLib;

export{

	class Animatable
	{
	private:
		glm::vec3 position_;
		float rotation_;
		glm::vec2 size_;
		glm::vec4 tint_;

	public:
		Animatable(glm::vec3 position = { 0.0f, 0.0f, 0.0f }, float rotation = 0.0f, glm::vec2 size = { 10.0f, 10.0f }, glm::vec4 tint = Color::Red);

		void Render() const;
		void Update(float dt);

		glm::vec3 GetPosition() const { return position_; }
		void SetPosition(glm::vec3 position) { position_ = position; }

		float GetAlpha() const { return tint_.a; }
		void SetAlpha(float alpha) { tint_.a = alpha; }
	};

	class Animation
	{
	protected:
		Animatable* animatables_ = nullptr;
		int number_of_animatables = 0;
		Animatable& animatable_;

		bool has_ended_ = false;
		float duration_ = 1.0f;   // One second default duration
		float time_passed_ = 0.0f;

	public:
		explicit Animation(Animatable& obj) : animatable_(obj) { }
		virtual ~Animation() = default;
		virtual void SetUp() { }
		virtual void UpdateValues(float percent_complete) {
			// To be overriden
		}
		virtual void Update(float dt) {
			UpdateValues(time_passed_ / duration_);

			time_passed_ += dt;
			if (duration_ < time_passed_)
			{
				has_ended_ = true;
			}
		}
		virtual bool HasEnded() { return has_ended_; }
		virtual void Reset() { has_ended_ = false; time_passed_ = 0.0f; }
	};

	class FadeIn : public Animation
	{
	public:
		FadeIn(Animatable& obj) : Animation(obj) {}

		void UpdateValues(float percent_complete) override
		{
			animatable_.SetAlpha(percent_complete); // TODO Easing functions here
		}
	};

	class Wait : public Animation
	{
	public:
		Wait(Animatable& obj) : Animation(obj) {}

		void UpdateValues(float percent_complete) override
		{

		}
	};

	class MoveTo : public Animation
	{
	private:
		float x_;
		float y_;
		glm::vec3 original_position_;
	public:
		MoveTo(Animatable& obj, float x, float y) : Animation(obj), x_(x), y_(y), original_position_(obj.GetPosition()) {}

		void SetUp() override
		{
			original_position_ = animatable_.GetPosition();
		}

		void UpdateValues(float percentage) override
		{
			glm::vec3 position = {
				x_ * percentage + (1 - percentage) * original_position_.x,
				y_ * percentage + (1 - percentage) * original_position_.y,
				original_position_.z
			};
			animatable_.SetPosition(position);
		}
	};

	class Cutscene
	{
	private:
		std::vector<ThunderLib::Ref<Animation>> commands_;
		int current_animation_ = -1;
		bool active_ = false;
	public:
		Cutscene() = default;

		void Add(ThunderLib::Ref<Animation> animation);
		void Play();
		void Update(float dt);
	};

}
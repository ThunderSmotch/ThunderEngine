module;
#include <glm/glm.hpp>
module Animation.Animation;
using namespace ThunderLib;

Animatable::Animatable(glm::vec3 position, float rotation, glm::vec2 size, glm::vec4 tint)
	: position_(position), rotation_(rotation), size_(size), tint_(tint)
{

}

void Animatable::Render() const
{
	Renderer2D::DrawQuad(position_, rotation_, size_, tint_);
}

void Cutscene::Add(Ref<Animation> animation)
{
	commands_.push_back(animation);
}

void Cutscene::Play()
{
	active_ = true;
	current_animation_ = 0;
	commands_[current_animation_]->SetUp();
}

void Cutscene::Update(float dt)
{
	if(current_animation_ == -1)
	{
		return;
	}

	Animation& animation = *commands_[current_animation_];
	animation.Update(dt);

	if (animation.HasEnded())
	{
		animation.Reset();

		current_animation_++;
		if (current_animation_ == commands_.size())
		{
			current_animation_ = -1;
		} 
		else
		{
			commands_[current_animation_]->SetUp();
		}
	}

}
module;
#include <glm/glm.hpp>
module Asteroids.Player;

import ThunderLib;
import Asteroids.ProjectileSystem;
using namespace ThunderLib;

extern int the_canvas_width;
extern int the_canvas_height;

Player::Player()
{
	texture_ = Texture2D::Create("res/textures/container.jpg");
	input_ = KeyInput::Create({ KeyCode::W, KeyCode::A, KeyCode::S, KeyCode::D, KeyCode::SPACE });
}

void Player::Render() const
{
	constexpr float width = 5.0f;
	constexpr float height = 20.0f;

	// Points that define the shape of the player
	auto bottom_left = glm::vec2(-width, 0);
	auto bottom_right = glm::vec2(width, 0);
	auto top = glm::vec2(0, height);

	// Rotate points
	float cosine = cos(rotation_);
	float sine = sin(rotation_);
	auto rotation_matrix = glm::mat2x2(cosine, -sine, sine, cosine);

	bottom_left = (rotation_matrix * bottom_left);
	bottom_right = rotation_matrix * bottom_right;
	top = rotation_matrix * top;

	// Draws rotated player
	Renderer2D::DrawLine(position_ + bottom_left, position_ + top, glm::vec4(1.0f));
	Renderer2D::DrawLine(position_ + bottom_right, position_ + top, glm::vec4(1.0f));
	Renderer2D::DrawLine(position_ + bottom_left, position_ + bottom_right, glm::vec4(1.0f));
	//Renderer2D::DrawQuad(position_, size_, texture_, { 1.0f, 1.0f, 1.0f, 1.0f });
}

// TODO Better movement system with some sort of drift involved!
// As it is it's very responsive but real spaceships should be harder to control
void Player::HandleInput(float dt) {
	if (input_->GetIsKeyPress(KeyCode::SPACE))
	{
		// @Incomplete refactor this alongside redner code
		float cosine = cos(rotation_);
		float sine = sin(rotation_);
		auto rotation_matrix = glm::mat2x2(cosine, -sine, sine, cosine);
		auto top = rotation_matrix * glm::vec2(0, 20.0f);

		ProjectileSystem::SpawnProjectile(position_ + top, rotation_, true);
	}

	// TODO Avoid player out of bounds

	if (input_->GetIsKeyDown(KeyCode::W))
	{
		position_.x += dt * speed_ * sin(rotation_);
		position_.y += dt * speed_ * cos(rotation_);
	}
	if (input_->GetIsKeyDown(KeyCode::S))
	{
		position_.x -= dt * speed_ * sin(rotation_);
		position_.y -= dt * speed_ * cos(rotation_);
	}
	if (input_->GetIsKeyDown(KeyCode::D))
	{
		rotation_ += dt * rotation_speed_;
	}
	if (input_->GetIsKeyDown(KeyCode::A))
	{
		rotation_ -= dt * rotation_speed_;
	}

	// Handle loop around the field
	if (position_.x >= (float) the_canvas_width)
	{
		position_.x = 0.0f;
	}
	else if (position_.x <= 0)
	{
		position_.x = (float) the_canvas_width;
	}
	
	if (position_.y >= (float) the_canvas_height)
	{
		position_.y = 0.0f;
	}
	else if (position_.y <= 0)
	{
		position_.y = (float) the_canvas_height;
	}
}
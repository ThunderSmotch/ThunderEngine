module;
#include <imgui/imgui.h>
export module Game.Animation;

import std;
import ThunderLib;

import Easing;

/*

Animation System

- Should be rewindable. Why (?)
- Shouldn't it be the simplest thing?

- Want both translate and move to's to work.

- Do I want them serializable? Or is code good enough?

Specific animations:
- Creation and Deletion
- Rotate Around
- Change Scale

*/

ThunderLib::Ref<ThunderLib::KeyInput> input;
bool pause = true;
f32 time = 0.0f;

enum class AnimatableType
{
	NONE = 0,
	RECT,
	CIRCLE,
	SPRITE,
	LINE,
	SVG,

	LENGTH,
};

// TODO Delete CircleProps for this
struct Animatable
{
	AnimatableType type = AnimatableType::NONE;
	bool hidden = false;
	Vec3 position = { 0.0f, 0.0f, 0.0f };
	Vec2 size = { 1.0f, 1.0f };
	f32  rotation = 0.0f;
	Vec4 color = { 1.0f, 0.0f, 0.0f, 1.0f };
};

std::vector<Animatable> anim_obj;          // Current ones being changed
std::vector<Animatable> prev_anim_obj;     // Previous state of each circle
std::vector<Animatable> initial_anim_obj;  // Initial state of circles (needed to reset/loop animation)

enum class AnimationType
{
	Wait = 0,
	MoveTo,
	Scale,
	Rotate,
	Length
};

struct ScaleData
{
	f32 initial;
	f32 final;
};

struct MoveToData
{
	f32 final_x;
	f32 final_y;
};

struct RotateData
{
	f32 angle; // in degrees
};

struct Animation
{
	AnimationType type = AnimationType::Wait;
	f32 start_time = 0.0f;
	f32 end_time = 1.0f;
	bool started = false;
	bool ended = false;
	EasingType easing = EasingType::Linear;
	std::vector<u8> actors = {};
	union AnimationData
	{
		ScaleData scale;
		MoveToData move_to;
		RotateData rotate;
	} data;
};

void InitObjects()
{
	Animatable object;
	object.type = AnimatableType::RECT;
	object.size = { 0.5f, 0.5f };

	object.position = { 0.5f, 0.75f, 0.0f };
	object.color = { 0.0f, 0.0f, 0.0f, 1.0f };
	initial_anim_obj.push_back(object);

	object.position = { 0.5f, 0.25f, 0.0f };
	object.color = { 1.0f, 0.0f, 0.0f, 1.0f };
	initial_anim_obj.push_back(object);

	object.position = { 0.25f, 0.5f, 0.0f };
	object.color = { 0.0f, 1.0f, 1.0f, 1.0f };
	object.type = AnimatableType::CIRCLE;
	initial_anim_obj.push_back(object);

	object.position = { 0.75f, 0.5f, 0.0f };
	object.color = { 1.0f, 0.0f, 1.0f, 1.0f };
	object.type = AnimatableType::CIRCLE;
	initial_anim_obj.push_back(object);

	anim_obj = initial_anim_obj;
	prev_anim_obj = initial_anim_obj;
}

std::vector<Animation> animations;

void Rotate(std::vector<u8> ids, f32 start, f32 end, RotateData rotate, EasingType easing = EasingType::Linear)
{
	auto anim = Animation();
	anim.type = AnimationType::Rotate;
	anim.actors = ids;
	anim.start_time = start;
	anim.end_time = end;
	anim.data.rotate = rotate;
	anim.easing = easing;

	animations.push_back(anim);
}

void Scale(std::vector<u8> ids, f32 start, f32 end, ScaleData scale, EasingType easing = EasingType::Linear)
{
	auto anim = Animation();
	anim.type = AnimationType::Scale;
	anim.actors = ids;
	anim.start_time = start;
	anim.end_time = end;
	anim.data.scale = scale;
	anim.easing = easing;

	animations.push_back(anim);
}

void MoveTo(std::vector<u8> ids, f32 start, f32 end, MoveToData move_to, EasingType easing = EasingType::Linear)
{
	auto anim = Animation();
	anim.type = AnimationType::MoveTo;
	anim.actors = ids;
	anim.start_time = start;
	anim.end_time = end;
	anim.data.move_to = move_to;
	anim.easing = easing;

	animations.push_back(anim);
}

void Script()
{
	Scale({ 0, 1, 2, 3 }, 0.0f, 1.0f, { 0.5f, 1.0f }, EasingType::InOutBack);
	Scale({ 0, 1, 2, 3 }, 1.0f, 4.0f, { 1.0f, 0.0f }, EasingType::InOutCubic);
	MoveTo({ 0, 1, 2, 3 }, 1.0f, 4.0f, { 0.5f, 0.5f }, EasingType::InOutCubic);
	Scale({ 0 }, 4.0f, 5.0f, { 0.0f, 0.5f }, EasingType::InOutCubic);
	MoveTo({ 0 }, 4.0f, 6.0f, { 0.0f, 1.0f }, EasingType::InOutCubic);
	MoveTo({ 0 }, 6.0f, 8.0f, { 1.0f, 1.0f }, EasingType::InOutCubic);
	MoveTo({ 0 }, 8.0f, 10.0f, { 1.0f, 0.0f }, EasingType::InOutCubic);
	MoveTo({ 0 }, 10.0f, 12.0f, { 0.0f, 0.0f }, EasingType::InOutCubic);
	MoveTo({ 0 }, 12.0f, 14.0f, { 0.5f, 0.5f }, EasingType::InOutCubic);
	Scale({ 0 }, 13.5f, 16.0f, { 0.5f, 10.0f });
	Rotate({ 0 }, 12.0f, 16.0f, { 8*360.0f }, EasingType::InOutCubic);
}

void AnimationInit(Animation& anim)
{
	switch (anim.type)
	{
	case AnimationType::Rotate:
	case AnimationType::MoveTo:
		for (auto i : anim.actors)
		{
			prev_anim_obj[i] = anim_obj[i];
		}
		break;
	default:
		break;
	}
}


void Animate(f32 time, Animation& anim)
{
	if (time < anim.start_time || anim.ended)
	{
		return;
	}
	
	f32 duration = anim.end_time - anim.start_time;
	f32 elapsed = time - anim.start_time;
	f32 progress = elapsed / duration;
	progress = ease(progress, anim.easing);
	
	if (time > anim.end_time)
	{
		progress = 1.0f;
		anim.ended = true;
		// END
	}

	if (!anim.started)
	{
		progress = 0.0f;
		anim.started = true;
		AnimationInit(anim);
		// START
	}

	switch (anim.type)
	{
	case AnimationType::Wait:
		break;
	case AnimationType::Scale:
	{
		for (auto i : anim.actors)
		{
			float s = anim.data.scale.initial * (1 - progress) + anim.data.scale.final * progress;
			anim_obj[i].size = { s, s };
		}
		break;
	}
	case AnimationType::MoveTo:
	{
		for (auto i : anim.actors)
		{
			Vec3 dest = { anim.data.move_to.final_x, anim.data.move_to.final_y, anim_obj[i].position.z };
			anim_obj[i].position = prev_anim_obj[i].position * (1 - progress) + dest * progress;
		}
		break;
	}
	case AnimationType::Rotate:
		for (auto i : anim.actors)
		{
			anim_obj[i].rotation = prev_anim_obj[i].rotation + anim.data.rotate.angle * progress;
		}
		break;
	default:
		break;
	}
}

void Update(float dt)
{
	if (pause) return;

	time += dt;

	for (auto& anim : animations)
	{
		Animate(time, anim);
	}
}

void ResetAnimation()
{
	time = 0.0f;
	pause = true;
	anim_obj = initial_anim_obj;
	prev_anim_obj = initial_anim_obj;

	for (auto& anim : animations)
	{
		anim.started = false;
		anim.ended = false;
	}
}

void RenderImgui(float dt)
{
	ImGui::Begin("Animation Control");
	ImGui::Text("%.0f FPS", 1.0f/dt);
	ImGui::Text("Time: %.1f secs", time);
	ImGui::Checkbox("Paused", &pause);
	if (ImGui::Button("Reset"))
	{
		ResetAnimation();
	}
	ImGui::End();
}


void Render(float dt)
{
	ThunderLib::RendererAPI::SetClearColor(Color::Blue);
	ThunderLib::RendererAPI::Clear();

	// Start scene
	ThunderLib::OrthographicCamera camera(0.0f, 1.0f, 0.0f, 1.0f);
	ThunderLib::Renderer2D::StartScene(camera);

	for (auto& animatable : anim_obj)
	{
		if (animatable.hidden)
		{
			continue;
		}

		switch (animatable.type)
		{
		case AnimatableType::RECT:
			ThunderLib::Renderer2D::DrawQuad(animatable.position, animatable.rotation, animatable.size, animatable.color);
			break;
		case AnimatableType::CIRCLE:
			ThunderLib::Renderer2D::DrawCircle(animatable.position, animatable.size.x, animatable.color, 1.0f, 0.01f);
			break;
		default:
			break;
		}
	}

	ThunderLib::Renderer2D::EndScene();

	RenderImgui(dt);
}

void ProcessInput(f32 dt)
{
	if (input->GetIsKeyPress(ThunderLib::KeyCode::ESCAPE))
	{
		ThunderLib::App::Get().Close();
	}

	if (input->GetIsKeyPress(ThunderLib::KeyCode::P))
	{
		pause = !pause;
	}

	if (input->GetIsKeyPress(ThunderLib::KeyCode::R))
	{
		ResetAnimation();
	}
}

export int Main()
{
	ThunderLib::App app("Animation", 800, 800);
	ThunderLib::Renderer::Init();
	InitObjects();
	Script();

	input = ThunderLib::KeyInput::Create({ 
		ThunderLib::KeyCode::P,
		ThunderLib::KeyCode::ESCAPE,
		ThunderLib::KeyCode::R
	});

	app.AddCallback(ProcessInput);
	app.AddCallback(Update);
	app.AddCallback(Render);
	app.Run();
	return 0;
}
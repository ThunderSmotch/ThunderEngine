module;
#include "glm/glm.hpp"
export module Game.Fractal;

import std;

import ThunderLib.Base;
import ThunderLib.Shader;
import ThunderLib.App;
import ThunderLib.VertexArray;
import ThunderLib.KeyInput;
import ThunderLib.RendererAPI;
import ThunderLib.KeyCode;

ThunderLib::Ref<ThunderLib::VertexArray> quad;
ThunderLib::Ref<ThunderLib::Shader> shader;
ThunderLib::Ref<ThunderLib::KeyInput> input;

static const std::string fractal_vertex_source =
#include "FractalVertex.glsl"
;

static const std::string fractal_fragment_source =
#include "FractalFragment.glsl"
;

float zoom = 1.0f;
glm::vec2 center = { 0.0f, 0.0f };
float velocity = 0.01f;
bool quit = false;

void InitQuad()
{
	shader = ThunderLib::Shader::CreateFromString(fractal_vertex_source, fractal_fragment_source);
	shader->Bind();

	quad = ThunderLib::VertexArray::Create();

	auto buffer = ThunderLib::VertexBuffer::Create(4*2*4);
	buffer->SetLayout({ { "aPos", ThunderLib::ShaderDataType::Float2 } });

	glm::vec2 vertices[4] = { {-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 1.0f} };
	buffer->SetData(vertices, 2 * 4 * 4);
	quad->AddVertexBuffer(buffer);

	u32 indices[6] = { 0, 1, 2, 2, 3, 0 };
	auto index_buffer = ThunderLib::IndexBuffer::Create(indices, 6);
	quad->SetIndexBuffer(index_buffer);

	ThunderLib::RendererAPI::Init();

	shader->SetFloat2("viewportDims", { 1024, 768 });
}

void ProcessInput(float t)
{
	if (input->GetIsKeyDown(ThunderLib::KeyCode::Q) )
	{
		zoom *= 1.01f;
	}
	if (input->GetIsKeyDown(ThunderLib::KeyCode::E))
	{
		zoom *= 0.99f;
	}

	if (input->GetIsKeyDown(ThunderLib::KeyCode::W))
	{
		center.y += velocity/zoom;
	}
	if (input->GetIsKeyDown(ThunderLib::KeyCode::S))
	{
		center.y -= velocity/zoom;
	}

	if (input->GetIsKeyDown(ThunderLib::KeyCode::A))
	{
		center.x -= velocity/zoom;
	}
	if (input->GetIsKeyDown(ThunderLib::KeyCode::D))
	{
		center.x += velocity/zoom;
	}

	if (input->GetIsKeyPress(ThunderLib::KeyCode::R))
	{
		zoom = 1.0f;
		center = { 0.0f, 0.0f };
	}

	if (input->GetIsKeyPress(ThunderLib::KeyCode::ESCAPE))
	{
		ThunderLib::App::Get().Close();
	}

}

void Render(float t)
{
	shader->Bind();
	shader->SetFloat("zoom", zoom);
	shader->SetFloat2("center", center);
	ThunderLib::RendererAPI::DrawIndexed(quad, 6);
}

export int Main()
{

	auto app = ThunderLib::App("Fractal", 1024, 768);
	InitQuad();

	input = ThunderLib::KeyInput::Create({ ThunderLib::KeyCode::W, ThunderLib::KeyCode::S, ThunderLib::KeyCode::A, ThunderLib::KeyCode::D,
		ThunderLib::KeyCode::E, ThunderLib::KeyCode::Q, ThunderLib::KeyCode::R, ThunderLib::KeyCode::ESCAPE });

	app.AddCallback(ProcessInput);
	app.AddCallback(Render);
	app.Run();
	return 0;
}
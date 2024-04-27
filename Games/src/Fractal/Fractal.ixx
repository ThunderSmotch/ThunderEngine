module;
#include "glm/glm.hpp"
export module Game.Fractal;

import std;

import ThunderEngine.Base;
import ThunderEngine.Shader;
import ThunderEngine.App;
import ThunderEngine.VertexArray;
import ThunderEngine.KeyInput;
import ThunderEngine.RendererAPI;
import ThunderEngine.KeyCode;

ThunderEngine::Ref<ThunderEngine::VertexArray> quad;
ThunderEngine::Ref<ThunderEngine::Shader> shader;
ThunderEngine::Ref<ThunderEngine::KeyInput> input;

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
	shader = ThunderEngine::Shader::CreateFromString(fractal_vertex_source, fractal_fragment_source);
	shader->Bind();

	quad = ThunderEngine::VertexArray::Create();

	auto buffer = ThunderEngine::VertexBuffer::Create(4*2*4);
	buffer->SetLayout({ { "aPos", ThunderEngine::ShaderDataType::Float2 } });

	glm::vec2 vertices[4] = { {-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 1.0f} };
	buffer->SetData(vertices, 2 * 4 * 4);
	quad->AddVertexBuffer(buffer);

	uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
	auto index_buffer = ThunderEngine::IndexBuffer::Create(indices, 6);
	quad->SetIndexBuffer(index_buffer);

	ThunderEngine::RendererAPI::Init();

	shader->SetFloat2("viewportDims", { 1024, 768 });
}

void ProcessInput(float t)
{
	if (input->GetIsKeyDown(ThunderEngine::KeyCode::Q) )
	{
		zoom *= 1.01f;
	}
	if (input->GetIsKeyDown(ThunderEngine::KeyCode::E))
	{
		zoom *= 0.99f;
	}

	if (input->GetIsKeyDown(ThunderEngine::KeyCode::W))
	{
		center.y += velocity/zoom;
	}
	if (input->GetIsKeyDown(ThunderEngine::KeyCode::S))
	{
		center.y -= velocity/zoom;
	}

	if (input->GetIsKeyDown(ThunderEngine::KeyCode::A))
	{
		center.x -= velocity/zoom;
	}
	if (input->GetIsKeyDown(ThunderEngine::KeyCode::D))
	{
		center.x += velocity/zoom;
	}

	if (input->GetIsKeyPress(ThunderEngine::KeyCode::R))
	{
		zoom = 1.0f;
		center = { 0.0f, 0.0f };
	}

	if (input->GetIsKeyPress(ThunderEngine::KeyCode::ESCAPE))
	{
		ThunderEngine::App::Get().Close();
	}

}

void Render(float t)
{
	shader->Bind();
	shader->SetFloat("zoom", zoom);
	shader->SetFloat2("center", center);
	ThunderEngine::RendererAPI::DrawIndexed(quad, 6);
}

export int Main()
{

	auto app = ThunderEngine::App("Fractal", 1024, 768);
	InitQuad();

	input = ThunderEngine::KeyInput::Create({ ThunderEngine::KeyCode::W, ThunderEngine::KeyCode::S, ThunderEngine::KeyCode::A, ThunderEngine::KeyCode::D,
		ThunderEngine::KeyCode::E, ThunderEngine::KeyCode::Q, ThunderEngine::KeyCode::R, ThunderEngine::KeyCode::ESCAPE });

	app.AddCallback(ProcessInput);
	app.AddCallback(Render);
	app.Run();
	return 0;
}
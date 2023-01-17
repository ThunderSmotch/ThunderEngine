#pragma once

#include <glm/glm.hpp>

#include "Renderer/Texture.h"
#include "Renderer/Camera.h"

namespace ThunderEngine
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void StartScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawTriangle();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint_color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint_color);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tint_color);

		static void DrawTriangle(const glm::vec2& position);


	private:
		static void StartBatch();
		static void NextBatch();

		static void InitTriangle();
		static void InitQuad();
		static void InitWhiteTexture();
	};
}
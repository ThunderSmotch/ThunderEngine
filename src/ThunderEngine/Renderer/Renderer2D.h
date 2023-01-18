#pragma once

#include <glm/glm.hpp>

#include "Texture.h"
#include "Camera.h"

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

		static void DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color);

		static void DrawTriangle();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint_color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint_color);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tint_color);

		static void DrawTriangle(const glm::vec2& position);

		static void SetLineWidth(float width);
		static float GetLineWidth();

	private:
		static void StartBatch();
		static void NextBatch();

		static void InitLine();
		static void InitTriangle();
		static void InitQuad();
		static void InitWhiteTexture();
	};
}
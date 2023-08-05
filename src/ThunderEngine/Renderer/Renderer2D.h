#pragma once

#include <glm/glm.hpp>

#include "Texture.h"
#include "Camera.h"

namespace ThunderEngine
{
	class Renderer2D
	{
	public:
		/// <summary>
		/// Initializes the 2D rendering system (automatically called by Renderer)
		/// </summary>
		static void Init();

		/// <summary>
		/// Shutdowns the 2D rendering system (automatically called by Renderer)
		/// </summary>
		static void Shutdown();

		/// <summary>
		/// Starts a 2D scene rendering from a orthographic camera
		/// </summary>
		/// <param name="camera">The orthographic scene</param>
		static void StartScene(const OrthographicCamera& camera);

		/// <summary>
		/// Ends the current scene, letting the GPU know it can draw all the queued commands.
		/// </summary>
		static void EndScene();

		/// <summary>
		/// Flushes the current batch to the GPU, letting it draw the batch.
		/// </summary>
		static void Flush();

		/// <summary>
		/// Adds a DrawLine command to the rendering queue
		/// </summary>
		/// <param name="start">Start position</param>
		/// <param name="end">End position</param>
		/// <param name="color">Color of the line</param>
		static void DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color);

		/// <summary>
		/// Draws a quad at the specified position with the provided scaling and color
		/// </summary>
		/// <param name="position">The position of the center of the quad</param>
		/// <param name="size">The horizontal and vertical scales of the quad</param>
		/// <param name="color">The color of the quad</param>
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		
		/// <summary>
		/// Draws a quad at the specified position with the provided scaling and color
		/// </summary>
		/// <param name="position">The position of the center of the quad</param>
		/// <param name="size">The horizontal and vertical scales of the quad</param>
		/// <param name="color">The color of the quad</param>
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		
		/// <summary>
		/// Draws a quad with the provided transform and color
		/// </summary>
		/// <param name="transform">The world transform of the quad</param>
		/// <param name="color">The color of the quad</param>
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

		/// <summary>
		/// Draws a texture at the specified position with the provided scaling and color tint
		/// </summary>
		/// <param name="position">The position of the center of the texture</param>
		/// <param name="size">The horizontal and vertical scales of the quad</param>
		/// <param name="texture">The texture to draw</param>
		/// <param name="tint_color">Color tint of the texture (white is the normal sprite texture)</param>
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint_color);

		/// <summary>
		/// Draws a texture at the specified position with the provided scaling and color tint
		/// </summary>
		/// <param name="position">The position of the center of the texture</param>
		/// <param name="size">The horizontal and vertical scales of the quad</param>
		/// <param name="texture">The texture to draw</param>
		/// <param name="tint_color">Color tint of the texture (white is the normal sprite texture)</param>
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint_color);

		/// <summary>
		/// Draws a quad with the provided transform and color tint
		/// </summary>
		/// <param name="transform">The world transform of the quad</param>
		/// <param name="texture">The texture to draw</param>
		/// <param name="tint_color">Color tint of the texture (white is the normal sprite texture)</param>
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tint_color);

		/// <summary>
		/// Deprecated
		/// </summary>
		static void DrawTriangle();
		/// <summary>
		/// Deprecated
		/// </summary>
		/// <param name="position"></param>
		static void DrawTriangle(const glm::vec2& position);

		/// <summary>
		/// Draws a colored circle at the specified transform with the given outline parameters
		/// </summary>
		/// <param name="transform">The world transform of the circle</param>
		/// <param name="color">The inner color</param>
		/// <param name="border_color">The border color</param>
		/// <param name="border_thickness">Percentage of radius that the border uses</param>
		/// <param name="thickness">Percentage of circle to show, from outside inwards.</param>
		/// <param name="fade">Parameter controlling the antialiasing of the circle</param>
		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, const glm::vec4& border_color,
			float border_thickness = 0.05f, float thickness = 1.0f, float fade = 0.005f);

		/// <summary>
		/// Draw a borderless circle with the given color
		/// </summary>
		/// <param name="transform">The world transform of the circle</param>
		/// <param name="color">The inner color</param>
		/// <param name="thickness">Percentage of circle to show, from outside inwards.</param>
		/// <param name="fade">Parameter controlling the antialiasing of the circle</param>
		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f);

		/// <summary>
		/// Draws a colored circle at the specified position with the given size and outline parameters
		/// </summary>
		/// <param name="position">Position of the center of the circle</param>
		/// <param name="size">Size of the circle</param>
		/// <param name="color">The inner color</param>
		/// <param name="border_color">The border color</param>
		/// <param name="border_thickness">Percentage of radius that the border uses</param>
		/// <param name="thickness">Percentage of circle to show, from outside inwards.</param>
		/// <param name="fade">Parameter controlling the antialiasing of the circle</param>
		static void DrawCircle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const glm::vec4 border_color,
			float border_thickness, float thickness = 1.0f, float fade = 0.005f);

		/// <summary>
		/// Draws a colored borderless circle at the specified position with the given size
		/// </summary>
		/// <param name="position">Position of the center of the circle</param>
		/// <param name="size">Size of the circle</param>
		/// <param name="color">The inner color</param>
		/// <param name="thickness">Percentage of circle to show, from outside inwards.</param>
		/// <param name="fade">Parameter controlling the antialiasing of the circle</param>
		static void DrawCircle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f);

		/// <summary>
		/// Sets the line width
		/// </summary>
		/// <param name="width">Line width in pixels</param>
		static void SetLineWidth(float width);
		/// <summary>
		/// Gets the current line width
		/// </summary>
		/// <returns>Returns line width in pixels</returns>
		static float GetLineWidth();

	private:
		static void StartBatch();
		static void NextBatch();

		static void InitLine();
		static void InitCircle();
		static void InitTriangle();
		static void InitQuad();
		static void InitWhiteTexture();
	};
}
module;
#include <glm/glm.hpp>
export module ThunderLib.Platform.OpenGL.RendererAPI;

import ThunderLib.Base;
import ThunderLib.VertexArray;

export namespace ThunderLib
{
	class OpenGLRendererAPI
	{
	public:
		static void Init();

		static void DrawIndexed(const Ref<VertexArray>& vertex_array, u32 index_count = 0);
		static void DrawLines(const Ref<VertexArray>& vertex_array, u32 vertex_count);

		static void SetLineWidth(float width);

		static void SetWireframeMode(bool enabled);

		static void SetViewport(u32 x, u32 y, u32 width, u32 height);

		static void Clear();
		static void SetClearColor(const glm::vec4& color);
	};
	export using RendererAPI = OpenGLRendererAPI;
}
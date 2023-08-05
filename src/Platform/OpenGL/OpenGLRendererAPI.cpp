#include "tepch.h"
#include "OpenGLRendererAPI.h"

#include <glad/gl.h>

namespace ThunderEngine
{
	void GLAPIENTRY ErrorOccurredGL(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam)
	{
		if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		{
			return;
		}

		printf("Message from OpenGL:\nSource: 0x%x\nType: 0x%x\n"
			"Id: 0x%x\nSeverity: 0x%x\n", source, type, id, severity);
		printf("%s\n", message);
		exit(-1); // shut down the program gracefully (it does cleanup stuff too)
		// without exit(), OpenGL will constantly print the error message... make sure to kill your program.
	}

	void OpenGLRendererAPI::Init()
	{
		//Setup OpenGL debug output
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(ErrorOccurredGL, NULL);

		// Enabled Blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Enable depth testing
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_LINE_SMOOTH);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count)
	{
		vertex_array->Bind();
		uint32_t count = index_count ? index_count : vertex_array->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertex_array, uint32_t vertex_count)
	{
		vertex_array->Bind();
		glDrawArrays(GL_LINES, 0, vertex_count);
	}

	void OpenGLRendererAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}

	void OpenGLRendererAPI::SetWireframeMode(bool enabled)
	{
		if (enabled)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}


}

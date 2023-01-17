#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace ThunderEngine
{
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		return CreateRef<OpenGLShader>(filepath);
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertex_path, const std::string& fragment_path)
	{
		return CreateRef<OpenGLShader>(name, vertex_path, fragment_path);
	}

}


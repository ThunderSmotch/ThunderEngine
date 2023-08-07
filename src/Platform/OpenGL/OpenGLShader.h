#pragma once

#include <unordered_map>

#include "ThunderEngine/Renderer/Shader_Base.h"

namespace ThunderEngine
{
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class OpenGLShader : public Shader_Base<OpenGLShader> {
	private:
		uint32_t renderer_id_;
		std::unordered_map<std::string, int32_t> uniform_location_cache_;
		std::string name_;
	public:
		// Compiles a shader from specified path that uses the #shader directive to separate shader types
		OpenGLShader(const std::string& shader_path);
		OpenGLShader(const std::string& name, const std::string& vertex_path, const std::string& fragment_path);
		OpenGLShader(const std::string& vertex_code, const std::string& fragment_code);
		~OpenGLShader();

		static Ref<OpenGLShader> Create(const std::string& filepath)
		{
			return CreateRef<OpenGLShader>(filepath);
		}

		// BUG Not checking if files exist!!!
		static Ref<OpenGLShader> Create(const std::string& name, const std::string& vertex_path, const std::string& fragment_path)
		{
			return CreateRef<OpenGLShader>(name, vertex_path, fragment_path);
		}

		static Ref<OpenGLShader> CreateFromString(const std::string& vertex_code, const std::string& fragment_code)
		{
			return CreateRef<OpenGLShader>(vertex_code, fragment_code);
		}

		void Bind() const;   // Activates the shader program
		void Unbind() const; // Deactivates the shader program

		const std::string& GetName() const { return name_; }

		void SetBool(const std::string& name, uint8_t value);
		void SetInt(const std::string& name, int value);
		void SetIntArray(const std::string& name, int* values, uint32_t count);
		void SetFloat(const std::string& name, float value);
		void SetFloat2(const std::string& name, const glm::vec2& value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);
		void SetMat4(const std::string& name, const glm::mat4& matrix);

	private:
		int GetUniformLocation(const std::string& name);

		ShaderProgramSource ParseShader(const std::string& filepath);
		uint32_t CompileShader(uint32_t type, const std::string& source);
		uint32_t CreateShader(const std::string& vertex_source, const std::string& fragment_source);
	};
}
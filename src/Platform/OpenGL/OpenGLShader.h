#pragma once

#include <unordered_map>

#include "Renderer/Shader.h"

namespace ThunderEngine
{
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class OpenGLShader : public Shader {
	private:
		uint32_t renderer_id_;
		std::unordered_map<std::string, int32_t> uniform_location_cache_;
		std::string name_;
	public:
		// Compiles a shader from specified path that uses the #shader directive to separate shader types
		OpenGLShader(const std::string& shader_path);
		OpenGLShader(const std::string& name, const std::string& vertex_path, const std::string& fragment_path);
		virtual ~OpenGLShader();

		virtual void Bind() const override;   // Activates the shader program
		virtual void Unbind() const override; // Deactivates the shader program

		virtual const std::string& GetName() const override { return name_; }

		virtual void SetBool(const std::string& name, uint8_t value) override;
		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) override;

	private:
		int GetUniformLocation(const std::string& name);

		ShaderProgramSource ParseShader(const std::string& filepath);
		uint32_t CompileShader(uint32_t type, const std::string& source);
		uint32_t CreateShader(const std::string& vertex_source, const std::string& fragment_source);
	};
}
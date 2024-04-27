module;
#include <glm/glm.hpp>
export module ThunderLib.Platform.OpenGL.Shader;

import std;
import ThunderLib.Base;

export namespace ThunderLib
{
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class OpenGLShader {
	private:
		u32 renderer_id_;
		std::unordered_map<std::string, std::int32_t> uniform_location_cache_;
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
		void SetIntArray(const std::string& name, int* values, u32 count);
		void SetFloat(const std::string& name, float value);
		void SetFloat2(const std::string& name, const glm::vec2& value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);
		void SetMat4(const std::string& name, const glm::mat4& matrix);

	private:
		int GetUniformLocation(const std::string& name);

		ShaderProgramSource ParseShader(const std::string& filepath);
		u32 CompileShader(u32 type, const std::string& source);
		u32 CreateShader(const std::string& vertex_source, const std::string& fragment_source);
	};
	export using Shader = OpenGLShader;
}
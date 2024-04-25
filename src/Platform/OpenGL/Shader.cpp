module;
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
module ThunderEngine.Platform.OpenGL.Shader;

import ThunderEngine.Logger;

namespace ThunderEngine
{
	OpenGLShader::OpenGLShader(const std::string& shader_path)
		: renderer_id_(0), name_("")
	{
		ShaderProgramSource source = ParseShader(shader_path);
		renderer_id_ = CreateShader(source.VertexSource, source.FragmentSource);

		// Extract name from filepath
		auto lastSlash = shader_path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = shader_path.rfind('.');
		auto count = lastDot == std::string::npos ? shader_path.size() - lastSlash : lastDot - lastSlash;
		name_ = shader_path.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertex_path, const std::string& fragment_path)
		: renderer_id_(0), name_("")
	{
		//Grab vertex and fragment sources from paths
		std::ifstream stream(vertex_path);
		std::stringstream v_buffer, f_buffer;
		v_buffer << stream.rdbuf();
		stream = std::ifstream(fragment_path);
		f_buffer << stream.rdbuf();

		renderer_id_ = CreateShader(v_buffer.str().c_str(), f_buffer.str().c_str());
		name_ = name;
	}

	OpenGLShader::OpenGLShader(const std::string& vertex_code, const std::string& fragment_code)
		: renderer_id_(0), name_("")
	{
		renderer_id_ = CreateShader(vertex_code, fragment_code);
		name_ = "InternalShader_" + std::to_string(renderer_id_);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(renderer_id_);
	}

	ShaderProgramSource OpenGLShader::ParseShader(const std::string& filepath)
	{
		std::ifstream stream(filepath);

		enum class OpenGLShaderType
		{
			NONE = -1,
			VERTEX = 0,
			FRAGMENT = 1,
		};

		std::string line;
		std::stringstream shadersCode[2];

		OpenGLShaderType type = OpenGLShaderType::NONE;

		while (getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{
					type = OpenGLShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos)
				{
					type = OpenGLShaderType::FRAGMENT;
				}
			}
			else
			{
				shadersCode[(int)type] << line << "\n";
			}
		}

		return { shadersCode[0].str(), shadersCode[1].str() };
	}

	uint32_t OpenGLShader::CompileShader(uint32_t type, const std::string& source)
	{
		uint32_t id = glCreateShader(type);
		const char* src = source.c_str();

		glShaderSource(id, 1, &src, NULL);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*) malloc(sizeof(char)*length);

			glGetShaderInfoLog(id, length, NULL, message);

			Logger::Error("Failed to compile % shader, see attached log: %", ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment"), message);

			if (message != 0)
			{
				memset(message, 0, sizeof(char) * length);
			}

			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	uint32_t OpenGLShader::CreateShader(const std::string& vertex_source, const std::string& fragment_source)
	{
		uint32_t vertex = CompileShader(GL_VERTEX_SHADER, vertex_source);
		uint32_t fragment = CompileShader(GL_FRAGMENT_SHADER, fragment_source);

		uint32_t program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);

		// Check if program linked correctly
		int result;
		glGetProgramiv(program, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)malloc(sizeof(char) * length);
			glGetProgramInfoLog(program, length, NULL, message);
			Logger::Error("Failed to compile program, see attached log: %", message);

			if (message != 0)
			{
				memset(message, 0, sizeof(char) * length);
			}

			glDeleteProgram(program);
		}

		// Delete shaders; they’re linked into our program and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);

		return program;
	}


	void OpenGLShader::Bind() const
	{
		glUseProgram(renderer_id_);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetBool(const std::string& name, uint8_t value)
	{
		glUniform1i(GetUniformLocation(name), (int)value);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		glUniform1iv(GetUniformLocation(name), count, values);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(GetUniformLocation(name), value.x, value.y);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) {
		glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		if (uniform_location_cache_.find(name) != uniform_location_cache_.end())
		{
			return uniform_location_cache_[name];
		}

		// If it is not in the cache then find location and store it
		int location = glGetUniformLocation(renderer_id_, name.c_str());
		if (location == -1)
		{
			Logger::Warning("Uniform % was not found!", name);
		}
		uniform_location_cache_[name] = location;
		return location;
	}
}
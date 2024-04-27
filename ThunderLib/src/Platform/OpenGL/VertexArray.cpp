module;
#include <glad/gl.h>
#include <cassert>
module ThunderLib.Platform.OpenGL.VertexArray;

import std;
import ThunderLib.BufferDefinitions;
import ThunderLib.Logger;

namespace ThunderLib
{
	static GLenum ShaderDataTypeToOpenGLType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
			return GL_FLOAT;
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
			return GL_INT;
		case ShaderDataType::Bool:
			return GL_BOOL;
		}

		Logger::Warning("Unknown ShaderDataType! Can't convert to OpenGL type!");
		return 0;
	}

	static GLint ShaderDataTypeToComponentNumber(ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Int:
		case ShaderDataType::Bool:
			return 1;
		case ShaderDataType::Float2:
		case ShaderDataType::Int2:
			return 2;
		case ShaderDataType::Float3:
		case ShaderDataType::Int3:
		case ShaderDataType::Mat3:
			return 3;
		case ShaderDataType::Float4:
		case ShaderDataType::Int4:
		case ShaderDataType::Mat4:
			return 4;
		}
		Logger::Warning("Unknown ShaderDataType! Can't get number of components!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
		:renderer_id_(0), index_buffer_(nullptr)
	{
		glCreateVertexArrays(1, &renderer_id_);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &renderer_id_);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(renderer_id_);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer)
	{
		assert(vertex_buffer->GetLayout().GetElements().size() > 0);

		glBindVertexArray(renderer_id_);
		vertex_buffer->Bind();

		const BufferLayout& layout = vertex_buffer->GetLayout();

		for (const BufferElement& element : layout.GetElements())
		{
			switch (element.type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(vertex_buffer_index_);

				glVertexAttribPointer(vertex_buffer_index_,
					ShaderDataTypeToComponentNumber(element.type),
					ShaderDataTypeToOpenGLType(element.type),
					element.normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.offset
				);

				vertex_buffer_index_++;
				break;
			}

			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(vertex_buffer_index_);
				glVertexAttribIPointer(vertex_buffer_index_,
					ShaderDataTypeToComponentNumber(element.type),
					ShaderDataTypeToOpenGLType(element.type),
					layout.GetStride(),
					(const void*)element.offset
				);
				vertex_buffer_index_++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = ShaderDataTypeToComponentNumber(element.type);

				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(vertex_buffer_index_);
					glVertexAttribPointer(vertex_buffer_index_,
						count,
						ShaderDataTypeToOpenGLType(element.type),
						element.normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.offset + sizeof(float) * count * i));
					glVertexAttribDivisor(vertex_buffer_index_, 1);
					vertex_buffer_index_++;
				}
				break;
			}
			default:
				Logger::Warning("Unknown ShaderDataType! Can't add to Buffer!");
			}
		}

		vertex_buffers_.push_back(vertex_buffer);

	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& index_buffer)
	{
		glBindVertexArray(renderer_id_);
		index_buffer->Bind();
		index_buffer_ = index_buffer;
	}
}
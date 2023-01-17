#pragma once

#include "ThunderEngine.h"

#include <iostream>

namespace ThunderEngine
{

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	// Returns the size in bytes of the ShaderDataType
	static uint32_t GetShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float: return 4;
		case ShaderDataType::Float2: return 4 * 2;
		case ShaderDataType::Float3: return 4 * 3;
		case ShaderDataType::Float4: return 4 * 4;
		case ShaderDataType::Mat3: return 4 * 3 * 3;
		case ShaderDataType::Mat4: return 4 * 4 * 4;
		case ShaderDataType::Int: return 4;
		case ShaderDataType::Int2: return 4 * 2;
		case ShaderDataType::Int3: return 4 * 3;
		case ShaderDataType::Int4: return 4 * 4;
		case ShaderDataType::Bool: return 1;
		}

		TE_WARN("Unknown ShaderDataType! Cannot get size.");
		return 0;
	}


	// Represents an element of a buffer's vertex
	struct BufferElement
	{
		std::string name;
		ShaderDataType type;
		uint32_t size;
		size_t offset;
		bool normalized;

		BufferElement() = default;

		BufferElement(std::string name, ShaderDataType type, bool normalized = false)
			: name(name), type(type), size(GetShaderDataTypeSize(type)), offset(0), normalized(normalized)
		{}
	};

	class BufferLayout
	{
	private:
		std::vector<BufferElement> elements_;
		uint32_t stride_ = 0;

	public:
		BufferLayout() {}

		BufferLayout(std::initializer_list<BufferElement> elements)
			:elements_(elements)
		{
			CalculateOffsetsAndStride();
		}

		uint32_t GetStride() const { return stride_; }
		const std::vector<BufferElement>& GetElements() const { return elements_; }

	private:
		void CalculateOffsetsAndStride();
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};


	// MAYBE support for other indices types (uint8_t) for smaller things.
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};
}

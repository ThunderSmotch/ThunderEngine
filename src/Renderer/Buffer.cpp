#include "Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace ThunderEngine
{

	void BufferLayout::CalculateOffsetsAndStride()
	{
		size_t offset = 0;
		stride_ = 0;

		for (BufferElement& element : elements_) {
			element.offset = offset;
			offset += element.size;
			stride_ += element.size;
		}
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		return CreateRef<OpenGLVertexBuffer>(size);
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		return CreateRef<OpenGLVertexBuffer>(vertices, size);
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		return CreateRef<OpenGLIndexBuffer>(indices, count);
	}
}

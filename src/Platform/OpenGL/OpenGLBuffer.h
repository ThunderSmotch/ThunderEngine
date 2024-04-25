#pragma once

#include "ThunderEngine/Renderer/Buffer_Base.h"

import ThunderEngine.Base;

namespace ThunderEngine
{
	class OpenGLVertexBuffer : public VertexBuffer_Base<OpenGLVertexBuffer>
	{
	private:
		uint32_t renderer_id_;
		BufferLayout layout_;

	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		~OpenGLVertexBuffer();

		static Ref<OpenGLVertexBuffer> Create(uint32_t size) 
		{ 
			return CreateRef<OpenGLVertexBuffer>(size);
		}
		static Ref<OpenGLVertexBuffer> Create(float* vertices, uint32_t size)
		{
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, uint32_t size);

		const BufferLayout& GetLayout() const { return layout_; }
		void SetLayout(const BufferLayout& layout) { layout_ = layout; }

	};

	class OpenGLIndexBuffer : public VertexBuffer_Base<OpenGLIndexBuffer>
	{
	private:
		uint32_t renderer_id_;
		uint32_t count_;

	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();

		static Ref<OpenGLIndexBuffer> Create(uint32_t* indices, uint32_t count)
		{
			return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		void Bind() const;
		void Unbind() const;

		uint32_t GetCount() const { return count_; }
	};
}

#pragma once

#include "Renderer/Buffer.h"

namespace ThunderEngine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	private:
		uint32_t renderer_id_;
		BufferLayout layout_;

	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const BufferLayout& GetLayout() const override { return layout_; }
		virtual void SetLayout(const BufferLayout& layout) override { layout_ = layout; }

	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	private:
		uint32_t renderer_id_;
		uint32_t count_;

	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return count_; }
	};
}

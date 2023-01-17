#pragma once

#include "Renderer/VertexArray.h"

namespace ThunderEngine
{
	class OpenGLVertexArray : public VertexArray
	{
	private:
		uint32_t renderer_id_;
		uint32_t vertex_buffer_index_ = 0;
		std::vector<Ref<VertexBuffer>> vertex_buffers_;
		Ref<IndexBuffer> index_buffer_;

	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return vertex_buffers_; }
		virtual const Ref<IndexBuffer> GetIndexBuffer() const { return index_buffer_; }

	};
}


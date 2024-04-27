export module ThunderLib.Platform.OpenGL.VertexArray;

import std;
import ThunderLib.Buffer;
import ThunderLib.Base;

export namespace ThunderLib
{
	class OpenGLVertexArray
	{
	private:
		uint32_t renderer_id_;
		uint32_t vertex_buffer_index_ = 0;
		std::vector<Ref<VertexBuffer>> vertex_buffers_;
		Ref<IndexBuffer> index_buffer_;

	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();
		static Ref<OpenGLVertexArray> Create() { return CreateRef<OpenGLVertexArray>(); }

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

		const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return vertex_buffers_; }
		const Ref<IndexBuffer> GetIndexBuffer() const { return index_buffer_; }
	};

	export using VertexArray = OpenGLVertexArray;
}


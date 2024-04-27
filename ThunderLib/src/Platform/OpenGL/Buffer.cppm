export module ThunderLib.Platform.OpenGL.Buffer;

import std;
import ThunderLib.Base;
import ThunderLib.BufferDefinitions;

export namespace ThunderLib
{
	class OpenGLVertexBuffer
	{
	private:
		u32 renderer_id_;
		BufferLayout layout_;

	public:
		OpenGLVertexBuffer(u32 size);
		OpenGLVertexBuffer(float* vertices, u32 size);
		~OpenGLVertexBuffer();

		static Ref<OpenGLVertexBuffer> Create(u32 size) 
		{ 
			return CreateRef<OpenGLVertexBuffer>(size);
		}
		static Ref<OpenGLVertexBuffer> Create(float* vertices, u32 size)
		{
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, u32 size);

		const BufferLayout& GetLayout() const { return layout_; }
		void SetLayout(const BufferLayout& layout) { layout_ = layout; }

	};

	class OpenGLIndexBuffer
	{
	private:
		u32 renderer_id_;
		u32 count_;

	public:
		OpenGLIndexBuffer(u32* indices, u32 count);
		~OpenGLIndexBuffer();

		static Ref<OpenGLIndexBuffer> Create(u32* indices, u32 count)
		{
			return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		void Bind() const;
		void Unbind() const;

		u32 GetCount() const { return count_; }
	};

	export using VertexBuffer = OpenGLVertexBuffer;
	export using IndexBuffer = OpenGLIndexBuffer;
}

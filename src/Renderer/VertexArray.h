#pragma once
#include "ThunderEngine.h"
#include "Buffer.h"

namespace ThunderEngine
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer> GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};

	/*template <typename Derived>
	struct VertexArrayBase
	{
		void Bind() { return static_cast<Derived*>(this)->Bind(); }
		void Unbind() { return static_cast<Derived*>(this)->Bind(); }

		void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) 
		{ return static_cast<Derived*>(this)->AddVertexBuffer(vertexBuffer); }

		void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) 
		{ return static_cast<Derived*>(this)->SetIndexBuffer(indexBuffer); }

		const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const 
		{ return static_cast<Derived*>(this)->GetVertexBuffers(); }

		const Ref<IndexBuffer> GetIndexBuffer() const 
		{ return static_cast<Derived*>(this)->GetIndexBuffer(); }

		static Ref<Derived> Create()
		{ return static_cast<Derived*>(this)->Create(); }
	};

	struct Derived : VertexArrayBase<Derived>
	{
		std::string ToString() { return "This is Derived."; }
	};

	typedef Derived VertexArrayImpl;*/
}
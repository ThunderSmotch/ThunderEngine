#pragma once

#include "ThunderEngine/Core/Base.h"
#include "Buffer.h"

import ThunderEngine.Base;

namespace ThunderEngine {
	template <typename Derived>
	struct VertexArray_Base
	{
		// Needed to ensure constructor implements this specification!
		static Ref<Derived> Create() { return Derived::Create(); }

		void Bind() { return TE_DERIVED(Bind); }
		void Unbind() { return TE_DERIVED(Bind); }

		void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) { TE_DERIVED(AddVertexBuffer, vertexBuffer); }
		void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) { TE_DERIVED(SetIndexBuffer, indexBuffer); }

		const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return TE_DERIVED(GetVertexBuffers); }
		const Ref<IndexBuffer> GetIndexBuffer() const { return TE_DERIVED(GetIndexBuffer); }
	};
}
#pragma once

#include "ThunderEngine.h"

namespace ThunderEngine
{
	template <typename Derived>
	struct GraphicsContext_Base 
	{
		static Ref<Derived> Create(void* window) { return Derived::Create(window); }
		
		void Init() { TE_DERIVED(Init); }
		void SwapBuffers() { TE_DERIVED(SwapBuffers); }
	};
}
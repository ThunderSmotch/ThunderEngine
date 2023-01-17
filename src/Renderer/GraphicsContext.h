#pragma once

#include "ThunderEngine.h"

namespace ThunderEngine
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static Ref<GraphicsContext> Create(void* window);
	private:

	};
}

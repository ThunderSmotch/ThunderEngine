#pragma once

#include "ThunderEngine/Core/Base.h"

#include <string>

import ThunderEngine.Base;

namespace ThunderEngine
{
	template <typename Derived>
	struct Texture_Base
	{
		uint32_t GetWidth() const { return TE_DERIVED(GetWidth); }
		uint32_t GetHeight() const { return TE_DERIVED(GetHeight); }
		uint32_t GetRendererID() const { return TE_DERIVED(GetRendererID); }

		const std::string& GetPath() const { return TE_DERIVED(GetPath); }

		void SetData(void* data, uint32_t size) { TE_DERIVED(SetData, data, size); }

		void Bind(uint32_t slot = 0) const { TE_DERIVED(Bind, slot); }

		bool IsLoaded() const { return TE_DERIVED(IsLoaded); }

		bool operator==(const Derived& other) const { return TE_DERIVED(operator== ,other); }

		static Ref<Derived> Create(uint32_t width, uint32_t height, uint32_t number_channels = 4)
		{
			return Derived::Create(width, height, number_channels);
		}

		static Ref<Derived> Create(const std::string& path)
		{
			return Derived::Create(path);
		}
	};
}
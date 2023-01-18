#pragma once

#include <memory>

#define TE_DERIVED(func, ...) static_cast<Derived*>(this)->func(__VA_ARGS__)

namespace ThunderEngine
{
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using UniqueRef = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr UniqueRef<T> CreateUniqueRef(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}

// Same includes as tepch.h
#include "ThunderEngine/Utils/Logger.h"
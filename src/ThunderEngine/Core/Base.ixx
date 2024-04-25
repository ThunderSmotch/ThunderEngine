export module ThunderEngine.Base;

import std;

export {
	using byte = unsigned char;
	using u32 = std::uint32_t;
	using uint32_t = std::uint32_t; // TODO Remove

	using string = std::string;
}

export namespace ThunderEngine {
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
};
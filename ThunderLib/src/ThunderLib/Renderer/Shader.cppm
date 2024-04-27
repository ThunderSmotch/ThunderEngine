module;
#include <glm/glm.hpp>
export module ThunderLib.Shader;

#if 1
export import ThunderLib.Platform.OpenGL.Shader;
#elif
// Other implementation
#endif

import std;
import ThunderLib.Base;

using namespace ThunderLib;
template <typename T>
concept is_shader = requires(T t, T const tc, const std::string& s, std::uint8_t c, int i, u32 u, int* v, float f,
	const glm::vec2& v2, const glm::vec3& v3, const glm::vec4& v4, const glm::mat4& m4)
{
	{ T::Create(s) } -> std::same_as<Ref<T>>;
	{ T::CreateFromString(s, s) } -> std::same_as<Ref<T>>;
	{ T::Create(s, s, s) } -> std::same_as<Ref<T>>;

	{ tc.Bind() } -> std::same_as<void>;
	{ tc.Unbind() } -> std::same_as<void>;

	{ t.SetBool(s, c) } -> std::same_as<void>;
	{ t.SetInt(s, i) } -> std::same_as<void>;
	{ t.SetIntArray(s, v, u) } -> std::same_as<void>;
	
	{ t.SetFloat(s, f) } -> std::same_as<void>;
	{ t.SetFloat2(s, v2) } -> std::same_as<void>;
	{ t.SetFloat3(s, v3) } -> std::same_as<void>;
	{ t.SetFloat4(s, v4) } -> std::same_as<void>;
	{ t.SetMat4(s, m4) } -> std::same_as<void>;
	
	{ tc.GetName() } -> std::same_as<const std::string&>;
};
static_assert(is_shader<Shader>);
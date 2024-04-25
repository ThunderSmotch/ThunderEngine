#pragma once

#include "ThunderEngine/Core/Base.h"

#include <glm/glm.hpp>
#include <string>

import ThunderEngine.Base;

namespace ThunderEngine
{
	template <typename Derived>
	struct Shader_Base {
		void Bind() const { TE_DERIVED(Bind); }
		void Unbind() const { TE_DERIVED(Unbind); }

		void SetBool(const std::string& name, uint8_t value) { TE_DERIVED(SetBool, name, value); }
		void SetInt(const std::string& name, int value) { TE_DERIVED(SetInt, name, value); }
		void SetIntArray(const std::string& name, int* values, uint32_t count) { TE_DERIVED(SetIntArray, name, values, count); }
		
		void SetFloat(const std::string& name, float value) { TE_DERIVED(SetFloat, name, value); }
		void SetFloat2(const std::string& name, const glm::vec2& value) { TE_DERIVED(SetFloat2, name, value); }
		void SetFloat3(const std::string& name, const glm::vec3& value) { TE_DERIVED(SetFloat3, name, value); }
		void SetFloat4(const std::string& name, const glm::vec4& value) { TE_DERIVED(SetFloat4, name, value); }
		void SetMat4(const std::string& name, const glm::mat4& matrix) { TE_DERIVED(SetMat4, name, matrix); }

		const std::string& GetName() const { return TE_DERIVED(GetName); }

		static Ref<Derived> Create(const std::string& filepath) { return Derived::Create(filepath); }
		static Ref<Derived> Create(const std::string& name, const std::string& vertex_path, const std::string& fragment_path)
		{
			return Derived::Create(name, vertex_path, fragment_path);
		}
		static Ref<Derived> CreateFromString(const std::string& vertex_code, const std::string& fragment_code) { return Derived::CreateFromString(vertex_code, fragment_code); }
	};
}
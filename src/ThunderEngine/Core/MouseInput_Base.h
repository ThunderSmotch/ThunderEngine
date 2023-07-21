#pragma once
#pragma once

#include "Base.h"
#include <vector>

namespace ThunderEngine
{
	enum class MouseKeyCode
	{
		MOUSE_1 = 0,
		MOUSE_2 = 1,
		MOUSE_3 = 2,
		MOUSE_4 = 3,
		MOUSE_5 = 4,
		MOUSE_6 = 5,
		MOUSE_7 = 6,
		MOUSE_8 = 7,

		MOUSE_LEFT = MOUSE_1,
		MOUSE_RIGHT = MOUSE_2,
		MOUSE_CENTER = MOUSE_3,
	};

	// Handles mouse input.
	template <typename Derived>
	struct MouseInput_Base
	{
		static Ref<Derived> Create(std::vector<MouseKeyCode> keys_to_monitor)
		{
			return Derived::Create(keys_to_monitor);
		}

		bool GetIsKeyDown(MouseKeyCode key) { return TE_DERIVED(GetIsKeyDown, key); }
		bool GetIsKeyPress(MouseKeyCode key) { return TE_DERIVED(GetIsKeyPress, key); }

		bool GetIsEnabled() { return TE_DERIVED(GetIsEnabled); }
		void SetIsEnabled(bool enabled) { TE_DERIVED(SetIsEnabled, enabled); }

		// Mouse specific functions
		double GetScrollWheelYOffset() { return TE_DERIVED(GetScrollWheelYOffset); }
		double GetScrollWheelXOffset() { return TE_DERIVED(GetScrollWheelXOffset); }

	};
}
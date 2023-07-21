#pragma once

#include "ThunderEngine/Core/MouseInput_Base.h"

#include <map>
#include <vector>

struct GLFWwindow;

namespace ThunderEngine
{
	class GLFWMouseInput : public MouseInput_Base<GLFWMouseInput>
	{
	private:
		std::map<MouseKeyCode, bool> keys_down_;
		std::map<MouseKeyCode, bool> keys_press_;
		bool is_enabled_;
		bool update_presses_ = false;

	public:
		GLFWMouseInput(std::vector<MouseKeyCode> keys_to_monitor);
		~GLFWMouseInput();

		static Ref<GLFWMouseInput> Create(std::vector<MouseKeyCode> keys_to_monitor)
		{
			return CreateRef<GLFWMouseInput>(keys_to_monitor);
		}

		bool GetIsKeyDown(MouseKeyCode key);
		bool GetIsKeyPress(MouseKeyCode key);


		bool GetIsEnabled() { return is_enabled_; }
		void SetIsEnabled(bool enabled) { is_enabled_ = enabled; }
	private:
		void SetIsKeyDown(MouseKeyCode key, bool is_down);
		void SetIsKeyPress(MouseKeyCode key, bool is_press);

	// Static part of the class to make it work
	public:
		static void SetupGLFWMouseButtonInputs(GLFWwindow* window);
		static void UpdateKeyPresses();
	private:
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

		// Keep a list of all KeyInput instances and notify them all of key events
		static std::vector<GLFWMouseInput*> instances_;
	};
}
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

		double y_offset;
		double x_offset; // Most mouses do not have this

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

		double GetScrollWheelYOffset() { return y_offset; }
		double GetScrollWheelXOffset() { return x_offset; }

		bool GetIsEnabled() { return is_enabled_; }
		void SetIsEnabled(bool enabled) { is_enabled_ = enabled; }
	private:
		void SetIsKeyDown(MouseKeyCode key, bool is_down);
		void SetIsKeyPress(MouseKeyCode key, bool is_press);

	// Static part of the class to make it work
	public:
		static void SetupGLFWMouseInputs(GLFWwindow* window);
		static void UpdateKeyPresses();
	private:
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		// Keep a list of all KeyInput instances and notify them all of key events
		static std::vector<GLFWMouseInput*> instances_;
	};
}
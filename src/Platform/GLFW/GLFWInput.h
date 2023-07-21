#pragma once

#include "ThunderEngine/Core/KeyInput_Base.h"

#include <map>
#include <vector>

struct GLFWwindow;

// PERF Switch to a bitfield approach, less data is being stored that way and keys_press_ is as easy as &ing both fields.

namespace ThunderEngine
{
	class GLFWKeyInput : public KeyInput_Base<GLFWKeyInput>
	{
	private:
		std::map<KeyCode, bool> keys_down_;
		std::map<KeyCode, bool> keys_press_;
		bool is_enabled_;
		bool update_presses_ = false;

	public:
		GLFWKeyInput(std::vector<KeyCode> keys_to_monitor);
		~GLFWKeyInput();

		static Ref<GLFWKeyInput> Create(std::vector<KeyCode> keys_to_monitor)
		{
			return CreateRef<GLFWKeyInput>(keys_to_monitor);
		}

		bool GetIsKeyDown(KeyCode key);
		bool GetIsKeyPress(KeyCode key);


		bool GetIsEnabled() { return is_enabled_; }
		void SetIsEnabled(bool enabled) { is_enabled_ = enabled; }
	private:
		void SetIsKeyDown(KeyCode key, bool is_down);
		void SetIsKeyPress(KeyCode key, bool is_press);

		// Static part of the class to make it work
	public:
		static void SetupGLFWKeyInputs(GLFWwindow* window);
		static void UpdateKeyPresses();
	private:
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		// Keep a list of all KeyInput instances and notify them all of key events
		static std::vector<GLFWKeyInput*> instances_;
	};
}
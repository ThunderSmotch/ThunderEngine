#pragma once

#include "Core/KeyInput.h"
#include <map>
#include <vector>

struct GLFWwindow;

namespace ThunderEngine
{
	class GLFWKeyInput : public KeyInput
	{
	private:
		std::map<KeyCode, bool> keys_downs_;
		std::map<KeyCode, bool> keys_press_;
		bool is_enabled_;
		bool update_presses_ = false;

	public:
		GLFWKeyInput(std::vector<KeyCode> keys_to_monitor);
		virtual ~GLFWKeyInput() override;

		virtual bool GetIsKeyDown(KeyCode key) override;

		virtual bool GetIsKeyPress(KeyCode key) override;


		virtual bool GetIsEnabled() override { return is_enabled_; }
		virtual void SetIsEnabled(bool enabled) override { is_enabled_ = enabled; }
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
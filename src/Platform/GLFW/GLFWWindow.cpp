#include "GLFWWindow.h"
#pragma once

#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Renderer/RendererAPI.h"

#include "GLFWInput.h"

namespace ThunderEngine
{
	GLFWWindow::GLFWWindow(const WindowProps& props)
		:window_(nullptr), window_props_(props), vsync_enabled_(true)
	{
		int success = glfwInit();
		if (success == GLFW_FALSE)
		{
			TE_ERROR("Failed to initialize GLFW");
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window_ = glfwCreateWindow(props.width, props.height, props.title.c_str(), NULL, NULL);
		if (window_ == NULL)
		{
			TE_ERROR("Failed to create window!");
			glfwTerminate();
			exit(-1);
		}

		// Setup callbacks
		GLFWKeyInput::SetupGLFWKeyInputs(window_);

		glfwSetErrorCallback([](int error, const char* description)
			{
				TE_ERROR("GLFW: %", description);
			});

		//Defines the callback when the window is resized
		// MAYBE Change this to a callback on the Window class
		glfwSetFramebufferSizeCallback(window_, [](GLFWwindow* window, int width, int height)
			{
				RendererCommand::SetViewport(0, 0, width, height);
			});

		// Defines the callback when the window should close
		glfwSetWindowCloseCallback(window_, [](GLFWwindow* window) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
		data->OnWindowClose();
			});

		glfwSetWindowUserPointer(window_, &window_data_);

		graphics_context_ = GraphicsContext::Create(window_);
		SetVSync(true);
	}

	GLFWWindow::~GLFWWindow()
	{
		glfwDestroyWindow(window_);
		glfwTerminate();
	}

	void GLFWWindow::OnUpdate()
	{
		graphics_context_->SwapBuffers();
		GLFWKeyInput::UpdateKeyPresses();
		glfwPollEvents();
	}

	void GLFWWindow::SetWindowCloseCallback(std::function<void()> func)
	{
		window_data_.OnWindowClose = func;
	}

	void GLFWWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
		vsync_enabled_ = enabled;
	}

}
#pragma once
#include "tepch.h"

#include "GLFWWindow.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "GLFWInput.h"
#include "GLFWMouseInput.h"

#include "ThunderEngine/Renderer/RendererAPI.h"

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
		GLFWMouseInput::SetupGLFWMouseInputs(window_);

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

		// ImGui Initialization
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window_, true);
		ImGui_ImplOpenGL3_Init("#version 450");
	}

	GLFWWindow::~GLFWWindow()
	{
		// ImGui Shutdown
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window_);
		glfwTerminate();
	}

	void GLFWWindow::OnPreUpdate()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// FUTURE Assess if input should be processed here and in this order!
		// HAS TO BE THIS ORDER OR UPDATE KEY PRESSES DOES NOT WORK
		GLFWKeyInput::UpdateKeyPresses();
		GLFWMouseInput::UpdateKeyPresses();
		glfwPollEvents();
	}

	void GLFWWindow::OnUpdate()
	{
		// ImGui Render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		ImGui::EndFrame();

		// Swap framebuffers
		graphics_context_->SwapBuffers();
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
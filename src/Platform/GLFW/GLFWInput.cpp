#include "tepch.h"
#include "GLFWInput.h"

#include <GLFW/glfw3.h>

#include "ThunderEngine/Core/App.h"

namespace ThunderEngine
{
    std::vector<GLFWKeyInput*> GLFWKeyInput::instances_ = std::vector<GLFWKeyInput*>();

    GLFWKeyInput::GLFWKeyInput(std::vector<KeyCode> keys_to_monitor)
        : is_enabled_(true)
    {
        for (KeyCode key : keys_to_monitor)
        {
            keys_down_[key] = false;
            keys_press_[key] = false;
        }
        GLFWKeyInput::instances_.push_back(this);
    }

    GLFWKeyInput::~GLFWKeyInput()
    {
        if (!instances_.empty())
        {
            std::erase(instances_, this);
        }
    }

    bool GLFWKeyInput::GetIsKeyDown(KeyCode key)
    {
        bool result = false;
        if (is_enabled_) {
            std::map<KeyCode, bool>::iterator it = keys_down_.find(key);
            if (it != keys_down_.end()) {
                result = keys_down_[key];
            }
        }
        return result;
    }

    bool GLFWKeyInput::GetIsKeyPress(KeyCode key)
    {
        bool result = false;
        if (is_enabled_) {
            std::map<KeyCode, bool>::iterator it = keys_press_.find(key);
            if (it != keys_press_.end()) {
                result = keys_press_[key];
            }
        }
        return result;
    }

    void GLFWKeyInput::SetIsKeyDown(KeyCode key, bool is_down)
    {
        std::map<KeyCode, bool>::iterator it = keys_down_.find(key);
        if (it != keys_down_.end()) {
            keys_down_[key] = is_down;
        }
    }

    void GLFWKeyInput::SetIsKeyPress(KeyCode key, bool is_press)
    {
        std::map<KeyCode, bool>::iterator it = keys_press_.find(key);
        if (it != keys_press_.end()) {
            keys_press_[key] = is_press;
        }
    }

    void GLFWKeyInput::SetupGLFWKeyInputs(GLFWwindow* window)
    {
        glfwSetKeyCallback(window, GLFWKeyInput::KeyCallback);
    }

    void GLFWKeyInput::UpdateKeyPresses()
    {
        for (GLFWKeyInput* key_input : instances_)
        {
            if (!key_input->is_enabled_ || !key_input->update_presses_) {
                continue;
            }

            for (auto& key : key_input->keys_press_) {
                key.second = false;
            }
            key_input->update_presses_ = false;
        }
    }

    void GLFWKeyInput::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        KeyCode key_code = (KeyCode)key;

        for (GLFWKeyInput* key_input : instances_)
        {
            if (action == GLFW_PRESS) {
                key_input->SetIsKeyPress(key_code, true);
                key_input->update_presses_ = true;
            }
            key_input->SetIsKeyDown(key_code, (action != GLFW_RELEASE));
        }
    }
}
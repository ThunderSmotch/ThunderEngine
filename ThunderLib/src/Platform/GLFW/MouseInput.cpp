module ThunderLib.Platform.GLFW.MouseInput;

namespace ThunderLib
{
    std::vector<GLFWMouseInput*> GLFWMouseInput::instances_ = std::vector<GLFWMouseInput*>();

    GLFWMouseInput::GLFWMouseInput(std::vector<MouseKeyCode> keys_to_monitor)
        : is_enabled_(true)
    {
        for (MouseKeyCode key : keys_to_monitor)
        {
            keys_down_[key] = false;
            keys_press_[key] = false;
        }
        GLFWMouseInput::instances_.push_back(this);
    }

    GLFWMouseInput::~GLFWMouseInput()
    {
        instances_.erase(std::remove(instances_.begin(), instances_.end(), this), instances_.end());
    }

    bool GLFWMouseInput::GetIsKeyDown(MouseKeyCode key)
    {
        bool result = false;
        if (is_enabled_) {
            std::map<MouseKeyCode, bool>::iterator it = keys_down_.find(key);
            if (it != keys_down_.end()) {
                result = keys_down_[key];
            }
        }
        return result;
    }

    bool GLFWMouseInput::GetIsKeyPress(MouseKeyCode key)
    {
        bool result = false;
        if (is_enabled_) {
            std::map<MouseKeyCode, bool>::iterator it = keys_press_.find(key);
            if (it != keys_press_.end()) {
                result = keys_press_[key];
            }
        }
        return result;
    }

    void GLFWMouseInput::SetIsKeyDown(MouseKeyCode key, bool is_down)
    {
        std::map<MouseKeyCode, bool>::iterator it = keys_down_.find(key);
        if (it != keys_down_.end()) {
            keys_down_[key] = is_down;
        }
    }

    void GLFWMouseInput::SetIsKeyPress(MouseKeyCode key, bool is_press)
    {
        std::map<MouseKeyCode, bool>::iterator it = keys_press_.find(key);
        if (it != keys_press_.end()) {
            keys_press_[key] = is_press;
        }
    }

    void GLFWMouseInput::GetMousePosition(double& x_pos, double& y_pos) {
        x_pos = x_position_;
        y_pos = y_position_;
    }

    // Static functions

    void GLFWMouseInput::SetupGLFWMouseInputs(GLFWwindow* window)
    {
        glfwSetMouseButtonCallback(window, GLFWMouseInput::MouseButtonCallback);
        glfwSetScrollCallback(window, GLFWMouseInput::MouseScrollCallback);
        glfwSetCursorPosCallback(window, GLFWMouseInput::CursorPositionCallback);
    }

    void GLFWMouseInput::UpdateKeyPresses()
    {
        for (GLFWMouseInput* key_input : instances_)
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

    void GLFWMouseInput::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        MouseKeyCode key_code = (MouseKeyCode) button;

        for (GLFWMouseInput* key_input : instances_)
        {
            if (action == GLFW::PRESS) {
                key_input->SetIsKeyPress(key_code, true);
                key_input->update_presses_ = true;
            }
            key_input->SetIsKeyDown(key_code, (action != GLFW::RELEASE));
        }
    }

    void GLFWMouseInput::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        //TE_INFO("XOFFSET % YOFFSET %", xoffset, yoffset);

        for (GLFWMouseInput* mouse_input : instances_)
        {
            mouse_input->x_offset_ = xoffset;
            mouse_input->y_offset_ = yoffset;
        }
    }

    void GLFWMouseInput::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        for (GLFWMouseInput* mouse_input : instances_) 
        {
            mouse_input->x_position_ = xpos;
            mouse_input->y_position_ = ypos;
        }
    }

}
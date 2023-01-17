#include "KeyInput.h"

#include "Platform/GLFW/GLFWInput.h"

namespace ThunderEngine
{

	std::shared_ptr<KeyInput> KeyInput::Create(std::vector<KeyCode> keys_to_monitor)
	{
		return CreateRef<GLFWKeyInput>(keys_to_monitor);
	}
}
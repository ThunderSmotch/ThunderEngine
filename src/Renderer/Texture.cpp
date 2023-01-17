#include "Texture.h"

#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace ThunderEngine
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, uint32_t number_channels)
	{
		return CreateRef<OpenGLTexture2D>(width, height, number_channels);
	}
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		return CreateRef<OpenGLTexture2D>(path);
	}
}
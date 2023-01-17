#include "OpenGLTexture2D.h"

#include <stb/stb_image.h>

namespace ThunderEngine
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, uint32_t number_channels)
		:width_(width), height_(height)
	{
		switch (number_channels)
		{
		case 1:
			data_format_ = GL_RED;
			internal_format_ = GL_R8;
			break;
		case 3:
			data_format_ = GL_RGB8;
			internal_format_ = GL_RGB;
			break;
		case 4:
		default:
			internal_format_ = GL_RGBA8;
			data_format_ = GL_RGBA;
			break;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
		glTextureStorage2D(renderer_id_, 1, internal_format_, width_, height_);

		glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:path_(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data;
		data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (data) {
			is_loaded_ = true;

			width_ = width;
			height_ = height;

			GLenum internal_format = 0, data_format = 0;
			if (channels == 4)
			{
				internal_format = GL_RGBA8;
				data_format = GL_RGBA;
			}
			else if (channels == 3)
			{
				internal_format = GL_RGB8;
				data_format = GL_RGB;
			}
			internal_format_ = internal_format;
			data_format_ = data_format;

			if (!(internal_format & data_format))
			{
				TE_ERROR("Failed to load texture. Format is not supported!");
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
			glTextureStorage2D(renderer_id_, 1, internal_format_, width_, height_);

			glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(renderer_id_, 0, 0, 0, width_, height_, data_format, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
		else
		{
			TE_WARN("Could not load the image at %.", path);
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &renderer_id_);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t channels;
		
		switch (data_format_)
		{
			case GL_RED:
				channels = 1;
				break;
			case GL_RGB:
				channels = 3;
				break;
			default:
			case GL_RGBA:
				channels = 4;
				break;
		}

		if (size != width_ * height_ * channels)
		{
			TE_ERROR("Error setting data to texture! Size mismatch: data must be the entire texture! Expected % bytes, got %.", size, width_*height_*channels);
		}

		glTextureSubImage2D(renderer_id_, 0, 0, 0, width_, height_, data_format_, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, renderer_id_);
	}

}
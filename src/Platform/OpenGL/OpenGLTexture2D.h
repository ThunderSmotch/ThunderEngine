#pragma once

#include "ThunderEngine/Renderer/Texture_Base.h"

#include <glad/gl.h>

namespace ThunderEngine
{
	class OpenGLTexture2D : public Texture_Base<OpenGLTexture2D>
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, uint32_t number_channels);
		OpenGLTexture2D(const std::string& path);
		~OpenGLTexture2D();

		static Ref<OpenGLTexture2D> Create(uint32_t width, uint32_t height, uint32_t number_channels = 4)
		{
			return CreateRef<OpenGLTexture2D>(width, height, number_channels);
		}

		static Ref<OpenGLTexture2D> Create(const std::string& path)
		{
			return CreateRef<OpenGLTexture2D>(path);
		}

		uint32_t GetWidth() const { return width_; }
		uint32_t GetHeight() const { return height_; }
		uint32_t GetRendererID() const { return renderer_id_; }
		const std::string& GetPath() const { return path_; }

		void SetData(void* data, uint32_t size);

		void Bind(uint32_t slot = 0) const;

		bool IsLoaded() const { return is_loaded_; }

		bool operator==(const OpenGLTexture2D& other) const { return renderer_id_ == other.GetRendererID(); }

		
	private:
		std::string path_;
		bool is_loaded_ = false;
		uint32_t width_;
		uint32_t height_;

		GLenum internal_format_;
		GLenum data_format_;

		uint32_t renderer_id_;
	};
}
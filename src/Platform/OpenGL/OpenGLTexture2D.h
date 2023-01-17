#pragma once

#include "Renderer/Texture.h"

#include <glad/gl.h>

namespace ThunderEngine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, uint32_t number_channels);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return width_; }
		virtual uint32_t GetHeight() const override { return height_; }
		virtual uint32_t GetRendererID() const override { return renderer_id_; }
		virtual const std::string& GetPath() const override { return path_; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool IsLoaded() const override { return is_loaded_; }

		virtual bool operator==(const Texture& other) const override { return renderer_id_ == other.GetRendererID(); }
		
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
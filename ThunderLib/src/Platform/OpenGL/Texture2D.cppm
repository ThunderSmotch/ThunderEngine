module;
#include <glad/gl.h>
export module ThunderLib.Platform.OpenGL.Texture2D;

import std;
import ThunderLib.Base;

export namespace ThunderLib
{
	class OpenGLTexture2D
	{
	public:
		OpenGLTexture2D(u32 width, u32 height, u32 number_channels);
		OpenGLTexture2D(const std::string& path);
		~OpenGLTexture2D();

		static Ref<OpenGLTexture2D> Create(u32 width, u32 height, u32 number_channels = 4)
		{
			return CreateRef<OpenGLTexture2D>(width, height, number_channels);
		}

		static Ref<OpenGLTexture2D> Create(const std::string& path)
		{
			return CreateRef<OpenGLTexture2D>(path);
		}

		u32 GetWidth() const { return width_; }
		u32 GetHeight() const { return height_; }
		u32 GetRendererID() const { return renderer_id_; }
		const std::string& GetPath() const { return path_; }

		void SetData(void* data, u32 size);

		void Bind(u32 slot = 0) const;

		bool IsLoaded() const { return is_loaded_; }

		bool operator==(const OpenGLTexture2D& other) const { return renderer_id_ == other.GetRendererID(); }

		
	private:
		std::string path_;
		bool is_loaded_ = false;
		u32 width_;
		u32 height_;

		GLenum internal_format_;
		GLenum data_format_;

		u32 renderer_id_ = 0;
	};

	export using Texture2D = OpenGLTexture2D;
}
#pragma once

#include "ThunderEngine/Core/Base.h"

#include "Texture.h"
#include "Camera.h"

#include <stb/stb_truetype.h>

namespace ThunderEngine
{

	class RendererText 
	{
	private:
		static Ref<Texture2D> font_bitmap;
		static stbtt_bakedchar char_data[96];

	public:
		static void Init();
		static void Shutdown();

		static void LoadFont(const std::string& font_path);
		static void LoadFont2(const std::string& font_path);


		static void StartScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawString(float x, float y, const char* string);

	private:
		static void Flush();
		static void StartBatch();
		static void NextBatch();
	};
}


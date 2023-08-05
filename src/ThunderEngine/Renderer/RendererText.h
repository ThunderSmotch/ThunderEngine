#pragma once

#include "ThunderEngine/Core/Base.h"

#include "Texture.h"
#include "Camera.h"
#include "ThunderEngine/Assets/Font.h"

#include <stb/stb_truetype.h>

namespace ThunderEngine
{

	class RendererText 
	{
	private:
		static Ref<Texture2D> font_bitmap;

		static Ref<Font> font;

		static stbtt_bakedchar char_data[96];

	public:
		static void Init();
		static void Shutdown();

		static void LoadFontOld(const std::string& font_path);

		static float GetStringPixelWidth(const char* string, float scale = 1.0f);
		static void LoadFont(Ref<Font> font);

		static void StartScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawStringOld(float x, float y, const char* string, float scale = 1.0f);
		static void DrawString(float x, float y, const char* string, float scale = 1.0f);

	private:
		static void Flush();
		static void StartBatch();
		static void NextBatch();
	};
}


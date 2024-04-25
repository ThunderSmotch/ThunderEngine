module;
#include <stb/stb_truetype.h>
#include <glm/ext/matrix_transform.hpp>
export module ThunderEngine.RendererText;

import std;
import ThunderEngine.Base;
import ThunderEngine.Texture;
import ThunderEngine.Camera;
import ThunderEngine.Font;

export namespace ThunderEngine
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
		static void DrawString(float x, float y, const char* string, float scale = 1.0f, glm::vec4 text_color = {0.0f, 0.0f, 0.0f, 1.0f},
			float outline_size = 0.0f, glm::vec4 outline_color = {0.0f, 0.0f, 0.0f, 1.0f});

	private:
		static void Flush();
		static void StartBatch();
		static void NextBatch();
	};
}


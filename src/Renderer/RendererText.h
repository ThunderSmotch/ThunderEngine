#pragma once

#include "ThunderEngine.h"

#include "Texture.h"

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
		static void Test(float x, float y, const char* text);
	};
}


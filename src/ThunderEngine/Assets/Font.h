#pragma once

#include "ThunderEngine/Core/Base.h"

#include "ThunderEngine/Renderer/Texture.h"

struct stbtt_fontinfo;
struct stbrp_rect;

namespace ThunderEngine
{

	struct FontGlyph
	{
		int index;

		int advance;
		int left_bearing;

		int width;
		int height;
		int xoff;
		int yoff;

		// Atlas Coordinates of glyph
		int x_atlas = -1;
		int y_atlas = -1;
	};

	class Font
	{
	private:
		FontGlyph* _chars = nullptr; // Index i corresponds to Unicode codepoint
		int* _kerning = nullptr;
		int _num_chars;
		byte* _atlas = nullptr;
		int _atlas_size;

		// Font VMetrics
		//TODO new lines: you should advance the vertical position by "*ascent - *descent + *lineGap"
		int _ascent;
		int _descent;
		int _line_gap;
		float _scale;

		Ref<Texture2D> _bitmap = nullptr;

	public:
		Font(const std::string& font_path, int num_chars = 256, int atlas_size = 512);
		~Font();

		void BindAtlas();
		int GetKerning(int codepoint1, int codepoint2) const;

		FontGlyph GetCharacterInfo(int codepoint) const;
		int GetAtlasSize() const;
		float GetScale() const;

		float GetStringPixelWidth(const char* string, float scale = 1.0f) const;

		void SaveAtlasToFile(const std::string& name) const;

	private:
		void LoadCharacter(int codepoint, const stbtt_fontinfo& font_info, byte** sdfs);
		void PackGlyphsIntoAtlas(byte** sdfs);
		void LoadKerningInfo(const stbtt_fontinfo& font_info);
	};
}
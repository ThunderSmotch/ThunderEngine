module;
#include <stb/stb_rect_pack.h>
#include <stb/stb_truetype.h>
#include <stb/stb_image_write.h>
export module ThunderEngine.Font;

// MAYBE migrate to msdfgen in the future to handle straight edged fonts better
// TODO Implement Drop Shadows https://libgdx.com/wiki/graphics/2d/fonts/distance-field-fonts
// Review this https://www.youtube.com/watch?v=Y1kuhXtVAc4
// Original Valve paper https://steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf

import ThunderEngine.Base;
import ThunderEngine.Texture;

export namespace ThunderEngine
{

	// Saves spacing information about a specific character
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

	// A TTF font class, uses 32 pixels as the default line height
	// FontGlyph sizes have this scaling included, with the exception of advance/kerning/line_height
	class Font
	{
	private:
		FontGlyph* _chars = nullptr; // Index i corresponds to Unicode codepoint
		int* _kerning = nullptr;     // Stores Kerning information between characters
		int _num_chars;              // Number of characters in the Font (usually extended ASCII)
		byte* _atlas = nullptr;      // Bitmap Atlas
		int _atlas_size;

		// Font VMetrics
		//TODO new lines: you should advance the vertical position by "*ascent - *descent + *lineGap"
		int _ascent;
		int _descent;
		int _line_gap;

		float _scale; // Scale factor that is used to convert from the TTF representation to the chosen line height

		Ref<Texture2D> _bitmap = nullptr;

	public:
		Font(const std::string& font_path, int num_chars = 256, int atlas_size = 512);
		~Font();

		void BindAtlas();
		int GetKerning(int codepoint1, int codepoint2) const;

		FontGlyph GetCharacterInfo(int codepoint) const;
		int GetAtlasSize() const;
		float GetScale() const;

		int GetLineHeight() const;

		float GetStringPixelWidth(const char* string, float scale = 1.0f) const;

		void SaveAtlasToFile(const std::string& name) const;

	private:
		void LoadCharacter(int codepoint, const stbtt_fontinfo& font_info, byte** sdfs);
		void PackGlyphsIntoAtlas(byte** sdfs);
		void LoadKerningInfo(const stbtt_fontinfo& font_info);
	};
}
#include "tepch.h"

#include <fstream>
#include <sstream>

#include <stb/stb_rect_pack.h>
#include <stb/stb_truetype.h>
#include <stb/stb_image_write.h>

#include "Font.h"

namespace ThunderEngine
{
    byte* LoadTTF(const std::string& font_path, stbtt_fontinfo* font_info)
    {
        // Read TTF file to byte buffer
        bool load_result = true;
        
        byte* ttf_buffer = new byte[1 << 20];

        std::ifstream input(font_path, std::ios::in | std::ios::binary);
        long i = 0;
        while (input.good())
        {
            auto t = (byte) input.get();
            ttf_buffer[i++] = t;
        }

        load_result = stbtt_InitFont(font_info, ttf_buffer, 0);

        if (!load_result)
        {
            TE_WARN("Could not load font info at %.", font_path);
        }

        return ttf_buffer;
    }

    void Font::LoadCharacter(int codepoint, const stbtt_fontinfo& font_info, byte** sdfs)
    {
        int width;
        int height;
        int xoff;
        int yoff;
        int advance;
        int left_bearing;

        int glyph_index = stbtt_FindGlyphIndex(&font_info, codepoint);
        _chars[codepoint].index = glyph_index;

        stbtt_GetGlyphHMetrics(&font_info, glyph_index, &advance, &left_bearing);

        _chars[codepoint].advance = advance;
        _chars[codepoint].left_bearing = left_bearing;

        auto char_sdf = stbtt_GetGlyphSDF(&font_info, _scale, glyph_index,
            16, 128, 128.0f/5, &width, &height, &xoff, &yoff);

        if (width < 0 || height < 0) {
            width = 0;
            height = 0;
            xoff = 0;
            yoff = 0;
        }

        _chars[codepoint].height = height;
        _chars[codepoint].width = width;
        _chars[codepoint].xoff = xoff;
        _chars[codepoint].yoff = yoff;

        sdfs[codepoint] = char_sdf;
    }

    void Font::PackGlyphsIntoAtlas(byte** sdfs)
    {
        // Init STB RectPack
        stbrp_context sdf_atlas_context;
        auto nodes = new stbrp_node[_atlas_size];
        stbrp_init_target(&sdf_atlas_context, _atlas_size, _atlas_size, nodes, _atlas_size);
        auto* rects = new stbrp_rect[_num_chars];

        // Grab valid rect info
        int num_rects = 0;
        for (int codepoint = 0; codepoint < _num_chars; codepoint++)
        {
            int height = _chars[codepoint].height;
            int width = _chars[codepoint].width;

            if (height > 0 && width > 0)
            {
                rects[num_rects].id = codepoint;
                rects[num_rects].h = height;
                rects[num_rects].w = width;
                num_rects++;
            }
        }

        // Pack rects into atlas
        int pack_result = stbrp_pack_rects(&sdf_atlas_context, rects, num_rects);
        if (pack_result == 0) {
            TE_WARN("Could not pack entire font into the atlas.");
        }

        // Copy SDF bitmaps into atlas
        for (int i = 0; i < num_rects; i++)
        {
            int codepoint = rects[i].id;

            int x = rects[i].x;
            int y = rects[i].y;

            if (sdfs[codepoint] == nullptr)
            {
                continue;
            }

            for (int u = 0; u < rects[i].w; u++)
            {
                for (int v = 0; v < rects[i].h; v++)
                {
                    _atlas[x + u + (y + v) * _atlas_size] = sdfs[codepoint][u + v * rects[i].w];
                }
            }

            // Copy Atlas coords to FontGlyph
            _chars[codepoint].x_atlas = x;
            _chars[codepoint].y_atlas = y;
        }

        // Delete dynamically allocated data
        for (int codepoint = 0; codepoint < _num_chars; codepoint++)
        {
            stbtt_FreeSDF(sdfs[codepoint], nullptr);
        }
        delete[] nodes;
        delete[] rects;
    }

    void Font::LoadKerningInfo(const stbtt_fontinfo& font_info)
    {
        for (int codepoint_1 = 0; codepoint_1 < _num_chars; codepoint_1++)
        {
            for (int codepoint_2 = 0; codepoint_2 < _num_chars; codepoint_2++)
            {
                _kerning[codepoint_1 * _num_chars + codepoint_2] = stbtt_GetGlyphKernAdvance(
                    &font_info, _chars[codepoint_1].index, _chars[codepoint_2].index);
            }
        }
    }

    int Font::GetKerning(int codepoint1, int codepoint2) const
    {
        return _kerning[codepoint1 * _num_chars + codepoint2];
    }

	Font::Font(const std::string& font_path, int num_chars, int atlas_size) : _chars(new FontGlyph[num_chars]), _kerning(new int[num_chars * num_chars]),
        _num_chars(num_chars), _atlas(new byte[atlas_size*atlas_size]), _atlas_size(atlas_size)
	{
        //Load TTF data
        stbtt_fontinfo font_info;
        byte* ttf_buffer = LoadTTF(font_path, &font_info);

        // Load FontVMetrics
        stbtt_GetFontVMetrics(&font_info, &_ascent, &_descent, &_line_gap);

        _scale = stbtt_ScaleForPixelHeight(&font_info, 32.0f);

        // Get each character's data
        byte** sdfs = new byte* [num_chars];
        for (int codepoint = 0; codepoint < num_chars; codepoint++)
        {
            LoadCharacter(codepoint, font_info, sdfs);
        }

        // Get character kerning information
        LoadKerningInfo(font_info);

        // Pack Glyphs into Atlas
        PackGlyphsIntoAtlas(sdfs);

        delete[] ttf_buffer;
        delete[] sdfs;

        // Create Texture2D on GPU
        _bitmap = Texture2D::Create(_atlas_size, _atlas_size, 1);
        _bitmap->SetData(_atlas, sizeof(byte) * (_atlas_size * _atlas_size));
	}

    Font::~Font()
    {
        delete[] _chars;
    }

    void Font::SaveAtlasToFile(const std::string& name) const
    {
        stbi_write_bmp((std::string("res/textures/") + name + ".bmp").c_str(), _atlas_size, _atlas_size, 1, _atlas);
    }

    void Font::BindAtlas()
    {
        _bitmap->Bind();
    }

    FontGlyph Font::GetCharacterInfo(int codepoint) const
    {
        return _chars[codepoint];
    }

    int Font::GetAtlasSize() const
    {
        return _atlas_size;
    }

    float Font::GetScale() const
    {
        return _scale;
    }

    float Font::GetStringPixelWidth(const char* string, float scale) const
    {
        float width = 0.0f;

        for (int i = 0; string[i] != '\0'; i++)
        {
            int codepoint = string[i];
            width += (_chars[codepoint].advance + GetKerning(codepoint, string[i + 1]) ) * _scale * scale;
        }

        return width;
    }
}


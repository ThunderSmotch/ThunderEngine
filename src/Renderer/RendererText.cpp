#include "RendererText.h"

#include "Texture.h"
#include "Renderer2D.h"

#include <stb/stb_truetype.h>

#include <stb/stb_image_write.h>

#include <fstream>
#include <sstream>

namespace ThunderEngine
{
    Ref<Texture2D> RendererText::font_bitmap;
    stbtt_bakedchar RendererText::char_data[96];

    //stbtt_bakedchar[96] RendererText::char_data; // ASCII 32..126 is 95 glyphs

    void RendererText::Test(float x, float y, const char* text)
    {
        // TODO assumptions are wrong!!!!
        // assume orthographic projection with units = screen pixels, origin at top left        
        Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, font_bitmap, { 1.0f, 1.0f, 1.0f, 1.0f });

        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(char_data, 512, 512, 'A' - 32, &x, &y, &q, 1);
        //glEnable(GL_TEXTURE_2D);
        //glBindTexture(GL_TEXTURE_2D, ftex);
        //glBegin(GL_QUADS);
        //while (*text) {
        //    if (*text >= 32 && *text < 128) {
        //        stbtt_aligned_quad q;
        //        stbtt_GetBakedQuad(cdata, 512, 512, *text - 32, &x, &y, &q, 1);//1=opengl & d3d10+,0=d3d9
        //        glTexCoord2f(q.s0, q.t0); glVertex2f(q.x0, q.y0);
        //        glTexCoord2f(q.s1, q.t0); glVertex2f(q.x1, q.y0);
        //        glTexCoord2f(q.s1, q.t1); glVertex2f(q.x1, q.y1);
        //        glTexCoord2f(q.s0, q.t1); glVertex2f(q.x0, q.y1);
        //    }
        //    ++text;
        //}
        //glEnd();
    }

	void RendererText::Init()
	{
        // Reading Times New Roman
        unsigned char* ttf_buffer = new unsigned char[1 << 20];

        std::ifstream input("res/fonts/open_sans.ttf", std::ios::in | std::ios::binary);

        long i = 0;
        while (input.good()) 
        {
            unsigned char t = input.get();
            ttf_buffer[i++] = t;
        }

        unsigned char* temp_bitmap = new unsigned char[512 * 512];


        stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, temp_bitmap, 512, 512, 32, 96, char_data); // no guarantee this fits!
        // can free ttf_buffer at this point
        delete[] ttf_buffer;

        font_bitmap = Texture2D::Create(512, 512, 1);
        font_bitmap->SetData(temp_bitmap, sizeof(unsigned char)*(512*512));

        stbi_write_bmp("res/textures/test.bmp", 512, 512, 1, temp_bitmap);

        delete[] temp_bitmap;
	}

}
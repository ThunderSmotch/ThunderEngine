module;
#include <stb/stb_rect_pack.h>
#include <stb/stb_truetype.h>
#include <stb/stb_image_write.h>
#include <glm/ext/matrix_transform.hpp>
module ThunderLib.RendererText;

import std;

import ThunderLib.Buffer;
import ThunderLib.VertexArray;
import ThunderLib.Shader;
import ThunderLib.RendererAPI;

static const std::string text2d_vertex_source =
#include "../res/shaders/Text2DVertex.glsl"
;

static const std::string text2d_fragment_source =
#include "../res/shaders/Text2DFragment.glsl"
;

static const std::string textsdf_vertex_source =
#include "../res/shaders/TextSDFVertex.glsl"
;

static const std::string textsdf_fragment_source =
#include "../res/shaders/TextSDFFragment.glsl"
;

namespace ThunderLib
{

    struct CharVertex
    {
        glm::vec4 a_pos;     // Text Quad Vertex
        glm::vec4 a_col;     // Text Color
        float a_out_size;    // Outline Size
        glm::vec4 a_out_col; // Outline Color
    };

    struct RendererTextData
    {
        static const uint32_t MaxChars = 10000;
        static const uint32_t MaxVertices = MaxChars * 4;
        static const uint32_t MaxIndices = MaxChars * 6;
        static const uint32_t MaxTextureSlots = 32;

        // Char information
        uint32_t char_index_count = 0;
        CharVertex* char_vertex_buffer_base = nullptr;
        CharVertex* char_vertex_buffer_ptr = nullptr;

        Ref<Shader> char_shader = nullptr;
        Ref<VertexArray> char_vertex_array = nullptr;
        Ref<VertexBuffer> char_vertex_buffer = nullptr;

        Ref<Shader> sdf_shader = nullptr;
    };
    static RendererTextData text_data;

    Ref<Texture2D> RendererText::font_bitmap = nullptr;

    Ref<Font> RendererText::font = nullptr;

    stbtt_bakedchar RendererText::char_data[96]; // Character information about texture subunits and offsets



    void RendererText::Init()
    {
        text_data.char_vertex_array = VertexArray::Create();

        text_data.char_vertex_buffer = VertexBuffer::Create(RendererTextData::MaxVertices*sizeof(CharVertex));
        text_data.char_vertex_buffer->SetLayout({
            {"aPos", ShaderDataType::Float4},
            {"aCol", ShaderDataType::Float4},
            {"aOutSize", ShaderDataType::Float},
            {"aOutCol", ShaderDataType::Float4}
            });
        text_data.char_vertex_array->AddVertexBuffer(text_data.char_vertex_buffer);

        // Allocate space for max vertices
        text_data.char_vertex_buffer_base = new CharVertex[RendererTextData::MaxVertices];

        // Allocate and set Index Buffer
        uint32_t* char_indices = new uint32_t[RendererTextData::MaxIndices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < RendererTextData::MaxIndices; i += 6)
        {
            char_indices[i + 0] = offset + 0;
            char_indices[i + 1] = offset + 1;
            char_indices[i + 2] = offset + 2;

            char_indices[i + 3] = offset + 2;
            char_indices[i + 4] = offset + 3;
            char_indices[i + 5] = offset + 0;

            offset += 4;
        }

        Ref<IndexBuffer> char_ib = IndexBuffer::Create(char_indices, RendererTextData::MaxIndices);
        text_data.char_vertex_array->SetIndexBuffer(char_ib);
        delete[] char_indices;

        // Create quad shader
        text_data.char_shader = Shader::CreateFromString(text2d_vertex_source, text2d_fragment_source);
        text_data.sdf_shader = Shader::CreateFromString(textsdf_vertex_source, textsdf_fragment_source);

        /* MAYBE Not needed 
        text_data.quad_vertex_positions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        text_data.quad_vertex_positions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        text_data.quad_vertex_positions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
        text_data.quad_vertex_positions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
        */
    }

    void RendererText::Shutdown()
    {
        delete[] text_data.char_vertex_buffer_base;
    }

    static stbtt_fontinfo font_info;

	void RendererText::LoadFontOld(const std::string& font_path)
	{
        unsigned char* ttf_buffer = new unsigned char[1 << 20];

        // Read file to byte buffer
        std::ifstream input(font_path, std::ios::in | std::ios::binary);

        long i = 0;
        while (input.good()) 
        {
            unsigned char t = input.get();
            ttf_buffer[i++] = t;
        }

        unsigned char* temp_bitmap = new unsigned char[512 * 512];

        // Convert to 1D bitmap
        stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, temp_bitmap, 512, 512, 32, 96, char_data); // no guarantee this fits!
        // can free ttf_buffer at this point
        delete[] ttf_buffer;

        // Create Texture2D on GPU
        font_bitmap = Texture2D::Create(512, 512, 1);
        font_bitmap->SetData(temp_bitmap, sizeof(unsigned char)*(512*512));

        // MAYBE Set up optional flag to save the bitmap
        //stbi_write_bmp("res/textures/test.bmp", 512, 512, 1, temp_bitmap);

        delete[] temp_bitmap;
	}

    float RendererText::GetStringPixelWidth(const char* string, float scale)
    {
        return font->GetStringPixelWidth(string, scale);
    }

    void RendererText::LoadFont(Ref<Font> _font)
    {
        font = _font;
    }

    void RendererText::StartScene(const OrthographicCamera& camera)
    {
        // Update camera uniform
        text_data.char_shader->Bind();
        text_data.char_shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        
        text_data.sdf_shader->Bind();
        text_data.sdf_shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        StartBatch();
    }

    void RendererText::EndScene()
    {
        Flush();
    }

    void RendererText::StartBatch()
    {
        text_data.char_index_count = 0;
        text_data.char_vertex_buffer_ptr = text_data.char_vertex_buffer_base;

        //text_data.texture_slot_index = 1;
    }

    void RendererText::NextBatch()
    {
        Flush();
        StartBatch();
    }

    void RendererText::Flush()
    {
        if (text_data.char_index_count)
        {
            uint32_t data_size = (uint32_t)((uint8_t*)text_data.char_vertex_buffer_ptr - (uint8_t*)text_data.char_vertex_buffer_base);
            text_data.char_vertex_buffer->SetData(text_data.char_vertex_buffer_base, data_size);

            // Bind texture
            // OLD font_bitmap->Bind();
            

            if (font != nullptr) 
            {
                text_data.sdf_shader->Bind();
                font->BindAtlas();
            }
            else if(font_bitmap != nullptr) // OLD
            {
                text_data.char_shader->Bind();
                font_bitmap->Bind();
            }

            RendererAPI::DrawIndexed(text_data.char_vertex_array, text_data.char_index_count);
        }
    }

    void RendererText::DrawStringOld(float x, float y, const char* string, float scale) 
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), {x, y, 1.0f}) * glm::scale(glm::mat4(1.0f), {scale, scale, 1.0f});
    
        constexpr size_t quad_vertex_count = 4;
        
        float xpos = x;
        float ypos = y;

        for (int i = 0; string[i] != '\0'; i++)
        {
            if (text_data.char_index_count >= RendererTextData::MaxIndices)
            {
                NextBatch();
            }

            float ipw = 1.0f / 512, iph = 1.0f / 512; // Coordinates in bitmap
            stbtt_bakedchar ch = char_data[string[i] - 32];

            // NOTE @Thunder: These coordinates start from the top left as the origin, however
            // as OpenGL flips textures and we never flipped the generated bitmap, we can just use the value as is.
            float s0 = ch.x0 * ipw; float t0 = ch.y0 * iph; // Top Left
            float s1 = ch.x1 * ipw; float t1 = ch.y1 * iph; // Bottom Right


            // PERF surely there's a better way to calculate what I want than this
            float c_width  = ch.x1 - ch.x0;
            float c_height = ch.y1 - ch.y0;

            float x0 = xpos + ch.xoff;
            float x1 = x0 + c_width;

            float y0 = ypos - ch.yoff;
            float y1 = y0 - c_height;

            glm::vec4 pos1 = transform * glm::vec4(x0, y0, 0.0f, 0.0f); // BL
            glm::vec4 pos2 = transform * glm::vec4(x1, y1, 0.0f, 0.0f); // TR

            text_data.char_vertex_buffer_ptr->a_pos = { pos1.x, pos1.y, s0, t0 }; // BL
            text_data.char_vertex_buffer_ptr++;
            text_data.char_vertex_buffer_ptr->a_pos = { pos2.x, pos1.y, s1, t0 }; // BR
            text_data.char_vertex_buffer_ptr++;
            text_data.char_vertex_buffer_ptr->a_pos = { pos2.x, pos2.y, s1, t1 }; // TR
            text_data.char_vertex_buffer_ptr++;
            text_data.char_vertex_buffer_ptr->a_pos = { pos1.x, pos2.y, s0, t1 }; // TL
            text_data.char_vertex_buffer_ptr++;

            xpos += ch.xadvance;

            text_data.char_index_count += 6;
        }
    }

    void RendererText::DrawString(float x, float y, const char* string, float scale, glm::vec4 text_color,
        float outline_size, glm::vec4 outline_color)
    {
        float xpos = x;
        float ypos = y;

        int atlas_size = font->GetAtlasSize();
        float font_scale = font->GetScale();

        for (int i = 0; string[i] != '\0'; i++)
        {
            if (text_data.char_index_count >= RendererTextData::MaxIndices)
            {
                NextBatch();
            }

            if (string[i] == '\n')
            {
                ypos -= font->GetLineHeight() * font_scale * scale;
                xpos = x;
                continue;
            }

            FontGlyph chr = font->GetCharacterInfo(string[i]);

            float ipw = 1.0f / atlas_size, iph = 1.0f / atlas_size; // Coordinates in bitmap

            // MAYBE move UV coordinates to the Font class

            // NOTE @Thunder: These coordinates start from the top left as the origin, however
            // as OpenGL flips textures and we never flipped the generated bitmap, we can just use the value as is.
            float s0 = chr.x_atlas * ipw; float t0 = chr.y_atlas * iph; // Top Left
            float s1 = (chr.x_atlas + chr.width) * ipw; float t1 = (chr.y_atlas + chr.height) * iph; // Bottom Right

            float x0 = xpos + chr.xoff * scale;
            float x1 = x0 + chr.width * scale;

            float y0 = ypos - chr.yoff * scale;
            float y1 = y0 - chr.height * scale;

            glm::vec4 vertices[4] = {
                { x0, y0, s0, t0 }, // BL
                { x1, y0, s1, t0 }, // BR
                { x1, y1, s1, t1 }, // TR
                { x0, y1, s0, t1 }  // TL
            };

            for (int v = 0; v < 4; v++)
            {
                text_data.char_vertex_buffer_ptr->a_pos = vertices[v];
                text_data.char_vertex_buffer_ptr->a_col = text_color;
                text_data.char_vertex_buffer_ptr->a_out_size = outline_size;
                text_data.char_vertex_buffer_ptr->a_out_col = outline_color;
                text_data.char_vertex_buffer_ptr++;
            }

            xpos += ( (float) (chr.advance + font->GetKerning(string[i], string[i+1])) )*font_scale*scale;

            text_data.char_index_count += 6;
        }
    }
}
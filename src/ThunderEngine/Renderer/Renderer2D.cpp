module;
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
module ThunderEngine.Renderer2D;

import ThunderEngine.Base;
import ThunderEngine.Buffer;
import ThunderEngine.Shader;
import ThunderEngine.VertexArray;
import ThunderEngine.RendererAPI;
import ThunderEngine.Texture;

// Engine Shaders are imported here
static const std::string line_vertex_source = 
#include "../res/shaders/LineVertex.glsl"
;

static const std::string line_fragment_source =
#include "../res/shaders/LineFragment.glsl"
;

static const std::string circle_vertex_source =
#include "../res/shaders/CircleVertex.glsl"
;

static const std::string circle_fragment_source =
#include "../res/shaders/CircleFragment.glsl"
;

static const std::string triangle_vertex_source =
#include "../res/shaders/TriangleVertex.glsl"
;

static const std::string triangle_fragment_source =
#include "../res/shaders/TriangleFragment.glsl"
;

static const std::string quad_vertex_source =
#include "../res/shaders/QuadVertex.glsl"
;

static const std::string quad_fragment_source =
#include "../res/shaders/QuadFragment.glsl"
;

namespace ThunderEngine
{

    struct LineVertex
    {
        glm::vec2 position;
        glm::vec4 color;
    };

    struct TriangleVertex
    {
        glm::vec3 position;
        glm::vec4 color;
    };

    struct QuadVertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 tex_coord;
        float tex_index;
    };

    struct CircleVertex 
    {
        glm::vec3 world_position;
        glm::vec3 local_position;
        glm::vec4 color;
        glm::vec4 border_color;
        float border_thickness;
        float thickness;
        float fade;
    };

    // Memory being allocated on the heap
    // Lines -> 80k vertices -> 6*4*80k = ~1.8 Mb

    // TODO: Not sure if this is the best way to store these variables
    // This is being put on the ThunderEngine namespace, everyone can access it.
    struct Renderer2DData
    {
        static const uint32_t MaxQuads = 9000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32;

        // Lines variables /////////////////////////////////
        Ref<Shader>       line_shader             = nullptr;
        Ref<VertexArray>  line_vertex_array       = nullptr;
        Ref<VertexBuffer> line_vertex_buffer      = nullptr;
        
        uint32_t          line_vertex_count       = 0;
        LineVertex*       line_vertex_buffer_base = nullptr;
        LineVertex*       line_vertex_buffer_ptr  = nullptr;

        float             line_width              = 2.0f;
        ////////////////////////////////////////////////////

        // Circles /////////////////////////////////////////
        Ref<Shader>       circle_shader             = nullptr;
        Ref<VertexArray>  circle_vertex_array       = nullptr;
        Ref<VertexBuffer> circle_vertex_buffer      = nullptr;

        uint32_t          circle_index_count = 0;
        CircleVertex*     circle_vertex_buffer_base = nullptr;
        CircleVertex*     circle_vertex_buffer_ptr  = nullptr;
        ///////////////////////////////////////////////////
        
        
        // Triangles ///////////////////////////////////////
        Ref<Shader> triangle_shader = nullptr;
        Ref<VertexArray> triangle_vertex_array = nullptr;
        Ref<VertexBuffer> triangle_vertex_buffer = nullptr;
        
        uint32_t triangle_index_count;
        TriangleVertex* triangle_vertex_buffer_base = nullptr;
        TriangleVertex* triangle_vertex_buffer_ptr = nullptr;

        Ref<IndexBuffer> triangle_ib = nullptr;
        ///////////////////////////////////////////////////


        // Quads variables
        uint32_t quad_index_count = 0;
        QuadVertex* quad_vertex_buffer_base = nullptr;
        QuadVertex* quad_vertex_buffer_ptr = nullptr;

        Ref<Shader> quad_shader = nullptr;
        Ref<VertexArray> quad_vertex_array = nullptr;
        Ref<VertexBuffer> quad_vertex_buffer = nullptr;
        Ref<Texture2D> white_texture = nullptr;

        Ref<IndexBuffer> quad_ib = nullptr;
        std::array<glm::vec4,4> quad_vertex_positions = {glm::vec4(), glm::vec4(), glm::vec4(), glm::vec4()};

        std::array<Ref<Texture2D>, MaxTextureSlots> texture_slots;
        uint32_t texture_slot_index = 1; // 0 = white texture
    };

    // TODO move this inside Renderer2D
    static Renderer2DData data;

    void Renderer2D::InitLine() {
        data.line_vertex_array = VertexArray::Create();

        // Create and set the Line Vertex Buffer layout
        data.line_vertex_buffer = VertexBuffer::Create(ThunderEngine::Renderer2DData::MaxVertices * sizeof(LineVertex));
        data.line_vertex_buffer->SetLayout({
            {"aPos", ShaderDataType::Float2},
            {"aCol", ShaderDataType::Float4}
            });
        data.line_vertex_array->AddVertexBuffer(data.line_vertex_buffer);

        data.line_vertex_buffer_base = new LineVertex[ThunderEngine::Renderer2DData::MaxVertices];

        data.line_shader = Shader::CreateFromString(line_vertex_source, line_fragment_source);
    }

    void Renderer2D::InitCircle() 
    {
        data.circle_vertex_array = VertexArray::Create();

        data.circle_vertex_buffer = VertexBuffer::Create(Renderer2DData::MaxVertices * sizeof(CircleVertex));
        data.circle_vertex_buffer->SetLayout({
            {"aWorldPosition", ShaderDataType::Float3},
            {"aLocalPosition", ShaderDataType::Float3},
            {"aColor", ShaderDataType::Float4},
            {"aBorderColor", ShaderDataType::Float4},
            {"aBorderThickness", ShaderDataType::Float},
            {"aThickness", ShaderDataType::Float},
            {"aFade", ShaderDataType::Float}
            });
        data.circle_vertex_array->AddVertexBuffer(data.circle_vertex_buffer);

        data.circle_vertex_buffer_base = new CircleVertex[ThunderEngine::Renderer2DData::MaxVertices];
        data.circle_vertex_array->SetIndexBuffer(data.quad_ib); // Reuse quad IndexBuffer

        data.circle_shader = Shader::CreateFromString(circle_vertex_source, circle_fragment_source);
    }

    void Renderer2D::InitTriangle() {
        data.triangle_shader = Shader::CreateFromString(triangle_vertex_source, triangle_fragment_source);

        data.triangle_vertex_buffer = VertexBuffer::Create(Renderer2DData::MaxVertices*sizeof(TriangleVertex));
        data.triangle_vertex_buffer->SetLayout({
            {"aPos", ShaderDataType::Float3},
            {"aCol", ShaderDataType::Float4}
            });

        data.triangle_vertex_array = VertexArray::Create();
        data.triangle_vertex_array->AddVertexBuffer(data.triangle_vertex_buffer);

        // Allocate space for max vertices
        data.triangle_vertex_buffer_base = new TriangleVertex[Renderer2DData::MaxVertices];

        // FIX This is stupid, I should really have a DrawVertices inside RendererCommand, to avoid DrawIndexed!
        // TODO  this will be swapped with a Triangle Strip approach in the end.
        // Allocate and set Index Buffer
        uint32_t* triangle_indices = new uint32_t[Renderer2DData::MaxVertices];

        for (uint32_t i = 0; i < Renderer2DData::MaxVertices; i++)
        {
            triangle_indices[i] = i;
        }

        data.triangle_ib = IndexBuffer::Create(triangle_indices, Renderer2DData::MaxVertices);
        data.triangle_vertex_array->SetIndexBuffer(data.triangle_ib);
        delete[] triangle_indices;

    }

    void Renderer2D::InitQuad() {

        
        // Create Quad Vertex Buffer and its layout
        data.quad_vertex_buffer = VertexBuffer::Create(Renderer2DData::MaxVertices*sizeof(QuadVertex));
        data.quad_vertex_buffer->SetLayout({
            {"aPos", ShaderDataType::Float3},
            {"aCol", ShaderDataType::Float4},
            {"aTexCoord", ShaderDataType::Float2},
            {"aTexIndex", ShaderDataType::Float},
            });

        data.quad_vertex_array = VertexArray::Create();
        data.quad_vertex_array->AddVertexBuffer(data.quad_vertex_buffer);

        // Allocate space for max vertices
        data.quad_vertex_buffer_base = new QuadVertex[Renderer2DData::MaxVertices];
        
        // Allocate and set Index Buffer
        uint32_t* quad_indices = new uint32_t[Renderer2DData::MaxIndices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < Renderer2DData::MaxIndices; i += 6)
        {
            quad_indices[i + 0] = offset + 0;
            quad_indices[i + 1] = offset + 1;
            quad_indices[i + 2] = offset + 2;

            quad_indices[i + 3] = offset + 2;
            quad_indices[i + 4] = offset + 3;
            quad_indices[i + 5] = offset + 0;

            offset += 4;
        }

        data.quad_ib = IndexBuffer::Create(quad_indices, Renderer2DData::MaxIndices);
        data.quad_vertex_array->SetIndexBuffer(data.quad_ib);
        delete[] quad_indices;

        // Create quad shader
        data.quad_shader = Shader::CreateFromString(quad_vertex_source, quad_fragment_source);
        
        data.quad_vertex_positions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        data.quad_vertex_positions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        data.quad_vertex_positions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
        data.quad_vertex_positions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
    }

    void Renderer2D::InitWhiteTexture() 
    {
        data.white_texture = Texture2D::Create(1, 1);
        uint32_t white_texture_data = 0xffffffff;
        data.white_texture->SetData(&white_texture_data, sizeof(uint32_t));

        data.texture_slots[0] = data.white_texture;
    }

    void Renderer2D::Init()
    {
        InitLine();
        InitTriangle();
        InitQuad();
        InitCircle();

        InitWhiteTexture();
    }

    void Renderer2D::Shutdown()
    {
        delete[] data.quad_vertex_buffer_base;
        delete[] data.triangle_vertex_buffer_base;
        delete[] data.line_vertex_buffer_base;
        delete[] data.circle_vertex_buffer_base;
    }

    void Renderer2D::StartScene(const OrthographicCamera& camera)
    {
        // PERF (Carlos) Can we store the camera in a better space? 
        // Currently I have to upload it manually to the several different shaders per frame.
        // Maybe using an uniform buffer!

        // Update camera uniform
        data.quad_shader->Bind();
        data.quad_shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        data.triangle_shader->Bind();
        data.triangle_shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        data.line_shader->Bind();
        data.line_shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        data.circle_shader->Bind();
        data.circle_shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        StartBatch();
    }

    void Renderer2D::EndScene()
    {
        Flush();
    }

    void Renderer2D::StartBatch()
    {
        data.line_vertex_count = 0;
        data.line_vertex_buffer_ptr = data.line_vertex_buffer_base;

        data.quad_index_count = 0;
        data.quad_vertex_buffer_ptr = data.quad_vertex_buffer_base;

        data.triangle_index_count = 0;
        data.triangle_vertex_buffer_ptr = data.triangle_vertex_buffer_base;

        data.circle_index_count = 0;
        data.circle_vertex_buffer_ptr = data.circle_vertex_buffer_base;

        data.texture_slot_index = 1;
    }

    void Renderer2D::NextBatch()
    {
        Flush();
        StartBatch();
    }

    void Renderer2D::Flush()
    {
        if (data.quad_index_count)
        {
            uint32_t data_size = (uint32_t)((uint8_t*)data.quad_vertex_buffer_ptr - (uint8_t*)data.quad_vertex_buffer_base);
            data.quad_vertex_buffer->SetData(data.quad_vertex_buffer_base, data_size);

            // Bind textures
            for (uint32_t i = 0; i < data.texture_slot_index; i++)
                data.texture_slots[i]->Bind(i);
            RendererAPI::DrawIndexed(data.quad_vertex_array, data.quad_index_count);
        }

        if (data.triangle_index_count)
        {
            uint32_t data_size = (uint32_t)((uint8_t*)data.triangle_vertex_buffer_ptr - (uint8_t*)data.triangle_vertex_buffer_base);
            data.triangle_vertex_buffer->SetData(data.triangle_vertex_buffer_base, data_size);

            data.triangle_shader->Bind();
            RendererAPI::DrawIndexed(data.triangle_vertex_array, data.triangle_index_count);
        }

        if (data.line_vertex_count)
        {
            uint32_t data_size = (uint32_t)((uint8_t*)data.line_vertex_buffer_ptr - (uint8_t*)data.line_vertex_buffer_base);
            data.line_vertex_buffer->SetData(data.line_vertex_buffer_base, data_size);

            data.line_shader->Bind();
            RendererAPI::SetLineWidth(data.line_width);
            RendererAPI::DrawLines(data.line_vertex_array, data.line_vertex_count);
        }

        if (data.circle_index_count)
        {
            uint32_t data_size = (uint32_t)((uint8_t*)data.circle_vertex_buffer_ptr - (uint8_t*)data.circle_vertex_buffer_base);
            data.circle_vertex_buffer->SetData(data.circle_vertex_buffer_base, data_size);

            data.circle_shader->Bind();
            RendererAPI::DrawIndexed(data.circle_vertex_array, data.circle_index_count);
        }
    }

    void Renderer2D::DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color)
    {
        if (data.line_vertex_count >= Renderer2DData::MaxVertices)
        {
            NextBatch();
        }

        data.line_vertex_buffer_ptr->position = start;
        data.line_vertex_buffer_ptr->color = color;
        data.line_vertex_buffer_ptr++;

        data.line_vertex_buffer_ptr->position = end;
        data.line_vertex_buffer_ptr->color = color;
        data.line_vertex_buffer_ptr++;

        data.line_vertex_count += 2;
    }

    void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, const glm::vec4& border_color, float border_thickness, float thickness, float fade)
    {
        if (data.circle_index_count >= Renderer2DData::MaxIndices)
        {
            NextBatch();
        }

        for (size_t i = 0; i < 4; i++)
        {
            data.circle_vertex_buffer_ptr->world_position = transform * data.quad_vertex_positions[i];
            data.circle_vertex_buffer_ptr->local_position = data.quad_vertex_positions[i] * 2.0f;
            data.circle_vertex_buffer_ptr->color = color;
            data.circle_vertex_buffer_ptr->border_color = border_color;
            data.circle_vertex_buffer_ptr->border_thickness = border_thickness;
            data.circle_vertex_buffer_ptr->thickness = thickness;
            data.circle_vertex_buffer_ptr->fade = fade;
            data.circle_vertex_buffer_ptr++;
        }

        data.circle_index_count += 6;
    }

    void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade)
    {
        DrawCircle(transform, color, color, 0.0f, thickness, fade);
    }

    void Renderer2D::DrawCircle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const glm::vec4 border_color,
        float border_thickness, float thickness, float fade) 
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        DrawCircle(transform, color, border_color, border_thickness, thickness, fade);
    }

    void Renderer2D::DrawCircle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float thickness, float fade)
    {
        DrawCircle(position, size, color, color, 0.0f, thickness, fade);
    }

    void Renderer2D::DrawCircle(const glm::vec3& position, float radius, const glm::vec4& color, float thickness, float fade)
    {
        DrawCircle(position, { radius, radius }, color, thickness, fade);
    }

    void Renderer2D::DrawCircle(const glm::vec3& position, float radius, const glm::vec4& color, const glm::vec4 border_color,
        float border_thickness, float thickness, float fade)
    {
        DrawCircle(position, { radius, radius }, color, border_color, border_thickness, thickness, fade);
    }

    void Renderer2D::DrawTriangle(
        const glm::vec3& p1, const glm::vec4& c1,
        const glm::vec3& p2, const glm::vec4& c2, 
        const glm::vec3& p3, const glm::vec4& c3)
    {
        if (data.triangle_index_count >= Renderer2DData::MaxVertices)
        {
            NextBatch();
        }

        data.triangle_vertex_buffer_ptr->position = p1;
        data.triangle_vertex_buffer_ptr->color = c1;
        data.triangle_vertex_buffer_ptr++;

        data.triangle_vertex_buffer_ptr->position = p2;
        data.triangle_vertex_buffer_ptr->color = c2;
        data.triangle_vertex_buffer_ptr++;

        data.triangle_vertex_buffer_ptr->position = p3;
        data.triangle_vertex_buffer_ptr->color = c3;
        data.triangle_vertex_buffer_ptr++;

        data.triangle_index_count += 3;
    }

    void Renderer2D::DrawTriangle(const glm::vec2& p1, const glm::vec4& c1,
        const glm::vec2& p2, const glm::vec4& c2,
        const glm::vec2& p3, const glm::vec4& c3)
    {
        DrawTriangle({ p1.x, p1.y, 0.9f }, c1, { p2.x, p2.y, 0.9f }, c2, { p3.x, p3.y, 0.9f }, c3);
    }

    void Renderer2D::DrawTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec4& color)
    {
        DrawTriangle(p1, color, p2, color, p3, color);
    }


    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, rotation, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        DrawQuad(transform, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        DrawQuad(transform, color);
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
    {
        constexpr size_t quad_vertex_count = 4;
        const float texture_index = 0.0f; // White Texture
        constexpr std::array<glm::vec2,4> texture_coords = { glm::vec2(0.0f), { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

        if (data.quad_index_count >= Renderer2DData::MaxIndices)
        {
            NextBatch();
        }

        for (size_t i = 0; i < quad_vertex_count; i++)
        {
            data.quad_vertex_buffer_ptr->position = transform * data.quad_vertex_positions[i];
            data.quad_vertex_buffer_ptr->color = color;
            data.quad_vertex_buffer_ptr->tex_coord = texture_coords[i];
            data.quad_vertex_buffer_ptr->tex_index = texture_index;
            data.quad_vertex_buffer_ptr++;
        }
        data.quad_index_count += 6;
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint_color)
    {
        DrawQuad({position.x, position.y, 0.0f}, rotation, size, texture, tint_color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint_color)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        DrawQuad(transform, texture, tint_color);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint_color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture, tint_color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint_color)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        DrawQuad(transform, texture, tint_color);
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tint_color)
    {
        constexpr size_t quad_vertex_count = 4;
        constexpr std::array<glm::vec2,4> texture_coords = { glm::vec2(0.0f), {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

        if (data.quad_index_count >= Renderer2DData::MaxIndices)
        {
            NextBatch();
        }

        float texture_index = 0.0f; 
        for (uint32_t i = 1; i < data.texture_slot_index; i++)
        {
            if (*data.texture_slots[i] == *texture)
            {
                texture_index = (float) i;
                break;
            }
        }

        // If not in the texture_slots then add it to the array
        if (texture_index == 0.0f)
        {
            if (data.texture_slot_index >= Renderer2DData::MaxTextureSlots)
                NextBatch();

            texture_index = (float) data.texture_slot_index;
            data.texture_slots[data.texture_slot_index] = texture;
            data.texture_slot_index++;
        }

        for (size_t i = 0; i < quad_vertex_count; i++)
        {
            data.quad_vertex_buffer_ptr->position = transform * data.quad_vertex_positions[i];
            data.quad_vertex_buffer_ptr->color = tint_color;
            data.quad_vertex_buffer_ptr->tex_coord = texture_coords[i];
            data.quad_vertex_buffer_ptr->tex_index = texture_index;
            data.quad_vertex_buffer_ptr++;
        }
        data.quad_index_count += 6;
    }

    /*
    void Renderer2D::DrawTriangle(const glm::vec2& position)
    {
        data.shader_triangle->Bind();
        data.va_triangle->Bind();
        RendererAPI::DrawIndexed(data.va_triangle, 3);
    }
    */

    void Renderer2D::SetLineWidth(float width)
    {
        data.line_width = width;
    }

    float Renderer2D::GetLineWidth()
    {
        return data.line_width;
    }
}
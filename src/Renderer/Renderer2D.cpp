#include "Renderer2D.h"

#include "ThunderEngine.h"

#include <array>

#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Buffer.h"
#include "VertexArray.h"

#include "RendererAPI.h"
#include "Texture.h"

namespace ThunderEngine
{
    struct QuadVertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 tex_coord;
        float tex_index;
    };

    struct Renderer2DData
    {
        static const uint32_t MaxQuads = 20000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

        // Triangles
        Ref<Shader> shader_triangle = nullptr;
        Ref<VertexArray> va_triangle = nullptr;

        // Quads information
        uint32_t quad_index_count = 0;
        QuadVertex* quad_vertex_buffer_base = nullptr;
        QuadVertex* quad_vertex_buffer_ptr = nullptr;

        Ref<Shader> quad_shader = nullptr;
        Ref<VertexArray> quad_vertex_array = nullptr;
        Ref<VertexBuffer> quad_vertex_buffer = nullptr;
        Ref<Texture2D> white_texture = nullptr;

        glm::vec4 quad_vertex_positions[4] = {glm::vec4(), glm::vec4(), glm::vec4(), glm::vec4()};

        std::array<Ref<Texture2D>, MaxTextureSlots> texture_slots;
        uint32_t texture_slot_index = 1; // 0 = white texture
    };

    static Renderer2DData data;


    void Renderer2D::InitTriangle() {
        data.shader_triangle = Shader::Create("res/shaders/Shader.shader");

        float vertices[] =
        {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.0f, 0.5f,
        };

        Ref<VertexBuffer> vb = VertexBuffer::Create(vertices, sizeof(vertices));
        vb->SetLayout({ BufferElement("aPos", ShaderDataType::Float2) });

        data.va_triangle = VertexArray::Create();
        data.va_triangle->AddVertexBuffer(vb);
    }

    void Renderer2D::InitQuad() {

        data.quad_vertex_array = VertexArray::Create();
        
        // Create Quad Vertex Buffer and its layout
        data.quad_vertex_buffer = VertexBuffer::Create(data.MaxVertices*sizeof(QuadVertex));
        data.quad_vertex_buffer->SetLayout({
            {"aPos", ShaderDataType::Float3},
            {"aCol", ShaderDataType::Float4},
            {"aTexCoord", ShaderDataType::Float2},
            {"aTexIndex", ShaderDataType::Float},
            });
        data.quad_vertex_array->AddVertexBuffer(data.quad_vertex_buffer);

        // Allocate space for max vertices
        data.quad_vertex_buffer_base = new QuadVertex[data.MaxVertices];
        
        // Allocate and set Index Buffer
        uint32_t* quad_indices = new uint32_t[data.MaxIndices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < data.MaxIndices; i += 6)
        {
            quad_indices[i + 0] = offset + 0;
            quad_indices[i + 1] = offset + 1;
            quad_indices[i + 2] = offset + 2;

            quad_indices[i + 3] = offset + 2;
            quad_indices[i + 4] = offset + 3;
            quad_indices[i + 5] = offset + 0;

            offset += 4;
        }

        Ref<IndexBuffer> quad_ib = IndexBuffer::Create(quad_indices, data.MaxIndices);
        data.quad_vertex_array->SetIndexBuffer(quad_ib);
        delete[] quad_indices;

        // Create quad shader
        data.quad_shader = Shader::Create("QuadShader", "res/shaders/QuadVertex.glsl", "res/shaders/QuadFragment.glsl");
        
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
        //InitTriangle();
        InitQuad();
        InitWhiteTexture();
    }

    void Renderer2D::Shutdown()
    {
        delete[] data.quad_vertex_buffer_base;
    }

    void Renderer2D::StartScene(const OrthographicCamera& camera)
    {
        // Update camera uniform
        data.quad_shader->Bind();
        data.quad_shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        StartBatch();
    }

    void Renderer2D::EndScene()
    {
        Flush();
    }

    void Renderer2D::StartBatch()
    {
        data.quad_index_count = 0;
        data.quad_vertex_buffer_ptr = data.quad_vertex_buffer_base;

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

            data.quad_shader->Bind();
            RendererCommand::DrawIndexed(data.quad_vertex_array, data.quad_index_count);
        }
    }

    void Renderer2D::DrawTriangle() // TODO Remove or fix the code
    {
        data.shader_triangle->Bind();
        data.va_triangle->Bind();
        RendererCommand::DrawIndexed(data.va_triangle, 3);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, color);
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
        constexpr glm::vec2 texture_coords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

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
        constexpr glm::vec2 texture_coords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

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

    void Renderer2D::DrawTriangle(const glm::vec2& position)
    {
        data.shader_triangle->Bind();
        data.va_triangle->Bind();
        RendererCommand::DrawIndexed(data.va_triangle, 3);
    }
}
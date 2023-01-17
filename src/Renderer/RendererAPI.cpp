#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace ThunderEngine
{
    UniqueRef<RendererAPI> RendererAPI::Create()
    {
        return CreateUniqueRef<OpenGLRendererAPI>();
    }

    // RendererCommand
    UniqueRef<RendererAPI> RendererCommand::renderer_api_ = RendererAPI::Create();


    void RendererCommand::Init()
    {
        renderer_api_->Init();
    }

    void RendererCommand::DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count)
    {
        renderer_api_->DrawIndexed(vertex_array, index_count);
    }

    void RendererCommand::DrawLines(const Ref<VertexArray>& vertex_array, uint32_t vertex_count)
    {
        renderer_api_->DrawLines(vertex_array, vertex_count);
    }

    void RendererCommand::SetWireframeMode(bool enabled)
    {
        renderer_api_->SetWireframeMode(enabled);
    }

    void RendererCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        renderer_api_->SetViewport(x, y, width, height);
    }

    void RendererCommand::Clear()
    {
        renderer_api_->Clear();
    }

    void RendererCommand::SetClearColor(const glm::vec4& color)
    {
        renderer_api_->SetClearColor(color);
    }

}

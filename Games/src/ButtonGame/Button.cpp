module;
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <imgui/imgui.h>
module ButtonGame.Button;
using namespace ThunderLib;

ButtonMachine::ButtonMachine()
{
    width = 760.0;
    height = 160.0;
    border_size = 20.0;
    rotation = 20.0f;

    position = 0.0f;
    button_color = Color::Red;
}

void ButtonMachine::Render(float dt)
{

    // ImGUI
    ImGui::Begin("Button Machine");
    ImGui::SliderFloat("Button Position", &position, -1.0f, 1.0f);
    ImGui::End();

    // Button Container
    float outer_box_width = width + 2 * border_size;
    float outer_box_height = height + 2 * border_size;

    // Outer Box
    Renderer2D::DrawQuad(glm::vec3({ 0.0f, 0.0f, 0.0f }), rotation, glm::vec2({ outer_box_width, outer_box_height }), Color::DarkGrey);

    Renderer2D::DrawCircle({ button_position, 0.0f }, outer_box_height, Color::DarkGrey);
    Renderer2D::DrawCircle({ -button_position, 0.0f }, outer_box_height, Color::DarkGrey);

    // Inner Box
    Renderer2D::DrawQuad(glm::vec3({ 0.0f, 0.0f, 0.1f }), rotation, glm::vec2({ outer_box_width, height }), Color::Grey);

    Renderer2D::DrawCircle({ button_position, 0.1f }, height, Color::Grey);
    Renderer2D::DrawCircle({ -button_position, 0.1f }, height, Color::Grey);

    //Button
    Renderer2D::DrawCircle({ button_position*position, 0.2f }, height, button_color, Color::Black, 0.1f);

    // Title 
    // Title Variables
    static glm::vec4 title_color = Color::White;
    static glm::vec4 outline_color = Color::Black;
    static float outline_size = 0.0f;

    ImGui::Begin("Title Text");
    ImGui::ColorEdit4("Text Color", &title_color.r);
    ImGui::ColorEdit4("Outline Color", &outline_color.r);
    ImGui::SliderFloat("Outline Size", &outline_size, 0.0f, 1.0f);
    ImGui::End();

    float title_scale = 2.0f;
    float title_width = RendererText::GetStringPixelWidth("Button Machine", title_scale);

    Renderer2D::DrawLine({ -title_width / 2, 290.0f }, { title_width / 2 , 290.0f }, Color::Yellow);
    RendererText::DrawString(-title_width / 2, 300.0f, "Button Machine", 2.0f, title_color, outline_size, outline_color);
}

void ButtonMachine::ProcessInput(MouseInput& mouse)
{
    float circleX = button_position.x * position;
    float circleY = button_position.y * position;

    double x;
    double y;
    mouse.GetMousePosition(x, y);

    // TODO Convert Mouse Coords to World Coords
    x = x - 1280 / 2;
    y = 720 / 2 - y;

    bool inside_circle = (x - circleX) * (x - circleX) + (y - circleY) * (y - circleY) <= (height / 2) * (height / 2);

    // Fix button coords
    if(inside_circle && mouse.GetIsKeyDown(MouseKeyCode::MOUSE_LEFT))
    {
        button_color = Color::Yellow;
        SoundPlayer::PlaySound("hit");
    }
    else
    {
        button_color = Color::Red;
    }
}

void ButtonMachine::Update(float dt)
{
    static float t = 0;
    t += dt;

    float outer_box_width = width + 2 * border_size;

    position = sin(t);
    button_position = { cos(glm::radians(rotation)) * outer_box_width / 2, sin(glm::radians(rotation)) * outer_box_width / 2 };
}
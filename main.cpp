#include "implementation.hpp"

int main()
{
    // Initialize new object of type 'Implement'
    // Window name is required
    Implementation imgui { "ImGui DirectX9 Implementation", 800, 600 };

    // Loop message
    while (imgui.get_msg().message != WM_QUIT)
    {
        if (PeekMessageA(&imgui.get_msg(), nullptr, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&imgui.get_msg());
            DispatchMessageA(&imgui.get_msg());
            continue;
        }
        
        // Start new frame
        imgui.start_frame();

        // Your code start here

        if (ImGui::Begin("Some title"))
        {
            ImGui::Text("Hello World!");
            if (ImGui::Button("Light Style"))
            {
                imgui.set_style(Implementation::style_color::LIGHT);
            }
        } 
        ImGui::End();

        // Your code end here

        // End frame
        imgui.end_frame();
        // Define color of background
        imgui.set_background_color(0, 255, 0);
    }
}
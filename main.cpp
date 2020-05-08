#include "implementation.hpp"

int main()
{
    // Initialize new object of type 'Implement'
    // Window name is required
    Implementation imgui{ "ImGui DirectX9 Implementation", "DX", { 1920, 800 }, { 0, 0 }, Implementation::class_style::SAVEBITS, WS_OVERLAPPEDWINDOW };

    imgui.start_window();

    //Example variables
    bool show_demo_window       { true };
    bool show_another_window    { true };
    float float_slider          { 0.0f };
    int counter                 { 0    };
    ImVec4 simple_color         { 0.40f, 0.50f, 0.60f, 1.0f };

    // Loop message
    while ( imgui.get_msg().message != WM_QUIT )
    {
        if ( PeekMessageA( &imgui.get_msg(), nullptr, 0U, 0U, PM_REMOVE ) )
        {
            TranslateMessage( &imgui.get_msg() );
            DispatchMessageA( &imgui.get_msg() );
            continue;
        }
        
        // Start new frame
        imgui.start_frame();

        // Your code start here
        if ( show_demo_window )
            ImGui::ShowDemoWindow( &show_demo_window );

        {
            ImGui::Begin( "Hello, world!" );                              // Create a window called "Hello, world!" and append into it.

            ImGui::Text( "This is some useful text." );                   // Display some text (you can use a format strings too)
            ImGui::Checkbox( "Demo Window", &show_demo_window );          // Edit bools storing our window open/close state
            ImGui::Checkbox( "Another Window", &show_another_window );

            ImGui::SliderFloat( "float", &float_slider, 0.0f, 1.0f );     // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3( "simple color", reinterpret_cast< float* >( &simple_color ) );   // Edit 3 floats representing a color
            if ( ImGui::Button( "Dark Style Color" ) )
                imgui.set_style( Implementation::color_style::dark );
            if ( ImGui::Button( "Button" ) )                                // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text( "counter = %d", counter );

            ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
            ImGui::End();
        }

        if ( show_another_window )
        {
            ImGui::Begin( "Another Window", &show_another_window );       // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text( "Hello from another window!" );
            if ( ImGui::Button( "Close Me" ) )
                show_another_window = false;
            ImGui::End();
        }
        // Your code end here

        // End frame
        imgui.end_frame();
        // Define color of background
        imgui.set_background_color(0, 255, 0);
    }
}
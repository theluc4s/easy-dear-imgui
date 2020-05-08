# Implementation ImGui DirectX9
[![](https://img.shields.io/github/license/theluc4s/ImGui-DX9-Implementation.svg)](https://raw.githubusercontent.com/theluc4s/ImGui-DX9-Implementation/master/LICENSE) [![](https://img.shields.io/badge/release-last-green.svg)](https://github.com/theluc4s/ImGui-DX9-Implementation/releases)

* [Design goals](https://github.com/theluc4s/ImGui-DX9-Implementation#design-goals)
  * [Single-header integration](https://github.com/theluc4s/ImGui-DX9-Implementation#single-header-integration)
  * [Don't fail](https://github.com/theluc4s/ImGui-DX9-Implementation#dont-fail)
* [Examples](https://github.com/theluc4s/ImGui-DX9-Implementation#examples)
  * [Visual Studio 2019](https://github.com/theluc4s/ImGui-DX9-Implementation#visual-studio-2019)
* [User functions](https://github.com/theluc4s/ImGui-DX9-Implementation#user-functions)
* [Special thanks](https://github.com/theluc4s/ImGui-DX9-Implementation#special-thanks)

## Design goals
There are several variants for implementing imgui using C++. Each project has its implementation. My class has the following goals:

###### Single-header integration:
The class was built in a single header and is not tied to dependencies, in addition to ImGui and the SDK of directx. You do not need C++17 to use it.

###### Don't fail:
A simple check using try when creating the object prevents a potential problem. Improvements are being implemented.

## Examples
Get the latest ImGui release or download my release that always contains the update for ImGui.
> The imgui files need to be in a 'imgui' folder and the implementation.hpp outside that folder.

#### Visual Studio 2019
main.cpp
```cpp
//Include implementation.hpp
#include "implementation.hpp"
```
```cpp
//Include implementation.hpp
#include "implementation.hpp"

int main()
{
    //Create an object of type: Implementation
    //The first argument is required:
    //std::string &window_name
    //std::string &class_name = "ImGui Basic Setup"
    //vector2 size { int x, int y } = { 1280, 800 }
    //vector2 pos  { int x, int y } = { 0, 0 }
    //style_class &class_style = style_class::CLASSDC
    Implementation imgui("ImGui DirectX9 Implementation");
}
```
```cpp
//Include implementation.hpp
#include "implementation.hpp"

int main()
{
    //Create an object of type: Implementation
    //The first argument is required:
    //std::string &window_name
    //vector2 size { int x, int y } = { 1280, 800 }
    //vector2 pos  { int x, int y } = { 0, 0 }
    //style_class &class_style = style_class::CLASSDC
    Implementation imgui("ImGui DirectX9 Implementation");
	
    //Start our window
    imgui.start_window();

    // Create a loop message
    // Use get_msg() to get reference to m_msg
    while (imgui.get_msg().message != WM_QUIT)
    {
        if (PeekMessageA(&imgui.get_msg(), nullptr, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&imgui.get_msg());
            DispatchMessageA(&imgui.get_msg());
            continue;
        }
    }
}
```
```cpp
//Include implementation.hpp
#include "implementation.hpp"

int main()
{
    //Create an object of type: Implementation
    //The first argument is required:
    //std::string &window_name
    //std::string &class_name = "ImGui Basic Setup"
    //vector2 size { int x, int y } = { 1280, 800 }
    //vector2 pos  { int x, int y } = { 0, 0 }
    //style_class &class_style = style_class::CLASSDC
    Implementation imgui("ImGui DirectX9 Implementation");
	
    //Example variables
    bool show_demo_window       { true };
    bool show_another_window    { true };
    float float_slider          { 0.0f };
    int counter                 { 0    };
    ImVec4 simple_color         { 0.40f, 0.50f, 0.60f, 1.0f };
	
    //Start our window
    imgui.start_window();
	
    // Create a loop message
    // Use get_msg() to get reference to m_msg
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
        imgui.set_background_color(255, 255, 255);
    }
}
```
## User functions
The following functions are available through the instance of the object of type Implementation:

| Function name | Description                    |
| ------------- | ------------------------------ |
| `void start_window()`      | **Creates the new Window and Start**       |
| `void start_frame()`      | **Creates the new imgui frame**       |
| `void end_frame()`   | **Finalizes the frame of the imgui**     |
| `void set_background_color(int,int,int)`   | **Define a new color**     |
| `void set_style(const style_color&)`   | **Color style of the ImGui using an enum**     |
| `MSG& get_msg()`   | **Returns a reference to m_msg**     |
| `const HWND& get_hwnd() const`   | **Returns a reference to m_hwnd**     |
| `const WNDCLASSEX& get_wnd_class() const`   | **Returns a reference to m_window_class**     |
| `const std::string& get_name() const`   | **Returns a reference to m_window_name**     |
| `const std::string& get_class_name() const`   | **Returns a reference to m_class_name**     |
| `const vector2& get_size() const`   | **Returns a reference to m_size**     |
| `const vector2& get_pos() const`   | **Returns a reference to m_pos**     |
| `const class_style& get_class_style() const`   | **Returns a reference to enum class_style**     |
| `const unsigned long get_wnd_style() const`   | **Returns a reference to m_window_style**     |
| `void set_name( std::string name )`   | **Sets the name of the window**     |
| `void set_class_name( std::string class_name )`   | **Sets the class name of the window**     |
| `void set_size( vector2 init_wnd_size )`   | **Sets the initial window size**     |
| `void set_pos( vector2 init_wnd_pos )`   | **Sets the starting position of the window.**     |
| `void set_class_style( class_style cs )`   | **Sets the style of the window class**     |
| `void set_wnd_style( unsigned long wnd_style )`   | **Sets the window style**     |

## Special thanks
Thanks to [@Darkratos](https://github.com/Darkratos) and [@Nomade040](https://github.com/Nomade040) by test and point me improvements.

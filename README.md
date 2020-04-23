# Implementation ImGui DirectX9
[![](https://img.shields.io/github/license/theluc4s/ImGui-DX9-Implementation)](https://raw.githubusercontent.com/theluc4s/ImGui-DX9-Implementation/master/LICENSE)

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
One of the main factors that I am solving is the lack of error handling in my class, this can bring headaches. But I am implementing this.
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
    //The first argument is required
    Implementation imgui("ImGui DirectX9 Implementation");
}
```
```cpp
//Include implementation.hpp
#include "implementation.hpp"

int main()
{
    //Create an object of type: Implementation
    //The first argument is required
    Implementation imgui("ImGui DirectX9 Implementation");

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
    //The first argument is required
    Implementation imgui("ImGui DirectX9 Implementation");

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
```
## User functions
The following functions are available through the instance of the object of type Implementation:

| Function name | Description                    |
| ------------- | ------------------------------ |
| `void start_frame()`      | **Creates the new imgui frame**       |
| `void end_frame()`   | **Finalizes the frame of the imgui**     |
| `void set_background_color(int,int,int)`   | **Define a new color**     |
| `void set_style(const style_color&)`   | **Color style of the ImGui using an enum**     |
| `MSG& get_msg()`   | **Returns a reference to m_msg**     |
| `const HWND& get_hwnd() const`   | **Returns a reference to m_hwnd**     |
| `const WNDCLASSEX& get_hwnd() const`   | **Returns a reference to m_window_class**     |

## Special thanks
Thanks to [Darkratos](https://github.com/Darkratos) and [@Nomade040](https://github.com/Nomade040) by test and point me improvements.

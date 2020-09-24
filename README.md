# Easy Dear ImGui
[![](https://img.shields.io/github/license/theluc4s/Easy-Dear-ImGui.svg)](https://raw.githubusercontent.com/theluc4s/Easy-Dear-ImGui/master/LICENSE) [![](https://img.shields.io/badge/release-last-green.svg)](https://github.com/theluc4s/Easy-Dear-ImGui/releases)

* [Design goals](https://github.com/theluc4s/Easy-Dear-ImGui#design-goals)
  * [Single-header integration](https://github.com/theluc4s/Easy-Dear-ImGui#single-header-integration)
  * [Don't fail](https://github.com/theluc4s/Easy-Dear-ImGui#dont-fail)
* [Examples](https://github.com/theluc4s/Easy-Dear-ImGui#examples)
  * [Visual Studio 2019](https://github.com/theluc4s/Easy-Dear-ImGui#visual-studio-2019)
* [User functions](https://github.com/theluc4s/Easy-Dear-ImGui#user-functions)
* [Special thanks](https://github.com/theluc4s/Easy-Dear-ImGui#special-thanks)

## Design goals
There are several variants for implementing dear imgui using C++. Each project has its implementation. My class has the following goals:

###### Single-header integration:
The class was built in a single header and is not linked to dependencies.

###### Don't fail:
If the window cannot be built or some state of the context of the imgui cannot be defined, it is safe to call the functions in the loop, nothing will be drawn and no exception will be thrown.
> Assuming Visual Studio, a call to OutputDebugStringA through a log macro and you can directly view the line, function and reason for the failure.

## Examples
Get the [latest ImGui release](https://github.com/ocornut/imgui/releases/) and download my release.
> The imgui files need to be in a 'dear_imgui' folder and the easy_dear_imgui.hpp outside that folder.

#### Visual Studio 2019
main.cpp
```cpp
#include "easy_dear_imgui.hpp"

int __stdcall WinMain(
	_In_ HINSTANCE,
	_In_opt_ HINSTANCE,
	_In_ LPSTR,
	_In_ int
)
{
	//Use the easy_di namespace
	easy_di::window window
	{
		"My Window",	//Window name
		"my_window",	//Class window name
		{ 0, 0 },		//Start position window
		{ 200, 100 },	//Start size window
		true			//Vertical sincronization, default value is 'true'
	};

	//If process_message() returns false, the message was equal to WM_QUIT or CreateWindowExA failed and returned a nullptr handle.
	while ( window.process_message() )
	{
		if ( window.imgui_start_frame() )
		{
			ImGui::ShowDemoWindow();

			window.imgui_end_frame();
		}

		window.set_background_color();
	}

	return 0;
}
```

#### Custom window procedure template
If you want to use a custom window procedure, use the template below and write your own.
```cpp
LRESULT __stdcall custom_wnd_procedure( HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param )
{
	if ( ImGui_ImplWin32_WndProcHandler( hwnd, msg, w_param, l_param ) )
		return true;

	switch ( msg )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	case WM_SIZE:
		if ( easy_di::dx::g_ptr_d3d_device && ( w_param != SIZE_MINIMIZED ) )
		{
			easy_di::dx::g_d3d_pp.BackBufferWidth  = easy_di::low_order < unsigned short, LPARAM >( l_param );
			easy_di::dx::g_d3d_pp.BackBufferHeight = easy_di::high_order< unsigned short, LPARAM >( l_param );
			easy_di::dx::reset_device();
		}
		return 0;
	default:
		break;
	}

	return DefWindowProcA( hwnd, msg, w_param, l_param );
}
```
Then just define g_cwnd_proc before creating the object.
```cpp
//before create the object
easy_di::dx::g_cwnd_proc = custom_wnd_procedure;
//create the object
easy_di::window window
	{
		"My Window",    //Window name
		"my_window",    //Class window name
		{ 0, 0 },       //Start position window
		{ 200, 100 },   //Start size window
		true            //Vertical sincronization, default value is 'true'
	};
```


## User functions
The following functions are available:

| Function name | Description |
| --- | :---: |
| `bool process_message()` | **Process the message queue and message** |
| `bool imgui_start_frame()` | **Creates a dear imgui frame.** |
| `bool imgui_end_frame()` | **Ends a dear imgui frame.** |
| `void set_background_color(int,int,int)` | **Define a new background color** |
| `bool get_vsync_state()` | **Returns the state of vertical synchronization.** |
| `MSG& get_msg()` | **Returns a reference to m_msg** |

## Special thanks
Thanks to [@Darkratos](https://github.com/Darkratos) and [@Nomade040](https://github.com/Nomade040) by test and point me improvements.

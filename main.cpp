#include "easy_dear_imgui.hpp"


int __stdcall WinMain(
	_In_ HINSTANCE,
	_In_opt_ HINSTANCE,
	_In_ LPSTR,
	_In_ int
)
{
	// Use the easy_di namespace
	//
	easy_di::window window
	{
		"My Window",         // Window name
		"my_window",         // Class window name
		{ 0, 0 },            // Start position window
		{ 1280, 720 },       // Start size window
		SW_SHOW,             // Window show state       ( default parameter )
		CS_CLASSDC,          // Window class style      ( default parameter )
		WS_OVERLAPPEDWINDOW, // Window style            ( default parameter )
		true                 // Vertical sincronization ( default parameter )
	};

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

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
		"My Window",                                                  // Window name
		"my_window",                                                  // Class window name
		{ 0, 0 },                                                     // Start position window
		{ 1280, 720 },                                                // Start size window
		SW_SHOW,                                                      // Window show state       ( default parameter = SW_SHOW )
		CS_CLASSDC,                                                   // Window class style      ( default parameter = CS_CLASSDC )
		WS_OVERLAPPEDWINDOW,                                          // Window style            ( default parameter = WS_OVERLAPPEDWINDOW )
		// A wrapper for adding an icon has been added (add the icon to your project's resource and use your ID)
		//
		easy_di::utils::icon( 0 /*RESOURCE ICON ID*/, { 128, 128 } ), // Icon                    ( default parameter = nullptr )
		easy_di::utils::icon( 0 /*RESOURCE ICON ID*/, { 16, 16 } ),   // Small icon              ( default parameter = nullptr )
		true                                                          // Vertical sincronization ( default parameter = true )
	};

	// If process_message() returns false, the message was equal to WM_QUIT or CreateWindowExA failed and returned a nullptr handle.
	//
	while ( window.process_message() )
	{
		if ( window.imgui_start_frame() )
		{
			ImGui::ShowDemoWindow();

			window.imgui_end_frame();
		}

		window.set_background_color();
	}
}

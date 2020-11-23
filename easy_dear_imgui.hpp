#pragma once
#include "dear_imgui/imgui.h"
#include "dear_imgui/imgui_impl_dx9.h"
#include "dear_imgui/imgui_impl_win32.h"

#include <d3d9.h>
#include <memory>
#include <string>
#include <system_error>
#include <Windows.h>

#pragma comment( lib, "d3d9.lib" )

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

namespace easy_di
{
	namespace log
	{
#define log(format, ...) trace_impl( __FILE__, __LINE__, __FUNCTION__, format, __VA_ARGS__ )

		std::string get_last_error_as_string( const int error_code )
		{
			char *buffer{ nullptr };

			const auto msg{ FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
											nullptr, // (not used with FORMAT_MESSAGE_FROM_SYSTEM)
											error_code,
											MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
											reinterpret_cast< LPSTR >( &buffer ),
											0,
											nullptr ) };
			if ( msg > 0 )
			{
				auto deleter{ []( void *ptr )
				{
					LocalFree( ptr );
				} };

				const std::unique_ptr< char, decltype( deleter ) > message{ buffer, deleter };

				return message.get();
			}

			return std::system_error( GetLastError(), std::system_category(),
									  "Failed to retrieve error message string.\n" ).what();
		}

		void trace_impl( const char *file_name, const int line, const char *func_sig, const char *format, ... )
		{
			// Format example:
			// https://docs.microsoft.com/en-us/cpp/build/reference/fc-full-path-of-source-code-file-in-diagnostics?view=vs-2019
			// [10] File: dummy.cpp
			// [10] Func: dummy_foo
			// [10] GetLastError: [0] - The operation completed successfully.
			// [10] Reason: nullptr argument
			//

			static constexpr auto buffer_size{ 1024u };
			auto buffer{ std::make_unique< char[] >( buffer_size ) };

			const auto error_code{ GetLastError() };

			snprintf( buffer.get(), buffer_size,
					  "[%d] File: %s\n[%d] Func: %s\n[%d] GetLastError: [%d] - %s[%d] Reason: ",
					  line, file_name,
					  line, func_sig,
					  line, error_code, get_last_error_as_string( error_code ).data(),
					  line );

			OutputDebugStringA( buffer.get() );

			// retrieve the variable arguments
			//
			va_list args{};
			va_start( args, format );

			vsnprintf( buffer.get(), buffer_size, format, args );

			OutputDebugStringA( buffer.get() );

			va_end( args );
		}
	}

	struct vec2
	{
		int m_x;
		int m_y;

		vec2( const int x, const int y ) :
			m_x{ x },
			m_y{ y }
		{}
		~vec2()
		{
			this->m_x = 0;
			this->m_y = 0;
		}
	};

	template< typename T, typename A >
	T low_order( A data )
	{
		return ( ( sizeof( T ) < sizeof( A ) ) ? static_cast< T >( data & 0xffffffff ) : throw );
	}
	template< typename T, typename A >
	T high_order( A data )
	{
		return ( ( sizeof( T ) < sizeof( A ) ) ? static_cast< T >( ( data >> ( sizeof( T ) * 8 ) ) & 0xffffffff ) : throw );
	}

	namespace dx
	{
		using wnd_proc_t = LRESULT( __stdcall * )( HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param );

		// Use a different processing window of your own.
		// Don't forget to check ImGui_ImplWin32_WndProcHandler ( return 1 ) and call reset_device in the case WM_SIZE.
		//
		static wnd_proc_t               g_cwnd_proc{ nullptr };

		// Create Microsoft Direct3D objects and set up the environment
		//
		static LPDIRECT3D9              g_ptr_d3d{ nullptr };
		static LPDIRECT3DDEVICE9        g_ptr_d3d_device{ nullptr };
		static D3DPRESENT_PARAMETERS    g_d3d_pp{ 0 };

		static void reset_device()
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();

			if ( g_ptr_d3d_device->Reset( &g_d3d_pp ) == D3DERR_INVALIDCALL )
				IM_ASSERT( 0 );

			ImGui_ImplDX9_CreateDeviceObjects();
		}

		static LRESULT __stdcall wnd_proc( HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param )
		{
			if ( ImGui_ImplWin32_WndProcHandler( hwnd, msg, w_param, l_param ) )
				return true;

			switch ( msg )
			{
				case WM_DESTROY:
					PostQuitMessage( 0 );
					return 0;
				case WM_CLOSE:
					DestroyWindow( hwnd );
					return 0;
				case WM_SIZE:
					if ( g_ptr_d3d_device && ( w_param != SIZE_MINIMIZED ) )
					{
						g_d3d_pp.BackBufferWidth = low_order < unsigned short, LPARAM >( l_param );
						g_d3d_pp.BackBufferHeight = high_order< unsigned short, LPARAM >( l_param );
						reset_device();
					}
					return 0;
			}
			return DefWindowProcA( hwnd, msg, w_param, l_param );
		}
	}

	namespace utils
	{
		HICON icon( const int id_icon, const vec2& size, const int fu_load = LR_DEFAULTCOLOR )
		{
			return static_cast< HICON >( LoadImageA( GetModuleHandleA( nullptr ), MAKEINTRESOURCE( id_icon ), IMAGE_ICON, size.m_x, size.m_y, fu_load ) );
		}
	}

	class impl_window
	{
	protected:
		HWND        m_hwnd;
		WNDCLASSEX  m_window_class;
		std::string m_window_name;
		std::string m_class_name;

		impl_window
		(
			const std::string &window_name,                             // Window name
			const std::string &class_name,                              // Window class name
			const vec2 &window_pos,                                     // Window start position
			const vec2 &window_size,                                    // Window start size
			const uint32_t class_style,                                 // Class window style
			const uint32_t window_style,								// Window style
			const HICON icon,											// Icon
			const HICON small_icon										// Small icon
		) :
			m_hwnd{ nullptr },
			m_window_name{ window_name },
			m_class_name{ class_name }
		{
			this->m_window_class =
			{
				sizeof WNDCLASSEX,                                      // Class size
				class_style,                                            // 0x0040 - https://docs.microsoft.com/en-us/windows/win32/winmsg/window-class-styles
				dx::g_cwnd_proc ? dx::g_cwnd_proc : dx::wnd_proc,       // Customizable or current window procedure
				0,
				0,
				GetModuleHandleA( nullptr ),                            // Should be equivalent to the instance passed into WinMain
				icon,                                                   // Icon
				nullptr,
				nullptr,
				nullptr,
				m_class_name.data(),                                    // Window class name
				small_icon                                              // Small icon
			};

			if ( RegisterClassExA( &this->m_window_class ) )
			{
				this->m_hwnd = CreateWindowExA
				(
					0,                                                  // WS_EX_RIGHTSCROLLBAR - https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
					m_window_class.lpszClassName,                       // Window class name
					m_window_name.data(),                               // Window name
					window_style,                                       // WS_OVERLAPPEDWINDOW - https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles
					window_pos.m_x,                                     // Window start position X
					window_pos.m_y,                                     // Window start position Y
					window_size.m_x,                                    // Window size X
					window_size.m_y,                                    // Window size Y
					nullptr,
					nullptr,
					this->m_window_class.hInstance,                     // A handle to the instance of the module to be associated with the window.
					nullptr
				);

				if ( this->m_hwnd )
					log::log( "Handle is nullptr. CreateWindowExA failed!\n" );
			}
			else
				log::log( "RegisterClassExA returned 0!\n" );
		}
		~impl_window()
		{
			if ( !UnregisterClassA( this->m_window_class.lpszClassName, this->m_window_class.hInstance ) )
				log::log( "UnregisterClassA returned 0!" );

			this->m_hwnd = nullptr;
			this->m_window_class = { 0 };

			dx::g_d3d_pp = {};

			dx::g_cwnd_proc = nullptr;
		}
	public:
		impl_window( const impl_window & ) = delete;
		impl_window &operator=( const impl_window & ) = delete;
	};

	class impl_imgui : public impl_window
	{
	protected:
		bool m_vsync;

		impl_imgui
		(
			const std::string &window_name,
			const std::string &class_name,
			const vec2 &window_pos,
			const vec2 &window_size,
			const uint32_t class_style,
			const uint32_t window_style,
			const HICON icon,
			const HICON small_icon,
			const bool vsync
		) :
			impl_window
		{
			window_name,
			class_name,
			window_pos,
			window_size,
			class_style,
			window_style,
			icon,
			small_icon
		},
			m_vsync{ vsync }
		{
			if ( this->m_hwnd )
			{
				if ( !create_device() )
					log::log( "create_device returned false.\n" );
			}
		}

		~impl_imgui()
		{
			// ImGui does not check if the context is valid when cleaning, so it will try to access memory on a null pointer.
			//
			if ( ImGui::GetCurrentContext() )
			{
				ImGui_ImplDX9_Shutdown();
				ImGui_ImplWin32_Shutdown();

				ImGui::DestroyContext();
			}

			clear_device();
		}

		void clear_device()
		{
			if ( dx::g_ptr_d3d_device )
			{
				dx::g_ptr_d3d_device->Release();
				dx::g_ptr_d3d_device = nullptr;
			}
			if ( dx::g_ptr_d3d )
			{
				dx::g_ptr_d3d->Release();
				dx::g_ptr_d3d = nullptr;
			}
		}

		bool create_device()
		{
			dx::g_ptr_d3d = Direct3DCreate9( D3D_SDK_VERSION );

			if ( !dx::g_ptr_d3d )
				return false;

			dx::g_d3d_pp.Windowed = 1;
			dx::g_d3d_pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			dx::g_d3d_pp.BackBufferFormat = D3DFMT_UNKNOWN;
			dx::g_d3d_pp.EnableAutoDepthStencil = 1;
			dx::g_d3d_pp.AutoDepthStencilFormat = D3DFMT_D16;
			dx::g_d3d_pp.PresentationInterval = m_vsync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
			if ( dx::g_ptr_d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, this->m_hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &dx::g_d3d_pp, &dx::g_ptr_d3d_device ) < 0 )
				return false;

			return true;
		}

		void imgui_init_context()
		{
			if ( this->m_hwnd )
			{
				if ( ImGui::CreateContext() )
					ImGui::StyleColorsDark();
				else
					log::log( "CreateContext returned nullptr.\n" );
			}
		}

		void imgui_init_windx()
		{
			if ( this->m_hwnd && dx::g_ptr_d3d_device && ImGui::GetCurrentContext() )
			{
				if ( !ImGui_ImplWin32_Init( this->m_hwnd ) )
					log::log( "ImGui_ImplWin32_Init returned false.\n" );
				if ( !ImGui_ImplDX9_Init( dx::g_ptr_d3d_device ) )
					log::log( "ImGui_ImplDX9_Init returned false.\n" );
			}
		}

	public:
		impl_imgui( const impl_imgui & ) = delete;
		impl_imgui &operator=( const impl_imgui & ) = delete;

		bool imgui_start_frame()
		{
			if ( ImGui::GetCurrentContext() )
			{
				ImGui_ImplDX9_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				return true;
			}

			return false;
		}

		void imgui_end_frame()
		{
			if ( dx::g_ptr_d3d_device && ImGui::GetCurrentContext() )
			{
				ImGui::EndFrame();

				if ( dx::g_ptr_d3d_device->BeginScene() >= 0 )
				{
					ImGui::Render();
					ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData() );
					dx::g_ptr_d3d_device->EndScene();
				}
				const auto result{ dx::g_ptr_d3d_device->Present( nullptr, nullptr, nullptr, nullptr ) };

				if ( result == D3DERR_DEVICELOST && dx::g_ptr_d3d_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET )
					dx::reset_device();
			}
		}

		bool get_vsync_state() const
		{
			return m_vsync;
		}

		void set_background_color( const int r = 105, const int g = 105, const int b = 105 )
		{
			if ( dx::g_ptr_d3d_device )
			{
				dx::g_ptr_d3d_device->SetRenderState( D3DRS_ZENABLE, 0 );
				dx::g_ptr_d3d_device->SetRenderState( D3DRS_ALPHABLENDENABLE, 0 );
				dx::g_ptr_d3d_device->SetRenderState( D3DRS_SCISSORTESTENABLE, 0 );
				dx::g_ptr_d3d_device->Clear( 0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, ( ( 255 & 0xff ) << 24 ) | ( ( r & 0xff ) << 16 ) | ( ( g & 0xff ) << 8 ) | ( b & 0xff ), 1.0f, 0 );
			}
		}
	};

	class window : public impl_imgui
	{
	private:
		MSG m_msg;
	public:
		window
		(
			const std::string &window_name,
			const std::string &class_name,
			const vec2 &window_pos,
			const vec2 &window_size,
			const uint32_t cmd_show = SW_SHOW,
			const uint32_t class_style = CS_CLASSDC,
			const uint32_t window_style = WS_OVERLAPPEDWINDOW,
			const HICON icon = nullptr,
			const HICON small_icon = nullptr,
			const bool vsync = true
		) :
			impl_imgui
		{
			window_name,
			class_name,
			window_pos,
			window_size,
			class_style,
			window_style,
			icon,
			small_icon,
			vsync
		},
			m_msg{ 0 }
		{
			ShowWindow( this->m_hwnd, cmd_show );
			UpdateWindow( this->m_hwnd );

			imgui_init_context();
			imgui_init_windx();
		}
		~window()
		{
			this->m_msg = {};
		}

		bool process_message()
		{
			if ( !this->m_hwnd )
				return false;

			if ( PeekMessageA( &m_msg, nullptr, 0u, 0u, PM_REMOVE ) )
			{
				if ( m_msg.message == WM_QUIT )
					return false;

				TranslateMessage( &m_msg );
				DispatchMessageA( &m_msg );
			}

			return true;
		}

		MSG &get_msg()
		{
			return m_msg;
		}
	};
}

/*
- This header is free to use and uses the MIT license.

- It was developed based on the main.cpp of the example_win32_directx9 of ImGui.

- It was designed on top of ImGui 1.76 release and there is no guarantee of operation in other previous versions.

- It allows the user to create an interface based on Win32 - DirectX9 with ImGui.
- In a few lines of code and does not require std::c++14 or later for this.

- DirectX10, 11 and 12 but are still supported.

-> Special thanks to github.com/Darkratos and github.com/Nomade040 for their suggestions for improvements.

*/

#pragma once

// ImGui Headers
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

// DirectX includes
#include <d3d9.h>
#include <string> //std::string
#pragma comment(lib, "d3d9.lib")

// Global prototypes
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static LPDIRECT3D9              g_pd3d;
static LPDIRECT3DDEVICE9        g_pd3d_device;
static D3DPRESENT_PARAMETERS    g_d3dpp;
static LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Creates and registers a window directly from the constructor.
class Implementation_Window
{
public:
    // Enum class style preset
    enum class style_class
    {
        VREDRAW = 0x0001,
        HREDRAW = 0x0002,
        DBLCLKS = 0x0008,
        OWNDC = 0x0020,
        CLASSDC = 0x0040,
        PARENTDC = 0x0080,
        NOCLOSE = 0x0200,
        SAVEBITS = 0x0800,
        BYTEALIGNCLIENT = 0x1000,
        BYTEALIGNWINDOW = 0x2000,
        GLOBALCLASS = 0x4000
    };
protected:
    // If a m_hwnd is not nullptr then CreateWindowA has returned success and we have a window.
    HWND m_hwnd{};
    // If m_wnd_class is not nullptr, then our window structure provided by user parameters has been generated.
    WNDCLASSEX m_window_class{};

    // Builder will provide the necessary arguments for creating our window class and then the window.
    // If CreateWindowA fails then m_hwnd will be nullptr.
    Implementation_Window(const std::string& window_name,
        int width, int heigth,
        const style_class& class_style,
        const std::string& class_name,
        int initial_pos_x, int initial_pos_y) :
        m_window_class{ sizeof(WNDCLASSEX),
                        static_cast<unsigned int>(class_style),
                        WndProc,
                        0,
                        0,
                        GetModuleHandleA(nullptr),
                        nullptr, nullptr, nullptr, nullptr,
                        class_name.c_str(),
                        nullptr }
    {
        RegisterClassExA(&m_window_class);
        m_hwnd = CreateWindowExA(0LU,
                        m_window_class.lpszClassName,
                        window_name.c_str(),
                        WS_OVERLAPPEDWINDOW,
                        initial_pos_x, initial_pos_y,
                        width, heigth,
                        nullptr, nullptr,
                        m_window_class.hInstance,
                        nullptr);
    }

    // The destructor will destroy our window, invalidating m_hwnd and unregistering the window.
    ~Implementation_Window()
    {
        DestroyWindow(m_hwnd);
        UnregisterClassA(m_window_class.lpszClassName, m_window_class.hInstance);
    }
public:
    // No builders available to create a new object of type
    Implementation_Window(const Implementation_Window&) = delete;
    Implementation_Window& operator=(const Implementation_Window&) = delete;

    // Const references return to m_hwnd and m_wnd_class that can only be accessed.
    const HWND& get_hwnd() const { return m_hwnd; }
    const WNDCLASSEX& get_wnd() const { return m_window_class; }
};

// It has device controland directx functions.
class Implementation_Render : public Implementation_Window
{
protected:
    // Release the buffer via "Release()" and set the pointer to nullptr, so it is ready to use again.
    void clear_device_d3d()
    {
        if (g_pd3d_device)
        {
            g_pd3d_device->Release();
            g_pd3d_device = nullptr;
        }
        if (g_pd3d)
        {
            g_pd3d->Release();
            g_pd3d = nullptr;
        }
    }

    // Creates the device and defines vsync or not.
    bool create_device_d3d(const bool vsync = true)
    {
        if ((g_pd3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
            return false;

        // Create the D3DDevice
        ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
        g_d3dpp.Windowed = TRUE;
        g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
        g_d3dpp.EnableAutoDepthStencil = TRUE;
        g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
        // Present with vsync
        // Present without vsync, maximum unthrottled framerate
        // The boolean vsync define this.
        g_d3dpp.PresentationInterval = (vsync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE);
        if (g_pd3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3d_device) < 0)
            return false;

        return true;
    }

    // The builder is responsible for calling the "Implementation_Window" builder that will create a window.
    Implementation_Render(const std::string& window_name,
        int width, int heigth,
        const style_class& class_style,
        const std::string& class_name,
        int initial_pos_x, int initial_pos_y) :
        Implementation_Window(window_name,
                         width, heigth,
                         class_style,
                         class_name,
                         initial_pos_x, initial_pos_y)
    {
        // Then the device is created and the window updated.
        // If create_window_d3d fails then the device is cleared.
        if (!create_device_d3d())
        {
            clear_device_d3d();
        }

        ShowWindow(m_hwnd, SW_SHOWDEFAULT);
        UpdateWindow(m_hwnd);
    }

    // Clear device
    ~Implementation_Render()
    {
        clear_device_d3d();
    }
public:
    //No builders available to create a new object of type
    Implementation_Render(const Implementation_Render&) = delete;
    Implementation_Render& operator=(const Implementation_Render&) = delete;

    // The reset device is a static function because WindowProc uses it
    static void reset_device();
};

//const std::string& window_name
//int width = 1280
//int heigth = 800
//const style_class& class_style = style_class::CLASSDC
//const std::string& class_name = "ImGui Basic Setup"
//int initial_x = 10
//int initial_y = 10
class Implementation final : public Implementation_Render
{
public:
    //Enum Imgui preset set color style
    enum class style_color
    {
        CLASSIC,
        DARK,
        LIGHT
    };
private:
    // MSG struct to while loop main.cpp
    MSG m_msg{};
public:
    //window_name is required.
    Implementation(const std::string& window_name,
        int width = 1280, int heigth = 800,
        const style_class& class_style = style_class::CLASSDC,
        const std::string& class_name = "ImGui Basic Setup",
        int initial_pos_x = 10, int initial_pos_y = 10) :
        Implementation_Render(window_name,
                         width, heigth,
                         class_style,
                         class_name,
                         initial_pos_x, initial_pos_y)
    {
        // Initializes the context of imgui, then implements Win32 and DirectX and sets the color style to classic.
        setup_imgui_context();

        memset(&m_msg, 0, sizeof(m_msg));
    }

    // ImGui Destroy
    ~Implementation()
    {
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    // Toggles color style simply by making a call and passing a member of the enum style_color
    void set_style(const style_color& style) const
    {
        switch (style)
        {
        case style_color::CLASSIC:
            ImGui::StyleColorsClassic();
            break;
        case style_color::DARK:
            ImGui::StyleColorsDark();
            break;
        case style_color::LIGHT:
            ImGui::StyleColorsLight();
            break;
        default:
            ImGui::StyleColorsClassic();
            break;
        }
    }

    // A wrapper for the background color of the window.
    // If no arguments are passed it will be black.
    void set_background_color(const int r = 0, const int g = 0, const int b = 0)
    {
        g_pd3d_device->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3d_device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA(r, g, b, 255);
        g_pd3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
    }

    // Initializes the context of imgui, then implements Win32 and DirectX and sets the color style to classic.
    // get_hwnd() returns a reference to the m_hwnd variable
    void setup_imgui_context()
    {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsClassic();

        assert(ImGui_ImplWin32_Init(get_hwnd()) && "ImGui Context: failed ImplWin32!");
        assert(ImGui_ImplDX9_Init(g_pd3d_device) && "ImGui Context: failed ImplDX9!");
    }

    // A wrapper for the New Frame
    void start_frame()
    {
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    // A wrapper for the End Frame.
    // And also the device reset.
    void end_frame()
    {
        ImGui::EndFrame();
        if (g_pd3d_device->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3d_device->EndScene();
        }
        HRESULT result = g_pd3d_device->Present(NULL, NULL, NULL, NULL);

        if (result == D3DERR_DEVICELOST && g_pd3d_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            reset_device();
    }

    // Returns a reference to our variable m_msg;
    // get_msg().message for example.
    MSG& get_msg() { return m_msg; }
};

// Reset the device
void Implementation_Render::reset_device()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3d_device->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3d_device != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            Implementation_Render::reset_device();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hWnd, msg, wParam, lParam);
}
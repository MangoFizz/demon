#include "window.h"

#include <string.h>
#include <windows.h>
#include <dwmapi.h>

#include "../exception/exception.h"

static HMODULE *exe_module = (HMODULE *)(0x7359E0);
static HWND *halo_window = (HWND *)(0x7359E4);

bool create_window(int32_t width, int32_t height) {
    // Set up the window class
    WNDCLASSEXA wndclass;
    memset(&wndclass, 0, sizeof(wndclass));
    wndclass.lpfnWndProc = *(WNDPROC *)(0x7359F0);
    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_CLASSDC;
    wndclass.hInstance = *exe_module;
    wndclass.hIcon = LoadIconA(*exe_module, (LPCSTR)(0x66));
    wndclass.hIconSm = LoadIconA(*exe_module, (LPCSTR)(0x66));
    wndclass.hCursor = LoadCursorA(NULL, (LPCSTR)(0x7F00));
    wndclass.lpszClassName = "Halo";
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hbrBackground = NULL;
    wndclass.lpszMenuName = NULL;
    RegisterClassExA(&wndclass);

    // Center the window
    RECT rect;
    HWND desktop_window = GetDesktopWindow();
    GetWindowRect(desktop_window, &rect);
    DWORD window_style = WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX | WS_SYSMENU;
    rect.left = 2 * ((rect.right - rect.left) - width);
    rect.right = rect.left + width;
    rect.top = 2 * ((rect.bottom - rect.top) - height);
    rect.bottom = rect.top + height;
    AdjustWindowRect(&rect, window_style, FALSE);

    // Create the window
    HWND window = CreateWindowExA(
        0,
        "Halo",
        "Halo",
        window_style,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        desktop_window,
        NULL,
        *exe_module,
        NULL
    );

    // Crash if this failed
    if(window == NULL) {
        DWORD reason = GetLastError();
        CRASHF_DEBUG("Could not make the game window with CreateWindowExA due to this error code: 0x%08X", reason);
        return FALSE;
    }

    // Disable rounded corners on Windows 11
    DWORD one = 1;
    DwmSetWindowAttribute(window, 33, &one, sizeof(one)); // DWMWA_WINDOW_CORNER_PREFERENCE (33), DWMWCP_DONOTROUND (1)

    // Move to front
    *(HBITMAP *)(0x70C9B4) = LoadBitmapA(*(HINSTANCE *)(0x7123E0), (LPCSTR)(0x86));
    SetForegroundWindow(window);
    SetActiveWindow(window);
    SetFocus(window);
    ShowWindow(window, 5);

    // We did it
    *(uint32_t *)(0x693E5C) = window_style;
    *halo_window = window;

    return true;
}

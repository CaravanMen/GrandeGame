#include "platform.h"
#include "schnitzel_lib.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

// #######################################################
//					Windows Globals
// #######################################################
static HWND window;

// Default window callback
LRESULT windows_window_callback(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (msg) {
	case WM_CLOSE:
		running = false;
		break;
	default:
		// Let windows handle the default input for now
		result = DefWindowProcA(window, msg, wParam, lParam);
		break;
	}
	return result;
}
// Creating the window
bool platform_create_window(int width, int height, char* title) {
	HINSTANCE instance = GetModuleHandleA(0);
	WNDCLASSA wc = {};
	wc.hInstance = instance;
	wc.hIcon = LoadIcon(instance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(instance, IDC_ARROW);
	wc.lpszClassName = title;
	wc.lpfnWndProc = windows_window_callback;

	if (!RegisterClassA(&wc)) {
		return false;
	}

	int dwStyle = WS_OVERLAPPEDWINDOW;
	window = CreateWindowExA(0, title, title, dwStyle, 100, 100, width, height, NULL, NULL, instance, NULL);

	if (window == NULL) {
		return false;
	}
	ShowWindow(window, SW_SHOW);

	return true;
}
// Window event update
void platform_update_window() {
	MSG msg;
	while (PeekMessageA(&msg, window, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageA(&msg); // Calls the callback when creating the window
	}
}

void* platform_load_gl_function(char* funName)
{
	PROC proc = wglGetProcAddress(funName);
	if (!proc)
	{
		static HMODULE opengl32DLL = LoadLibraryA("opengl32.dll");
		proc = GetProcAddress(opengl32DLL, funName);
		if (!proc) {
			SM_ASSERT(false, "Failed to load gl function %s", "glCreateProgram");
			return nullptr;
		}
	}

}
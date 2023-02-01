///
/// create a ui project for mqtt client
///
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <windows.h>

#include "user_lib.h"
#include "resource.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment( lib, "Comctl32.lib" )
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif




/*-----------------------------------------------------------------------------------*/
//message process group
void onCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CREATESTRUCTA* pcs = (CREATESTRUCTA*)lParam;
	//create a controls container
	user_create_client_wnd(pcs->hInstance,SW_NORMAL, hWnd);

}
void onSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = LOWORD(lParam);
	rect.bottom = HIWORD(lParam);
	//resize client wnd
	clientwnd_response_resize(GetDlgItem(hWnd, ID_CLIENTWND1), rect);

}
void onClose(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int ret = 0;
	ret = MessageBoxA(hWnd, "Are you sure Exit ?", "Attention", MB_YESNO | MB_ICONQUESTION);
	if (ret == IDYES) {
		DestroyWindow(hWnd);
	}
	else {
		XDEBUG("exit cancel\n");
	}
}
void onDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//post quit
	PostQuitMessage(0);
}
//message process func.
LRESULT CALLBACK main_wndproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		onCreate(hWnd, message, wParam, lParam);
		break;
	case WM_SIZE:
		onSize(hWnd, message, wParam, lParam);
		break;
	case WM_CLOSE:
		onClose(hWnd, message, wParam, lParam);
		break;
	case WM_DESTROY:
		onDestroy(hWnd, message, wParam, lParam);
		break;

	case WM_CONTEXTMENU:
		break;

	default:
		return DefWindowProcA(hWnd, message, wParam, lParam);
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/
int user_register_class(HINSTANCE hInstance)
{
	WNDCLASSA wca;

	wca.style = CS_HREDRAW | CS_VREDRAW;
	wca.lpfnWndProc = main_wndproc;
	wca.cbClsExtra = 0;
	wca.cbWndExtra = 0;
	wca.hInstance = hInstance;
	wca.hIcon = 0;
	wca.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wca.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wca.lpszMenuName = MAKEINTRESOURCEA(IDR_MENU1);
	wca.lpszClassName = "main_wndclass";

	return RegisterClassA(&wca);
}
int user_init_instance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hWnd = CreateWindowA("main_wndclass", "MQTT", WS_OVERLAPPEDWINDOW,100, 100, 1920, 1080, NULL, (HMENU)0, hInstance, nullptr);
	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}
int main_message_loop(void)
{
	MSG msg;

	while (GetMessageA(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return (int)msg.wParam;
}
//
// window app entry point function
//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	user_debug_init();

	if (user_register_class(hInstance) == 0)
	{
		XDEBUG("user_register_class failed\n");
		return 400;
	}
	else {
		XDEBUG("user_register_class ok\n");
	}

	if (user_init_instance(hInstance, nShowCmd) == 0)
	{
		XDEBUG("user_init_instance failed\n");
		return 400;
	}
	else {
		XDEBUG("user_init_instance ok\n");
	}

	//main message loop.
	return main_message_loop();
}

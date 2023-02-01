#pragma once

#include <windows.h>



void user_create_client_wnd(HINSTANCE hInstance, int nCmdShow, HWND hParent);
int clientwnd_response_resize(HWND hWnd, RECT rec);
void user_put_message_on_control(char* topic, char* data, int datalen);
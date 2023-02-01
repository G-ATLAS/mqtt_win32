#define WIN32_LEAN_AND_MEAN
#include "user_lib.h"

#include "commctrl.h"
#include "windowsx.h"
#include <windows.h>
#include <winsock2.h>
#include "user_mqtthub.h"

#include <stdlib.h>

//gloal var
HWND g_hclientwnd = NULL;




LRESULT CALLBACK gp_bypass_wndproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_COMMAND || message == WM_NOTIFY)
	{
		//relay message
	  return SendMessage(GetParent(hWnd), message, wParam, lParam);
	}
	return CallWindowProc((WNDPROC)GetWindowLongPtr(hWnd,GWLP_USERDATA),hWnd, message, wParam, lParam);
}
static void onCreate_mqttinit(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Initialize Winsock
	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		XDEBUG("WSAStartup failed with error\n");
	}
	user_mqtt_init();
}

static void onCreate_controls(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//as an example
	//HWND hwnd = nullptr;
	//hwnd = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Button", "Example", BS_PUSHBUTTON | BS_TEXT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 555, 555, 100, 30, hWnd, (HMENU)0, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);

	HWND hwnd = nullptr;
	HINSTANCE hinst = nullptr;
	hinst = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
	//create begin
	//Broker Setting group
	HWND hwndgrp = nullptr;
	hwndgrp = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Button", "Broker Setting", BS_GROUPBOX | BS_TEXT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 5, 5, 450, 150, hWnd, (HMENU)ID_GP_BS, hinst, nullptr);
	//group item
	hwnd = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Static", "Broker Address:", SS_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 20, 25, 150,25, hwndgrp, (HMENU)0, hinst, nullptr);
	hwnd = CreateWindowExA(WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,"Edit",    "", ES_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 170, 25, 250, 30, hwndgrp, (HMENU)ID_GP_BS_ED_BADD, hinst, nullptr);
	//group item
	hwnd = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Static", "Broker Port:", SS_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 20, 65, 150, 25, hwndgrp, (HMENU)0, hinst, nullptr);
	hwnd = CreateWindowExA(WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Edit", "", ES_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 170, 65, 250, 30, hwndgrp, (HMENU)ID_GP_BS_ED_BPORT, hinst, nullptr);
	//group item
	hwnd = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Static", "Client ID:", SS_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 20, 105, 150, 25, hwndgrp, (HMENU)0, hinst, nullptr);
	hwnd = CreateWindowExA(WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Edit", "", ES_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 170, 105, 250, 30, hwndgrp, (HMENU)ID_GP_BS_ED_CID, hinst, nullptr);

	//User Credentials group
	hwndgrp = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Button", "User Credentials", BS_GROUPBOX | BS_TEXT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, (5+455), 5, 450, 150, hWnd, (HMENU)ID_GP_UC, hinst, nullptr);
	//group item
	hwnd = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Static", "User Name:", SS_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 20, 25, 150, 25, hwndgrp, (HMENU)0, hinst, nullptr);
	hwnd = CreateWindowExA(WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Edit", "", ES_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 170, 25, 250, 30, hwndgrp, (HMENU)ID_GP_UC_ED_UNAME, hinst, nullptr);
	//group item
	hwnd = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Static", "Password:", SS_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 20, 65, 150, 25, hwndgrp, (HMENU)0, hinst, nullptr);
	hwnd = CreateWindowExA(WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Edit", "", ES_LEFT | ES_PASSWORD | WS_CHILD | WS_VISIBLE, 170, 65, 250, 30, hwndgrp, (HMENU)ID_GP_UC_ED_UPASS, hinst, nullptr);

	//User Operations group
	hwndgrp = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Button", "User Operations", BS_GROUPBOX | BS_TEXT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, (5 + 455 + 455), 5, 450, 150, hWnd, (HMENU)ID_GP_UO, hinst, nullptr);
	//group item
	hwnd = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Button", "Connect", BS_PUSHBUTTON | BS_TEXT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 100, 25, 200, 30, hwndgrp, (HMENU)ID_GP_UO_BN_CON, hinst, nullptr);
	hwnd = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Button", "Disconnect", BS_PUSHBUTTON | BS_TEXT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 100, 65, 200, 30, hwndgrp, (HMENU)ID_GP_UO_BN_DISCON, hinst, nullptr);
	hwnd = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Button", "Status:Break", BS_PUSHBUTTON | BS_TEXT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 100, 105, 200, 30, hwndgrp, (HMENU)ID_GP_UO_BN_STATUS, hinst, nullptr);

	
	//publish group
	hwndgrp = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Button", "Publish", BS_GROUPBOX | BS_TEXT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, (5), (5+170), 450, 500, hWnd, (HMENU)ID_GP_PUB, hinst, nullptr);
	//group item
	hwnd = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Static", "To Topic:", SS_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 20, 25, 150, 25, hwndgrp, (HMENU)0, hinst, nullptr);
	hwnd = CreateWindowExA(WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Edit", "", ES_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 170, 25, 250, 30, hwndgrp, (HMENU)ID_GP_PUB_ED_TOPIC, hinst, nullptr);
	//group item
	hwnd = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Static", "Data to Send:", SS_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 20, 65, 150, 25, hwndgrp, (HMENU)0, hinst, nullptr);
	hwnd = CreateWindowExA(WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Edit", "", ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL, 170, 65, 250, 350, hwndgrp, (HMENU)ID_GP_PUB_ED_DATA, hinst, nullptr);
	//group item
	hwnd = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Button", "Publish", BS_PUSHBUTTON | BS_TEXT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 170, 430, 250, 30, hwndgrp, (HMENU)ID_GP_PUB_BN_PUB, hinst, nullptr);


	//subcribe group
	hwndgrp = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Button", "Subcribe", BS_GROUPBOX | BS_TEXT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, (5 + 455), (5 + 170), (450+5+450), 500, hWnd, (HMENU)ID_GP_SUB, hinst, nullptr);
	//group item
	hwnd = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Static", "Get Topic:", SS_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 20, 25, 150, 25, hwndgrp, (HMENU)0, hinst, nullptr);
	hwnd = CreateWindowExA(WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Edit", "", ES_LEFT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 170, 25, 250, 30, hwndgrp, (HMENU)ID_GP_SUB_ED_TOPIC, hinst, nullptr);
	hwnd = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Button", "Subcribe", BS_PUSHBUTTON | BS_TEXT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 430, 25, 160, 30, hwndgrp, (HMENU)ID_GP_SUB_BN_SUB, hinst, nullptr);
	hwnd = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Button", "Unsub", BS_PUSHBUTTON | BS_TEXT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 595, 25, 160, 30, hwndgrp, (HMENU)ID_GP_SUB_BN_UNSUB, hinst, nullptr);
	//group item
	hwnd = CreateWindowExA(WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "ListBox", "", LBS_NOTIFY | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL, 20, 65, 430, 300, hwndgrp, (HMENU)ID_GP_SUB_LB_SUBED, hinst, nullptr);
	hwnd = CreateWindowExA(WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "ListBox", "", LBS_NOTIFY | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL, 455, 65, 430, 430, hwndgrp, (HMENU)ID_GP_SUB_LB_HISTORY, hinst, nullptr);
	//group item
	hwnd = CreateWindowExA(WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, "Edit", "", ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL, 20, 360, 430, 130, hwndgrp, (HMENU)ID_GP_SUB_ED_RECNOW, hinst, nullptr);

	//message bypass groupbox
	LONG_PTR handler ,newhandler;
	newhandler = (LONG_PTR)gp_bypass_wndproc;
	//after save old update new
	hwndgrp = GetDlgItem(hWnd, ID_GP_BS);
	handler = GetWindowLongPtr(hwndgrp,GWLP_WNDPROC);
	SetWindowLongPtr(hwndgrp,GWLP_USERDATA, handler);
	SetWindowLongPtr(hwndgrp, GWLP_WNDPROC, newhandler);

	hwndgrp = GetDlgItem(hWnd, ID_GP_UC);
	handler = GetWindowLongPtr(hwndgrp, GWLP_WNDPROC);
	SetWindowLongPtr(hwndgrp, GWLP_USERDATA, handler);
	SetWindowLongPtr(hwndgrp, GWLP_WNDPROC, newhandler);

	hwndgrp = GetDlgItem(hWnd, ID_GP_UO);
	handler = GetWindowLongPtr(hwndgrp, GWLP_WNDPROC);
	SetWindowLongPtr(hwndgrp, GWLP_USERDATA, handler);
	SetWindowLongPtr(hwndgrp, GWLP_WNDPROC, newhandler);

	hwndgrp = GetDlgItem(hWnd, ID_GP_PUB);
	handler = GetWindowLongPtr(hwndgrp, GWLP_WNDPROC);
	SetWindowLongPtr(hwndgrp, GWLP_USERDATA, handler);
	SetWindowLongPtr(hwndgrp, GWLP_WNDPROC, newhandler);

	hwndgrp = GetDlgItem(hWnd, ID_GP_SUB);
	handler = GetWindowLongPtr(hwndgrp, GWLP_WNDPROC);
	SetWindowLongPtr(hwndgrp, GWLP_USERDATA, handler);
	SetWindowLongPtr(hwndgrp, GWLP_WNDPROC, newhandler);


	//more here


	//style it
	//GetWindowLongPtr();




	//test a bit
	//hwnd = GetDlgItem(hWnd, ID_GP_SUB);
	//hwnd = GetDlgItem(hwnd, ID_GP_SUB_LB_SUBED);
	//if (hwnd)
	//{
	//	XDEBUG("get handle ok %x\n", hwnd);

	//}
	//else { XDEBUG("get handle not ok\n"); }

	//if (hwnd == NULL) return;
	//TCHAR tarray[100];
	//for (int i = 0; i < 50; i++)
	//{ 
	//	wsprintf(tarray, TEXT("Item %02d"), i);
	//	ListBox_AddItemData(hwnd, tarray);
	//}
	////scroll to the bottom
	//ListBox_SetTopIndex(hwnd, ListBox_GetCount(hwnd)-1);
}
BOOL CALLBACK EN_DIS_ChildProc(HWND   hwnd, LPARAM lParam)
{
	int onoff = (int)lParam;
	EnableWindow(hwnd, onoff);

	return TRUE;
}
//botton click event handler here
static void btn_connect_onclick(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	XDEBUG("connect  clicked!\n");
	//get some keyinfo from edit controls
	char address[60];
	char port[60];
	char clientid[60];
	char uname[60];
	char upass[60];
	int iport = 1883;

	HWND hWndgp = GetDlgItem(hWnd, ID_GP_BS);
	HWND hWnditem = GetDlgItem(hWndgp, ID_GP_BS_ED_BADD);
	GetWindowTextA(hWnditem, address,60);

	hWnditem = GetDlgItem(hWndgp, ID_GP_BS_ED_BPORT);
	GetWindowTextA(hWnditem, port, 60);

	hWnditem = GetDlgItem(hWndgp, ID_GP_BS_ED_CID);
	GetWindowTextA(hWnditem, clientid, 60);

	hWndgp = GetDlgItem(hWnd, ID_GP_UC);
	hWnditem = GetDlgItem(hWndgp, ID_GP_UC_ED_UNAME);
	GetWindowTextA(hWnditem, uname, 60);

	hWnditem = GetDlgItem(hWndgp, ID_GP_UC_ED_UPASS);
	GetWindowTextA(hWnditem, upass, 60);

	//port 
	iport = atoi(&port[0]);

	//string out
	//XDEBUG("address: %s\n", &address[0]);
	//XDEBUG("port: %s\n", &port[0]);
	//XDEBUG("clientid: %s\n", &clientid[0]);
	//XDEBUG("uname: %s\n", &uname[0]);
	//XDEBUG("upass: %s\n", &upass[0]);
	//XDEBUG("iport: %d\n", iport);


	//try to connect to the server. if ok disable edit and self, if fail do nothing.
	if (user_mqtt_try_con(&address[0],iport, &clientid[0], &uname[0], &upass[0]) == true)
	{
		//ok
		HWND heditparent = GetDlgItem(hWnd, ID_GP_BS);
		EnableWindow(GetDlgItem(heditparent, ID_GP_BS_ED_BADD), 0);
		EnableWindow(GetDlgItem(heditparent, ID_GP_BS_ED_BPORT), 0);
		EnableWindow(GetDlgItem(heditparent, ID_GP_BS_ED_CID), 0);
		heditparent = GetDlgItem(hWnd, ID_GP_UC);
		EnableWindow(GetDlgItem(heditparent, ID_GP_UC_ED_UNAME), 0);
		EnableWindow(GetDlgItem(heditparent, ID_GP_UC_ED_UPASS), 0);

		//unlock pair
		heditparent = GetDlgItem(hWnd, ID_GP_UO);
		EnableWindow(GetDlgItem(heditparent, ID_GP_UO_BN_DISCON), 1);
		EnableWindow((HWND)lParam, 0);

		//tips
		HWND hparent = GetDlgItem(hWnd, ID_GP_UO);
		hparent = GetDlgItem(hparent, ID_GP_UO_BN_STATUS);
		const char* tips = "con-ok";
		SetWindowTextA(hparent, tips);

		//sub pub gp enable
		EnumChildWindows(GetDlgItem(hWnd, ID_GP_PUB), EN_DIS_ChildProc, (LPARAM)1);
		EnumChildWindows(GetDlgItem(hWnd, ID_GP_SUB), EN_DIS_ChildProc, (LPARAM)1);
	}
	else
	{
		//fail
		HWND hparent = GetDlgItem(hWnd, ID_GP_UO);
		hparent = GetDlgItem(hparent, ID_GP_UO_BN_STATUS);
		const char* tips = "fail-retry";
		SetWindowTextA(hparent, tips);
	}
}
static void btn_discon_onclick(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	XDEBUG("disconnect  clicked!\n");
	//disconnect to server, and enable the edit controls so that we can modify them.
	if (user_mqtt_try_discon() == true)
	{
		//ok
		HWND heditparent = GetDlgItem(hWnd, ID_GP_BS);
		EnableWindow(GetDlgItem(heditparent, ID_GP_BS_ED_BADD), 1);
		EnableWindow(GetDlgItem(heditparent, ID_GP_BS_ED_BPORT), 1);
		EnableWindow(GetDlgItem(heditparent, ID_GP_BS_ED_CID), 1);
		heditparent = GetDlgItem(hWnd, ID_GP_UC);
		EnableWindow(GetDlgItem(heditparent, ID_GP_UC_ED_UNAME), 1);
		EnableWindow(GetDlgItem(heditparent, ID_GP_UC_ED_UPASS), 1);
		//unlock pair
		heditparent = GetDlgItem(hWnd, ID_GP_UO);
		EnableWindow(GetDlgItem(heditparent, ID_GP_UO_BN_CON), 1);
		EnableWindow((HWND)lParam, 0);

		HWND hparent = GetDlgItem(hWnd, ID_GP_UO);
		hparent = GetDlgItem(hparent, ID_GP_UO_BN_STATUS);
		const char* tips = "discon-ok";
		SetWindowTextA(hparent, tips);
		//clear sub area
		hparent = GetDlgItem(hWnd, ID_GP_SUB);
		SetWindowTextA(GetDlgItem(hparent, ID_GP_SUB_ED_RECNOW), "");
		::SendMessageA(GetDlgItem(hparent, ID_GP_SUB_LB_SUBED), LB_RESETCONTENT, 0, (LPARAM)0);
		::SendMessageA(GetDlgItem(hparent, ID_GP_SUB_LB_HISTORY), LB_RESETCONTENT, 0, (LPARAM)0);

		//sub pub gp disable
		EnumChildWindows(GetDlgItem(hWnd, ID_GP_PUB), EN_DIS_ChildProc, (LPARAM)0);
		EnumChildWindows(GetDlgItem(hWnd, ID_GP_SUB), EN_DIS_ChildProc, (LPARAM)0);
	}
	else
	{
		//fail
		//fail
		HWND hparent = GetDlgItem(hWnd, ID_GP_UO);
		hparent = GetDlgItem(hparent, ID_GP_UO_BN_STATUS);
		const char* tips = "discon-fail";
		SetWindowTextA(hparent, tips);
	}
}
static void btn_status_onclick(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	XDEBUG("status  clicked!\n");
}
static void btn_publish_onclick(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	XDEBUG("pub  clicked!\n");

	char* topic = NULL;
	char* payload = NULL;
	int topiclen = 0;
	int payloadlen = 0;
	int rtpayloadlen = 0;

	HWND hwndgp;
	HWND htopic;
	HWND hdata;
	hwndgp = GetDlgItem(hWnd, ID_GP_PUB);
	htopic = GetDlgItem(hwndgp, ID_GP_PUB_ED_TOPIC);
	hdata = GetDlgItem(hwndgp, ID_GP_PUB_ED_DATA);
	//get length
	topiclen = GetWindowTextLengthA(htopic);
	payloadlen = GetWindowTextLengthA(hdata);
	//alloc m
	topic =(char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, topiclen+2);
	payload = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, payloadlen+2);
	//check m
	if (topic == NULL || payload == NULL)
	{
		XDEBUG("btn_publish_onclick mem fail");
		return;
	}
	
	//get stuff
	rtpayloadlen = GetWindowTextA(hdata, payload, payloadlen + 2);
	GetWindowTextA(htopic, topic, topiclen + 2);
	//string out
	XDEBUG("topic:%s\n", topic);
	XDEBUG("data:%s\n", payload);

	//pub
	if (user_mqtt_try_pub(&mq_client, topic, payload, payloadlen) == true)
	{
		XDEBUG("publish ok\n");
	}
	else
	{
		XDEBUG("publish fail\n");
		err_discon_action();//
	}

	//release  whatever
	HeapFree(GetProcessHeap(), 0, topic);
	HeapFree(GetProcessHeap(), 0, payload);
}
static void btn_subscribe_onclick(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	XDEBUG("sub  clicked!\n");
	char* topic= NULL;
	int topiclen = 0;
	HWND hwnd = GetDlgItem(GetParent((HWND)lParam), ID_GP_SUB_ED_TOPIC);
	//get length and alloc mem
	topiclen = GetWindowTextLengthA(hwnd);
	topic = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, topiclen + 2);
	if (topic == NULL)
	{
		XDEBUG("btn_subscribe_onclick mem fail");
		return;
	}
	GetWindowTextA(hwnd, topic, topiclen + 2);
	//out string
	XDEBUG("topic to sub:%s\n", topic);

	//try to sub
	if (user_mqtt_try_sub(&mq_client, topic, 1) == true)
	{
		//success
		XDEBUG("subcribe ok\n");
		hwnd = GetDlgItem(GetParent((HWND)lParam), ID_GP_SUB_LB_SUBED);
		::SendMessageA(hwnd, LB_ADDSTRING, 0, (LPARAM)topic);
		//scroll to the bottom
		ListBox_SetTopIndex(hwnd, ListBox_GetCount(hwnd) - 1);
	}
	else
	{
		//fail
		XDEBUG("subcribe fail\n");
		err_discon_action();//
	}

	//whatever free mem
	HeapFree(GetProcessHeap(),0, topic);
}
void user_put_message_on_control(char * topic,char *data,int datalen)
{
	HWND hwndgp = GetDlgItem(g_hclientwnd, ID_GP_SUB);
	HWND hwnded = GetDlgItem(hwndgp, ID_GP_SUB_ED_RECNOW);
	HWND hwndlb = GetDlgItem(hwndgp, ID_GP_SUB_LB_HISTORY);
	char* buf = NULL;
	size_t buflen = 0;

	buflen = strlen(topic) + datalen;
	buf = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, buflen + 20);
	if (buf == NULL) {
		XDEBUG("user_put_message_on_control mem fail\n");
		return;
	}
	sprintf_s(buf, (buflen + 20), "%s :: %s", topic, data);

	//update message to current receive box
	SetWindowTextA(hwnded, buf);
	
	//add message into listbox
	if (ListBox_GetCount(hwndlb) < 100)
	{
		//just add
		::SendMessageA(hwndlb, LB_ADDSTRING, 0, (LPARAM)buf);
		//scroll to the bottom
		ListBox_SetTopIndex(hwndlb, ListBox_GetCount(hwndlb) - 1);
	}
	else
	{
		//overflow one,add one,so constant
		//delete old one
		::SendMessageA(hwndlb, LB_DELETESTRING, (WPARAM)0, 0);
		//add one
		::SendMessageA(hwndlb, LB_ADDSTRING, 0, (LPARAM)buf);
		//scroll to the bottom
		ListBox_SetTopIndex(hwndlb, ListBox_GetCount(hwndlb) - 1);
	}

	//free mem
	HeapFree(GetProcessHeap(), 0, buf);
}
static void btn_unsubscribe_onclick(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	XDEBUG("unsub  clicked!\n");

	LONG64 selindex = 0;
	LONG64 sellen = 0;
	char* selstr = NULL;
	HWND hwnd = GetDlgItem(GetParent((HWND)lParam), ID_GP_SUB_LB_SUBED);
	//get index
	selindex = ::SendMessageA(hwnd, LB_GETCURSEL, 0, 0);
	if (selindex == LB_ERR)
	{
		XDEBUG("no item selected\n");
		//invalid unsub
		return;
	}
	XDEBUG("item selected\n");
	sellen = ::SendMessageA(hwnd, LB_GETTEXTLEN, (WPARAM)selindex, 0);
	selstr = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sellen + 2);
	if (selstr == NULL)
	{
		XDEBUG("btn_unsubscribe_onclick mem fail\n");
		return;
	}
	//get text into buffer
	::SendMessageA(hwnd, LB_GETTEXT, (WPARAM)selindex, (LPARAM)selstr);

	//all fine and update edit box
	HWND hwnded = GetDlgItem(GetParent((HWND)lParam), ID_GP_SUB_ED_TOPIC);
	SetWindowTextA(hwnded, selstr);


	//try unsub
	if (user_mqtt_try_unsub(&mq_client, selstr) == true)
	{
		XDEBUG("unsub ok\n");
		//delete the item from listbox
		::SendMessageA(hwnd, LB_DELETESTRING, (WPARAM)selindex, 0);
	}
	else
	{
		XDEBUG("unsub fail\n");
	}

	//whatever free mem
	HeapFree(GetProcessHeap(), 0, selstr);
}
static void link_command_action(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		//botton below
	case ID_GP_UO_BN_CON:
		if (HIWORD(wParam) == BN_CLICKED)
			btn_connect_onclick(hWnd, message, wParam, lParam);
		break;
	case ID_GP_UO_BN_DISCON:
		if (HIWORD(wParam) == BN_CLICKED)
			btn_discon_onclick(hWnd, message, wParam, lParam);
		break;
	case ID_GP_UO_BN_STATUS:
		if (HIWORD(wParam) == BN_CLICKED)
			btn_status_onclick(hWnd, message, wParam, lParam);
		break;
	case ID_GP_PUB_BN_PUB:
		if (HIWORD(wParam) == BN_CLICKED)
			btn_publish_onclick(hWnd, message, wParam, lParam);
		break;
	case ID_GP_SUB_BN_SUB:
		if (HIWORD(wParam) == BN_CLICKED)
			btn_subscribe_onclick(hWnd, message, wParam, lParam);
		break;
	case ID_GP_SUB_BN_UNSUB:
		if (HIWORD(wParam) == BN_CLICKED)
			btn_unsubscribe_onclick(hWnd, message, wParam, lParam);
		break;
		//edit below
	case ID_GP_BS_ED_BADD:
	case ID_GP_BS_ED_BPORT:
	case ID_GP_BS_ED_CID:
	case ID_GP_UC_ED_UNAME:
	case ID_GP_UC_ED_UPASS:
	case ID_GP_PUB_ED_TOPIC:
	case ID_GP_PUB_ED_DATA:
	case ID_GP_SUB_ED_TOPIC:
	case ID_GP_SUB_ED_RECNOW:
		if (HIWORD(wParam) == EN_CHANGE)
		{
			CHAR cbuf[50];
			GetWindowTextA((HWND)lParam, cbuf, 50);
			XDEBUG("edit-change ID-%d:%s\n", LOWORD(wParam), cbuf);
		}
		break;

	default:
		DefWindowProcA(hWnd, message, wParam, lParam);
		break;
	}
}

/*-----------------------------------------------------------------------------------*/
//message process group
static void onCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	INITCOMMONCONTROLSEX ice;
	ice.dwSize = sizeof(INITCOMMONCONTROLSEX);
	ice.dwICC = ICC_STANDARD_CLASSES;
	
	if (InitCommonControlsEx(&ice))
		XDEBUG("InitCommonControlsEx ok\n");
	else
		XDEBUG("InitCommonControlsEx not ok\n");

	//create content
	onCreate_controls(hWnd, message, wParam, lParam);

	//
	onCreate_mqttinit(hWnd, message, wParam, lParam);
}
static void onCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	link_command_action(hWnd, message, wParam, lParam);
}
static void onNotify(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

}
LRESULT CALLBACK client_wndproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		onCreate(hWnd, message, wParam, lParam);
		break;
	case  WM_COMMAND:
		onCommand(hWnd, message, wParam, lParam);
		break;
	case  WM_NOTIFY:
		onNotify(hWnd, message, wParam, lParam);
		break;

	case WM_CLOSE:
		break;
	case WM_DESTROY:
		break;

	default:
		return DefWindowProcA(hWnd, message, wParam, lParam);
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/
int register_clientwnd_class(HINSTANCE hInstance)
{
	WNDCLASSA wca;

	wca.style = CS_HREDRAW | CS_VREDRAW;
	wca.lpfnWndProc = client_wndproc;
	wca.cbClsExtra = 0;
	wca.cbWndExtra = 0;
	wca.hInstance = hInstance;
	wca.hIcon = 0;
	wca.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wca.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wca.lpszMenuName = nullptr;
	wca.lpszClassName = "client_wndclass";

	return RegisterClassA(&wca);
}
int clientwnd_init_instance(HINSTANCE hInstance, int nCmdShow, HWND hParent)
{
	HWND hWnd;

	RECT rec;

	GetClientRect(hParent, &rec);
	int x = 0;
	int y = 0;
	int w = rec.right - rec.left;
	int h = rec.bottom - rec.top;

	hWnd = CreateWindowA("client_wndclass", "client", WS_CHILD, x, y, w, h, hParent, (HMENU)ID_CLIENTWND1, hInstance, nullptr);
	if (!hWnd)
	{
		return FALSE;
	}

	//save handle for use
	g_hclientwnd = hWnd;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}
int clientwnd_response_resize(HWND hWnd,RECT rec)
{
	int x = 0;
	int y = 0;
	int w = rec.right - rec.left;
	int h = rec.bottom - rec.top;
	//adjust shape
	return MoveWindow(hWnd, x, y, w, h, true);
}



void user_create_client_wnd(HINSTANCE hInstance, int nCmdShow, HWND hParent)
{
	register_clientwnd_class(hInstance);

	clientwnd_init_instance( hInstance,  nCmdShow,  hParent);
}
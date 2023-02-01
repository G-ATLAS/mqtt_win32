
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")


void debug_info(const char* pstr)
{
	std::cout << pstr;
}

int main(int argc, char** argv)
{

	int iResult;
	WSADATA wsaData;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET newConnSocket = INVALID_SOCKET;


	debug_info("I am server\n");
	// Validate the parameters
	if (argc != 2)
	{
		debug_info("Error:Validate the parameters \n");
		return 1;
	}

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		debug_info("Error:WSAStartup failed with error \n");
		return 1;
	}
	else {
		debug_info("Initialize Winsock ok \n");
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		debug_info("Error:socket failed with error \n");
		WSACleanup();
		return 1;
	}
	else {
		debug_info("Create a SOCKET ok \n");
	}

	// Setup the TCP listening socket
	sockaddr_in bindinfo;
	bindinfo.sin_family = AF_INET;
	bindinfo.sin_addr.s_addr = inet_addr(argv[1]);
	bindinfo.sin_port = htons(9000);

	iResult = bind(ListenSocket, (SOCKADDR*)&bindinfo, sizeof(bindinfo));
	if (iResult == SOCKET_ERROR) {
		debug_info("Error:bind failed with error \n");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else {
		debug_info("bind a SOCKET ok \n");
	}


	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		debug_info("Error:listen failed with error \n");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else {
		debug_info("listen on a SOCKET ok \n");
	}

	// Accept a client socket
	newConnSocket = accept(ListenSocket, NULL, NULL);
	if (newConnSocket == INVALID_SOCKET) {
		debug_info("Error:accept failed with error \n");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else {
		debug_info("accept a  new SOCKET ok \n");
	}


	// No longer need server socket
	closesocket(ListenSocket);


	// Receive until the peer shuts down the connection
	char recvbuf[1024];
	int iSendResult;
	do {

		iResult = recv(newConnSocket, recvbuf, 1024, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			recvbuf[iResult] = '\0';
			printf("Bytes received: %s\n", recvbuf);

			// Echo the buffer back to the sender
			iSendResult = send(newConnSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(newConnSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(newConnSocket);
			WSACleanup();
			return 1;
		}

	} while (iResult > 0);


	// shutdown the connection since we're done
	iResult = shutdown(newConnSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(newConnSocket);
		WSACleanup();
		return 1;
	}



	// cleanup
	closesocket(newConnSocket);
	WSACleanup();

	return 0;

	//system("pause");
}
















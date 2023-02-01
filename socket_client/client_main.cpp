
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")


void debug_info(const char *pstr)
{
	std::cout << pstr;
}

int main(int argc, char **argv)
{
	int iResult;
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;

	debug_info("I am client\n");
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
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		debug_info("Error:socket failed with error \n");
		WSACleanup();
		return 1;
	}
	else {
		debug_info("Create a SOCKET ok \n");
	}

	// Connect to server.
	sockaddr_in serverinfo;
	serverinfo.sin_family = AF_INET;
	serverinfo.sin_addr.s_addr = inet_addr(argv[1]);
	serverinfo.sin_port = htons(9000);

	iResult = connect(ConnectSocket, (SOCKADDR*)&serverinfo, sizeof(serverinfo));
	if (iResult == SOCKET_ERROR) {
		debug_info("Error:Connect failed with error \n");
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	else {
		debug_info("Connect to server ok \n");
	}



	// Send an initial buffer
	 const char* sendbuf = "hello it is my first c socket test";
	debug_info("try to send ... \n");
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		debug_info("Error:send failed with error\n");
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	std::cout << "Bytes Sent:" << iResult << "\n";


	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		debug_info("Error:shutdown failed with error\n");
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}


	// Receive until the peer closes the connection
	char recvbuf[1024];
	do {

		iResult = recv(ConnectSocket, recvbuf, 1024, 0);
		if (iResult > 0)
		{
			printf("Bytes received: %d\n", iResult);
			recvbuf[iResult] = '\0';
			printf("Bytes received: %s\n", recvbuf);
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}









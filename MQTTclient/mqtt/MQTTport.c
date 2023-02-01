/*******************************************************************************
 * Copyright (c) 2014, 2015 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Allan Stockdill-Mander - initial API and implementation and/or initial documentation
 *    Ian Craggs - convert to FreeRTOS
 *******************************************************************************/
#define WIN32_LEAN_AND_MEAN
#include "MQTTport.h"
//#include "lwip/api.h"
//#include <lwip/sockets.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h> 
#include "user_lib.h"
  
int ThreadStart(Thread* thread, unsigned int (*fn)(void*), void* arg)
{
	int rc = 0;

	thread->task = CreateThread(NULL, 0, fn, arg, 0, NULL);

	//0-fail 1-success
	return thread->task == NULL ? 0 : 1;
}


void MutexInit(Mutex* mutex)
{
	mutex->sem = CreateSemaphoreA(NULL,1,1,"mqtt-mutex");
}

int MutexLock(Mutex* mutex)
{
	return WaitForSingleObject(mutex->sem, INFINITE);
}

int MutexUnlock(Mutex* mutex)
{
	return ReleaseSemaphore(mutex->sem,1,NULL);
}


void TimerCountdownMS(Timer* timer, unsigned int timeout_ms)
{
	timer->TicksToWait = timeout_ms;// ms/tick
	timer->TicksStart = GetTickCount64(); /* Record the time at which this function was entered. */
}


void TimerCountdown(Timer* timer, unsigned int timeout) 
{
	TimerCountdownMS(timer, timeout * 1000);
}

BOOL user_check_timeout(LONG64 *pTicksStart, LONG64* pTicksToWait)
{
	//save tick point
	LONG64 TicksPoint = GetTickCount64();

	//check for forever
	if (*pTicksToWait == -1)
	{
		return FALSE;
	}

	if (*pTicksToWait > (TicksPoint - *pTicksStart))
	{
		*pTicksToWait -= (TicksPoint - *pTicksStart);
		*pTicksStart = TicksPoint;
		return FALSE;
	}
	else {
		*pTicksToWait = 0;
		return TRUE;
	}
}

int TimerLeftMS(Timer* timer) 
{
	user_check_timeout(&(timer->TicksStart), &(timer->TicksToWait));/* updates TicksToWait to the number left */
	return (timer->TicksToWait <= 0) ? 0 : (int)(timer->TicksToWait * 1);// tick/ms
}


char TimerIsExpired(Timer* timer)
{
	return user_check_timeout(&(timer->TicksStart), &(timer->TicksToWait)) == TRUE;
}


void TimerInit(Timer* timer)
{
	//clean
	timer->TicksStart = 0;
	timer->TicksToWait = 0;
}

int win32_read(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
	int recvLen = 0;

	DWORD timeoutval = timeout_ms;
	setsockopt(n->my_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeoutval, sizeof(timeoutval));
	
	do
	{
		int rc = 0;
		rc = recv(n->my_socket, buffer + recvLen, len - recvLen, 0);
		if (rc > 0)
			recvLen += rc;
		else if (rc == 0)
		{
			XDEBUG("win32_read Connection closed\n");
			recvLen = -1;
			break;
		}
		else if (rc < 0)//timeout will be here,not always error.
		{
			int errcode = WSAGetLastError();
			if (errcode == WSAETIMEDOUT) {
				//just timeout keep already received cnt
				break;
			}
			recvLen = rc;
			XDEBUG("win32_read recv failed: %d\n", errcode);
			break;
		}
	} while (recvLen < len);

	return recvLen;
}


int win32_write(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
	int sentLen = 0;


	DWORD timeoutval = timeout_ms;
	setsockopt(n->my_socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeoutval, sizeof(timeoutval));
	do
	{
		int rc = 0;

		rc = send(n->my_socket, buffer + sentLen, len - sentLen, 0);
		if (rc > 0)
			sentLen += rc;
		else if (rc == 0)
		{
			XDEBUG("win32_write Connection closed\n");
			sentLen = -1;
			break;
		}
		else if (rc < 0)
		{
			int errcode = WSAGetLastError();
			if (errcode == WSAETIMEDOUT) {
				//just timeout keep already received cnt
				break;
			}
			sentLen = rc;
			XDEBUG("win32_write send failed: %d\n", errcode);
			break;
		}
	} while (sentLen < len );

	return sentLen;
}


void win32_disconnect(Network* n)
{
	closesocket(n->my_socket);
}


void NetworkInit(Network* n)
{
	n->my_socket = 0;
	n->mqttread = win32_read;
	n->mqttwrite = win32_write;
	n->disconnect = win32_disconnect;
}


int NetworkConnect(Network* n, char* addr, int port)
{
	int retVal = -1;
	struct addrinfo hints;
	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct sockaddr isockaddr;
	 
	char portbuf[10] = {0};
	int fnret = 0;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	_itoa_s(port, portbuf, 10, 10); XDEBUG("string port:%s\n", portbuf);
	fnret = getaddrinfo(addr, portbuf, &hints, &result);
	if (fnret != 0)
	{
		XDEBUG("getaddrinfo fail\n");
		goto exit;
	}
	XDEBUG("getaddrinfo ok\n");
	//find the first match
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		if ((ptr->ai_family == AF_INET) && (ptr->ai_protocol == IPPROTO_TCP))
		{
			if (ptr->ai_addr > 0)
			{
				isockaddr = *ptr->ai_addr;
				break;
			}
		}
	}
	XDEBUG("get a valid sockaddress struct\n");
	//new socket
	n->my_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (n->my_socket == INVALID_SOCKET)
	{
		//err
		XDEBUG("NetworkConnect socket fail\n");
		goto exit;
	}
	XDEBUG("NetworkConnect socket ok\n");

	if((retVal = connect(n->my_socket,(struct sockaddr *)&isockaddr,sizeof(struct sockaddr))) == SOCKET_ERROR)
	{
		closesocket(n->my_socket);
		XDEBUG("NetworkConnect connect fail\n");
		goto exit;
	}
	XDEBUG("NetworkConnect connect ok\n");

exit:
	return retVal;
}


#if 0
int NetworkConnectTLS(Network *n, char* addr, int port, SlSockSecureFiles_t* certificates, unsigned char sec_method, unsigned int cipher, char server_verify)
{
	SlSockAddrIn_t sAddr;
	int addrSize;
	int retVal;
	unsigned long ipAddress;

	retVal = sl_NetAppDnsGetHostByName(addr, strlen(addr), &ipAddress, AF_INET);
	if (retVal < 0) {
		return -1;
	}

	sAddr.sin_family = AF_INET;
	sAddr.sin_port = sl_Htons((unsigned short)port);
	sAddr.sin_addr.s_addr = sl_Htonl(ipAddress);

	addrSize = sizeof(SlSockAddrIn_t);

	n->my_socket = sl_Socket(SL_AF_INET, SL_SOCK_STREAM, SL_SEC_SOCKET);
	if (n->my_socket < 0) {
		return -1;
	}

	SlSockSecureMethod method;
	method.secureMethod = sec_method;
	retVal = sl_SetSockOpt(n->my_socket, SL_SOL_SOCKET, SL_SO_SECMETHOD, &method, sizeof(method));
	if (retVal < 0) {
		return retVal;
	}

	SlSockSecureMask mask;
	mask.secureMask = cipher;
	retVal = sl_SetSockOpt(n->my_socket, SL_SOL_SOCKET, SL_SO_SECURE_MASK, &mask, sizeof(mask));
	if (retVal < 0) {
		return retVal;
	}

	if (certificates != NULL) {
		retVal = sl_SetSockOpt(n->my_socket, SL_SOL_SOCKET, SL_SO_SECURE_FILES, certificates->secureFiles, sizeof(SlSockSecureFiles_t));
		if (retVal < 0)
		{
			return retVal;
		}
	}

	retVal = sl_Connect(n->my_socket, (SlSockAddr_t *)&sAddr, addrSize);
	if (retVal < 0) {
		if (server_verify || retVal != -453) {
			sl_Close(n->my_socket);
			return retVal;
		}
	}

	SysTickIntRegister(SysTickIntHandler);
	SysTickPeriodSet(80000);
	SysTickEnable();

	return retVal;
}
#endif

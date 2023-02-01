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
 *******************************************************************************/

#include <windows.h>

#if !defined(MQTTport_H)
#define MQTTport_H

//#include "FreeRTOS.h"
//#include "FreeRTOS_Sockets.h"
//#include "FreeRTOS_IP.h"
//#include "semphr.h"
//#include "task.h"

typedef struct Timer 
{
	LONG64 TicksToWait;
	LONG64 TicksStart;
} Timer;

typedef struct Network Network;

struct Network
{
	unsigned long long my_socket;
	int (*mqttread) (Network*, unsigned char*, int, int);
	int (*mqttwrite) (Network*, unsigned char*, int, int);
	void (*disconnect) (Network*);
};

void TimerInit(Timer*);
char TimerIsExpired(Timer*);
void TimerCountdownMS(Timer*, unsigned int);
void TimerCountdown(Timer*, unsigned int);
int TimerLeftMS(Timer*);

typedef struct Mutex
{
	HANDLE sem;//win32 obj handle
} Mutex;

void MutexInit(Mutex*);
int MutexLock(Mutex*);
int MutexUnlock(Mutex*);

typedef struct Thread
{
	HANDLE task;//win32 obj handle
} Thread;

int ThreadStart(Thread*, unsigned int (*fn)(void*), void* arg);

int win32_read(Network*, unsigned char*, int, int);
int win32_write(Network*, unsigned char*, int, int);
void win32_disconnect(Network*);

void NetworkInit(Network*);
int NetworkConnect(Network*, char*, int);
/*int NetworkConnectTLS(Network*, char*, int, SlSockSecureFiles_t*, unsigned char, unsigned int, char);*/

#endif

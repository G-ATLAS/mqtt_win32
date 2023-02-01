
#include <stdlib.h> 
#include "user_lib.h"
#include "user_mqtthub.h"




extern HWND g_hclientwnd;


MQTTClient mq_client;
Network mq_network;
unsigned char sendbuf[100], readbuf[100];
MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
HANDLE ghevent_bgtask = NULL;

//on create place
void user_mqtt_init(void)
{
	NetworkInit(&mq_network);
	MQTTClientInit(&mq_client, &mq_network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
}
int user_check_for_terminate(void)
{
	HANDLE hevent = ghevent_bgtask;
	DWORD rc = 0;
	
	if (hevent == NULL)return 0;

	rc = WaitForSingleObject(hevent, 0);
	if (rc == WAIT_OBJECT_0)//signaled
	{
		ResetEvent(hevent);
		return 1;
	}
	return 0;
}
//try connect button place
bool user_mqtt_try_con(char * address,int port, char* clientID, char* uname, char* upass)
{
	int rc = 0;
	HANDLE hevent = NULL;

	if ((rc = NetworkConnect(&mq_network, address, port)) != 0)
	{
		//fail
		XDEBUG("user_mqtt_try_con NetworkConnect fail\n");
		goto fail_level0;
	}
	XDEBUG("user_mqtt_try_con NetworkConnect ok\n");

	//for task sync
	hevent = CreateEventA(NULL, true, false, NULL);
	if (hevent)ghevent_bgtask = hevent;

	//background task start
	if ((rc = MQTTStartTask(&mq_client)) != 1)
	{
		//fail
		XDEBUG("user_mqtt_try_con MQTTStartTask fail\n");
		goto fail_level1;
	}
	XDEBUG("user_mqtt_try_con MQTTStartTask ok\n");

	//mqtt connect
	connectData.MQTTVersion = 4;
	connectData.clientID.cstring = clientID;
	connectData.keepAliveInterval = 60;
	connectData.cleansession = 1;
	connectData.willFlag = 0;
	connectData.username.cstring = uname;
	connectData.password.cstring = upass;

	if ((rc = MQTTConnect(&mq_client, &connectData)) != 0)
	{
		//fail
		XDEBUG("user_mqtt_try_con MQTTConnect fail\n");
		goto fail_level2;
	}
	XDEBUG("user_mqtt_try_con MQTTConnect ok\n");

	return true;

	//rollback
fail_level2:///kill task
	SetEvent(ghevent_bgtask);
	//tcp discon
	mq_network.disconnect(&mq_network);
	//wait for thread end
	WaitForSingleObject(mq_client.thread.task, 500);
	CloseHandle(mq_client.thread.task);
	CloseHandle(ghevent_bgtask);
	return false;

fail_level1:///release resource
	mq_network.disconnect(&mq_network);

fail_level0:
	return false;
}
//try discon button place
bool user_mqtt_try_discon(void)
{
	//protocol discon
	MQTTDisconnect(&mq_client);

	//send end event
	SetEvent(ghevent_bgtask);

	//tcp discon
	mq_network.disconnect(&mq_network);
	XDEBUG("tcp discon ok\n");

	//wait for thread end
	WaitForSingleObject(mq_client.thread.task, 500);
	CloseHandle(mq_client.thread.task);
	CloseHandle(ghevent_bgtask);


	return true;
}
void err_discon_action(void)
{
	XDEBUG("enter err_discon_action\n");
	//send click message to discon btn
	HWND hwndgp = GetDlgItem(g_hclientwnd, ID_GP_UO);
	HWND hwndbn = GetDlgItem(hwndgp, ID_GP_UO_BN_DISCON);
	::PostMessageA(hwndbn, BM_CLICK, 0, 0);
	hwndbn = GetDlgItem(hwndgp, ID_GP_UO_BN_STATUS);
	SetWindowTextA(hwndbn, "err -discon");
	XDEBUG("exit err_discon_action\n");
}
void mq_messagehandler(void* dat)
{
	MessageData* data = (MessageData*)dat;
	char* topic = NULL;
	char* payload = NULL;
	int topiclen = data->topicName->lenstring.len;
	int payloadlen = (int)data->message->payloadlen;
	char* ptr = NULL;
	
	XDEBUG("enter mq_messagehandler\n");
	topic = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, topiclen + 10);
	if (topic == NULL)
	{
		XDEBUG("mq_messagehandler mem1 fail\n");
		return;
	}
	payload = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, payloadlen + 10);
	if (payload == NULL)
	{
		XDEBUG("mq_messagehandler mem2 fail\n");
		HeapFree(GetProcessHeap(), 0, topic);//rollback
		return;
	}
	ptr = topic;
	//topic ready
	writeMQTTString((unsigned char**)& ptr, *(data->topicName));

	//data ready
	memcpy(payload, data->message->payload, payloadlen);

	XDEBUG("message:%s\n", payload);

	//to window
	user_put_message_on_control(&topic[2], payload, payloadlen);

	//free mem
	HeapFree(GetProcessHeap(), 0, topic);
	HeapFree(GetProcessHeap(), 0, payload);

	XDEBUG("exit mq_messagehandler\n");
}
bool user_mqtt_try_sub(void * pclient,char * topic,int qos)
{
	int rc = 0;

	if ((rc = MQTTSubscribe((MQTTClient*)pclient, topic,(QoS)qos, (void (*) (MessageData*))mq_messagehandler)) != 0)
	{
		return false;
	}

	return true;
}
bool user_mqtt_try_unsub(void* pclient, char* topic)
{
	int rc = 0;

	if ((rc = MQTTUnsubscribe((MQTTClient*)pclient, topic)) != 0)
	{
		return false;
	}
	return true;
}
bool user_mqtt_try_pub(void* pclient, char* topic,char *str,int len)
{
	MQTTMessage message;
	int rc = 0;

	message.qos = QOS1;
	message.retained = 0;
	message.payload = str;
	message.payloadlen = len;

	if ((rc = MQTTPublish((MQTTClient*)pclient, topic, &message)) != 0)
	{
		return false;
	}
	return true;
}






#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "MQTTClient.h"
#include "MQTTport.h"

#ifdef __cplusplus
}
#endif


extern MQTTClient mq_client;
extern Network mq_network;



void user_mqtt_init(void);
bool user_mqtt_try_con(char* address, int port, char* clientID, char* uname, char* upass);
bool user_mqtt_try_discon(void);
bool user_mqtt_try_sub(void* pclient, char* topic, int qos);
bool user_mqtt_try_unsub(void* pclient, char* topic);
bool user_mqtt_try_pub(void* pclient, char* topic, char* str, int len);





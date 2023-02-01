#pragma once

/* header hub */
#include "user_clientwnd.h"
#include "user_mqttfunc.h"
//#include "user_mqtthub.h"










/*wnd id define*/
#define ID_CLIENTWND1     200



/*ctrls id define*/
//#define ID_BTN1           300  //as an example
//group
#define ID_GP_BS                  300
#define ID_GP_BS_ED_BADD          301
#define ID_GP_BS_ED_BPORT         302
#define ID_GP_BS_ED_CID           303


//group
#define ID_GP_UC                  310
#define ID_GP_UC_ED_UNAME         311
#define ID_GP_UC_ED_UPASS         312

//group
#define ID_GP_UO                  320
#define ID_GP_UO_BN_CON           321
#define ID_GP_UO_BN_DISCON        322
#define ID_GP_UO_BN_STATUS        323

//group
#define ID_GP_PUB                 330
#define ID_GP_PUB_ED_TOPIC        331
#define ID_GP_PUB_ED_DATA         332
#define ID_GP_PUB_BN_PUB          333

//group
#define ID_GP_SUB                 340
#define ID_GP_SUB_ED_TOPIC        341
#define ID_GP_SUB_BN_SUB          342
#define ID_GP_SUB_LB_SUBED        343
#define ID_GP_SUB_LB_HISTORY      344
#define ID_GP_SUB_ED_RECNOW       345
#define ID_GP_SUB_BN_UNSUB          346
//debug
#define XDEBUG_ENABLE
//define vary
#ifdef XDEBUG_ENABLE
#define XDEBUG(fmt , ...)     user_debug(fmt,__VA_ARGS__)
#else
#define XDEBUG()    
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	void user_debug(const char* format, ...);
	void user_debug_init(void);
	int user_check_for_terminate(void);
	void mq_messagehandler(void* dat);
	void err_discon_action(void);
#ifdef __cplusplus
}
#endif
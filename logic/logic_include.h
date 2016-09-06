/*************************************************
  Copyright (C), 2006-2016, Aurine
  File name:  	logic_include.h
  Author:     	chenbh
  Version:    	2.0
  Date: 		2016-06-30
  Description:  
				逻辑头文件集合,包含logic各模块的头文件,
				外部要使用logic的接口只要包含该文件就可以
  History:        
    1. Date:	
       Author:	
       Modification:
*************************************************/
#ifndef __LOGIC_INCLUDE_H__
#define __LOGIC_INCLUDE_H__

#include "SysGlobal.h"
#include "storage_include.h"
#include "driver_include.h"

// itp need 
#include "lwip/ip.h"
#include "lwip/dns.h"
#include "ite/itp.h"
#include "SDL/SDL.h"
#include "ite/itv.h"
#include "gb2312.h"

#include "timer/logic_timer.h"
#include "timer/logic_time.h"

#include "netcom/logic_udp_comm.h"
#include "netcom/logic_send_event.h"
#include "netcom/logic_packet_list.h"
#include "netcom/logic_net_param.h"
#include "netcom/logic_getip.h"
#include "netcom/logic_distribute.h"

#include "media/logic_media.h"

#include "call/logic_call.h"
#include "call/logic_interphone.h"
#include "call/logic_monitor.h"
#include "call/loigc_rtsp_monitor.h"

#include "alarm/logic_alarm.h"
#include "alarm/logic_alarm_net.h"

#include "message/logic_message.h"
#include "set/logic_wlmqj_set.h"

#include "linkage/logic_linkage.h"
#include "linkage/logic_sys_arbitration.h"

#include "jiadian/logic_jiadian.h"

#include "yuyue/logic_yuyue.h"

#include "dianti/logic_dianti.h"

#include "ipmodule/logic_ipmodule.h"

#include "comm/logic_appinit.h"
#include "comm/logic_pubcmd.h"
#include "comm/logic_model_func.h"

#endif


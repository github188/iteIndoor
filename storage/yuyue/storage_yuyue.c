/********************************************************
  Copyright (C), 2006-2016, Aurine
  File name:	storage_yuyue.c
  Author:		luofl
  Version:		V2.0
  Date:			2011-05-31
  Description:	�ҵ�ԤԼ�洢

  History:                 
    1. Date:
       Author:
       Modification: 
    2. ...
*********************************************************/
#include "storage_include.h"

#ifdef _YUYUE_MODE_
#define YUYUEINFO_SIZE			(sizeof(BE_COMM))

//static uint8 TimeCount = 0;
PBE_COMM_LIST JdYuyueList = NULL;					// �ҵ�ԤԼ�б�

/*************************************************
  Function:		free_yuyue_memory
  Description: 	�ͷ�ԤԼ�洢�ڴ�
  Input:		
  	1.YuyueList
  Output:		��
  Return:		��
  Others:
*************************************************/
static void free_yuyue_memory(PBE_COMM_LIST * YuyueList)
{
	if ((*YuyueList))
	{
		if ((*YuyueList)->be_comm)
		{
			free((*YuyueList)->be_comm);
			(*YuyueList)->be_comm = NULL;
		}
		free((*YuyueList));	
		(*YuyueList) = NULL;
	}
}

/*************************************************
  Function:		malloc_yuyue_memory
  Description: 	����ԤԼ�洢�ڴ�
  Input:
  	1.YuyueList
  	2.MaxNum	����¼��
  Output:		��
  Return:		��
  Others:
*************************************************/
static void malloc_yuyue_memory (PBE_COMM_LIST * YuyueList, uint32 MaxNum)
{
	*YuyueList = (PBE_COMM_LIST)malloc(sizeof(BE_COMM_LIST));	

	if (*YuyueList)
	{
		(*YuyueList)->nCount = 0;
		(*YuyueList)->be_comm = (PBE_COMM)malloc(YUYUEINFO_SIZE * MaxNum);
	}
}		

/*************************************************
  Function:		get_yuyue_from_storage
  Description: 	���ԤԼ�¼�
  Input:		
  	1.YuyueList
  Output:		��
  Return:		��
  Others:
*************************************************/
static void get_yuyue_from_storage(PBE_COMM_LIST YuyueList)
{
	if (YuyueList)
	{
		YuyueList->nCount = Fread_common(YUYUE_MANAGE_PATH, YuyueList->be_comm, YUYUEINFO_SIZE, MAX_YUYUE_NUM);
	}
}

/*************************************************
  Function:		save_yuyue_storage
  Description: 	��ԤԼд��洢
  Input:	
  	1.mode
  	2.YuyueList	
  Output:		��
  Return:		��
  Others:
*************************************************/
static void save_yuyue_storage(PBE_COMM_LIST YuyueList)
{
	Fwrite_common(YUYUE_MANAGE_PATH, YuyueList->be_comm, YUYUEINFO_SIZE, YuyueList->nCount);
	// add by luofl 2011-07-20 �ȴ�������ȫд��FLASH
	//DelayMs_nops(500);
}

/*************************************************
  Function:		storage_get_yuyue
  Description: 	���ԤԼ�¼�
  Input:		
  	1.Calltype
  Output:		��
  Return:		��
  Others:		
  	1.��ָ���ⲿ�ͷ�
*************************************************/
PBE_COMM_LIST storage_get_yuyue (void)
{
	PBE_COMM_LIST info = NULL;
	
	malloc_yuyue_memory(&info, MAX_YUYUE_NUM);
	if (NULL == info)
	{
		return NULL;
	}
	
	get_yuyue_from_storage(info);
	return info;
}

/*************************************************
  Function:		storage_add_yuyue
  Description: 	���һ��ԤԼ�¼�
  Input:		
  	1.
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_add_yuyue (PBE_COMM yuyue)
{
	uint32 num = 0;
	PBE_COMM_LIST YuyueList = NULL;
	ECHO_STORAGE ret = ECHO_STORAGE_ERR;
	BE_COMM buf[MAX_YUYUE_NUM];

	YuyueList = storage_get_yuyue();
	// �¼�¼����ͷ��
	if (YuyueList && YuyueList->be_comm)
	{
		if (0 == YuyueList->nCount)
		{
			
			memset(YuyueList->be_comm, 0, sizeof(BE_COMM));
			memcpy(YuyueList->be_comm, yuyue, sizeof(BE_COMM));			
			YuyueList->nCount = 1;
		}
		else
		{
			memset(buf, 0, sizeof(buf));
			memcpy(&(buf[0]), yuyue, sizeof(BE_COMM));	
			num = YuyueList->nCount;
			if (num >= MAX_YUYUE_NUM)
			{
				num = MAX_YUYUE_NUM - 1;
			}
			
			// ��ԭ�м�¼�����¼�¼����
			memcpy(&(buf[1]), YuyueList->be_comm, num*YUYUEINFO_SIZE);
			YuyueList->nCount++;
			if (YuyueList->nCount > MAX_YUYUE_NUM)
			{
				//YuyueList->nCount = MAX_YUYUE_NUM;
				return ret;
			}

			memset(YuyueList->be_comm, 0, sizeof(YuyueList->be_comm));
			memcpy(YuyueList->be_comm, buf, YUYUEINFO_SIZE*YuyueList->nCount);
		}
		
		save_yuyue_storage(YuyueList);
		ret = ECHO_STORAGE_OK;
	}

	free_yuyue_memory(&YuyueList);
	return ret;
}

/*************************************************
  Function:		storage_del_yuyues
  Description: 	ɾ��ԤԼ
  Input:		
  	1.DelList	
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_del_yuyues (PBE_DEL_LIST DelList)
{
	uint8 i;
	PBE_COMM be_comm = NULL;
	PBE_COMM_LIST YuyueList = NULL;
	ECHO_STORAGE ret = ECHO_STORAGE_ERR;

	if (DelList == NULL)
	{
		warn_log("DelList == null !!\n");
		return ret;
	}
		
	YuyueList = storage_get_yuyue();
	if (YuyueList && YuyueList->be_comm)
	{
		PBE_COMM_LIST YuyueListNew = NULL;	
		malloc_yuyue_memory(&YuyueListNew, MAX_YUYUE_NUM);
	
		for (i=0; i<YuyueList->nCount; i++)
		{
			be_comm = YuyueList->be_comm + i;
			if (DelList->DelFlg[i] == 0)
			{
				memcpy(YuyueListNew->be_comm+YuyueListNew->nCount, be_comm, YUYUEINFO_SIZE);
				YuyueListNew->nCount++;
			}
		}
				
		save_yuyue_storage(YuyueListNew);
		free_yuyue_memory(&YuyueListNew);
		ret = ECHO_STORAGE_OK;
	}

	free_yuyue_memory(&YuyueList);
	return ret;
}

/*************************************************
  Function:		storage_del_yuyue
  Description: 	ɾ��ԤԼ
  Input:		
  	1.Index		����
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_del_yuyue (uint8 Index)
{
	uint8 i, j;
	PBE_COMM_LIST YuyueList = NULL;
	ECHO_STORAGE ret = ECHO_STORAGE_ERR;
	BE_COMM buf[MAX_YUYUE_NUM];
	uint32 num = 0;
	
	YuyueList = storage_get_yuyue();
	if (YuyueList && YuyueList->be_comm)
	{
		num = YuyueList->nCount;
		if (num > 0 && Index <= (num-1))
		{
			if (Index == (num-1))
			{
				memset((YuyueList->be_comm+Index), 0, YUYUEINFO_SIZE);
			}
			else
			{	
				memset(buf, 0, YUYUEINFO_SIZE*MAX_YUYUE_NUM);
				memcpy(buf, YuyueList->be_comm, YUYUEINFO_SIZE*num);
				memset(YuyueList->be_comm, 0, YUYUEINFO_SIZE*MAX_YUYUE_NUM);
				j = 0;
				for(i = 0; i < num; i++)
				{
					if (Index != i)	
					{
						memcpy(YuyueList->be_comm+j, buf+i, YUYUEINFO_SIZE);
						j++;
					}
				}
			}
			YuyueList->nCount--;
		}
		
		save_yuyue_storage(YuyueList);
		ret = ECHO_STORAGE_OK;
	}

	free_yuyue_memory(&YuyueList);
	return ret;
}

/*************************************************
  Function:		storage_clear_yuyue
  Description: 	���ԤԼ�¼�
  Input:		��		
  Output:		��
  Return:		ECHO_STORAGE
  Others:
*************************************************/
ECHO_STORAGE storage_clear_yuyue (void)
{
	PBE_COMM_LIST YuyueList = NULL;
		
	YuyueList = storage_get_yuyue();
	if (YuyueList)
	{	
		YuyueList->nCount = 0;
		memset(YuyueList->be_comm, 0, YUYUEINFO_SIZE*MAX_YUYUE_NUM);
		save_yuyue_storage(YuyueList);
		free_yuyue_memory(&YuyueList);
		return ECHO_STORAGE_OK;
	}
	else
	{
		free_yuyue_memory(&YuyueList);
		return ECHO_STORAGE_ERR;
	}
}

/*************************************************
  Function:		storage_yuyue_init
  Description: 	��ʼ��ԤԼ����
  Input:		��
  Output:		��
  Return:		
  Others:		
*************************************************/
void storage_yuyue_init(void)
{
	if (JdYuyueList)
	{
		if (JdYuyueList->be_comm)
		{
			free(JdYuyueList->be_comm);
			JdYuyueList->be_comm = NULL;
		}
		free(JdYuyueList);
		JdYuyueList = NULL;
	}
	
	JdYuyueList = storage_get_yuyue();
	//dprintf("YuyueCount: %d\n", JdYuyueList->nCount);
}

#if 0
/*************************************************
  Function:		yuyue_time
  Description: 	�ж�ԤԼʱ���Ƿ�
  Input:		��
  Output:		��
  Return:		
  Others:		
*************************************************/
static uint8 yuyue_time(BE_DATETIME BeTime)
{
	uint32 year, BeYear;
	uint8 ret = FALSE;
	DATE_TIME g_time;
	uint8 mon, day, week, hour, min, BeMon, BeDay, BeWeek, BeHour, BeMin;
	
	get_timer(&g_time);
	year = g_time.year;
	mon = g_time.month;
	day = g_time.day;
	//week = g_time.week;
	week = WeekDay(year, mon, day);
	hour = g_time.hour;
	min = g_time.min;
	
	BeYear = BeTime.tm_year;
	BeMon = BeTime.tm_mon;
	BeDay = BeTime.tm_mday + 1;
	BeWeek = (BeTime.tm_wday+1) % 7;
	BeHour = BeTime.tm_hour;
	BeMin = BeTime.tm_min;

	//dprintf("year: %d     %d\n", year, BeYear);
	//dprintf("mon: %d     %d\n", mon, BeMon);
	//dprintf("day: %d     %d\n", day, BeDay);
	//dprintf("week: %d     %d\n", week, BeWeek);
	//dprintf("hour: %d     %d\n", hour, BeHour);
	//dprintf("min: %d     %d\n", min, BeMin);
	//dprintf("BeTime.BeType = %d\n", BeTime.BeType);
	switch(BeTime.BeType)
	{
		case BE_DAY:
			if (hour == BeHour && min == BeMin)
			{
				ret = TRUE;		
			}
			break;

		case BE_WEEK:
			if (week == BeWeek && hour == BeHour && min == BeMin)
			{
				ret = TRUE;
			}
			break;

		case BE_MONTH:
			if (day == BeDay && hour == BeHour && min == BeMin)
			{
				ret = TRUE;
			}
			break;

		#if 0
		case BE_YEAR:
			if (mon == BeMon && day == BeDay && hour == BeHour && min == BeMin)
			{
				ret = TRUE;
			}
			else
			{
				ret = FALSE;
			}
			break;

		case BE_MON_FRI:
			if ((week > 0 && week < 6) && hour == BeHour && min == BeMin)
			{
				ret = TRUE;
			}
			else
			{
				ret = FALSE;
			}
			break;

		case BE_MON_SAT:
			if ((week > 0 && week <= 6) && hour == BeHour && min == BeMin)
			{
				ret = TRUE;
			}
			else
			{
				ret = FALSE;
			}
			break;

		case BE_ONE_DAY:
			if (year == BeYear && mon == BeMon && day == BeDay && hour == BeHour && min == BeMin)
			{
				ret = TRUE;
			}
			else
			{
				ret = FALSE;
			}
			break;
		#endif
		
		default:
			ret = FALSE;
			break;
	}
	dprintf("yuyue_time is same: %d\n", ret);
	return ret;
}

/*************************************************
  Function:		yuyue_ontimer
  Description: 	ԤԼ��ʱ��
  Input:		��
  Output:		��
  Return:		
  Others:		
*************************************************/
void yuyue_ontimer(void)
{
	uint8 i;
	uint16 type, betype, addr, degree, act, index, istune;
	
	//if (++TimeCount >= 60)
	if (++TimeCount >= 30)                        // ���׳��ֿ����ε�����
	{
		TimeCount = 0;
		if (JdYuyueList)
		{
			for (i = 0; i < JdYuyueList->nCount; i++)
			{
				if (JdYuyueList->be_comm[i].Used)
				{
					//dprintf("yuyue_ontimer: i = %d\n", i);
					if (TRUE == yuyue_time(JdYuyueList->be_comm[i].BeTime))
					{
						dprintf("yuyue_time: index = %d\n", i);
						betype = JdYuyueList->be_comm[i].BeType;	// ԤԼ����
						type = JdYuyueList->be_comm[i].Type;		// �豸����
						index = JdYuyueList->be_comm[i].Index;		// �豸����
						addr = JdYuyueList->be_comm[i].Address;		// �豸��ַ
						act = JdYuyueList->be_comm[i].Action;		// ִ�ж���
						degree = JdYuyueList->be_comm[i].Degree;	// ���ڼ���
                        istune = JdYuyueList->be_comm[i].IsTune;
						dprintf("yuyue betype: %d\n", betype);
						switch (betype)
						{
							case EV_BE_SM:	
							    #ifdef _AURINE_ELEC_NEW_
								if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY) )
								{
								    if (index < 5)
								    {
								        exec_jd_scene_mode(index);
								    }
								    else
								    {
								        jd_aurine_scene_open(JdYuyueList->be_comm[i].Address,JdYuyueList->be_comm[i].Address,JdYuyueList->be_comm[i].Index+1);
								    }
								}
								else
								{
								    exec_jd_scene_mode(index);
								}
								#else
								exec_jd_scene_mode(index);
								#endif
								break;
								
							case EV_BE_JD:
								if (act)
								{
									dprintf("485send  device_on: addr = %d, degree = %d\n", addr, degree);
									#ifdef _AURINE_ELEC_NEW_
									if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY) )
									{
									    if (type == JD_TYPE_DENGGUANG)
									    {
									        if (istune)
									        {
									            jd_aurine_light_open(index, addr, degree*10);
									        }
									        else
									        {
									            jd_aurine_light_open(index, addr, 100);
									        }
									    }
									    else
									    {
									        jd_aurine_yuyue_oper(JD_ON, type-1, index, addr, 100);
									    }
									}
									else
									{
									    device_on(addr, degree);
									}
									#else
									device_on(addr, degree);
									#endif
								}
								else
								{
								    #ifdef _AURINE_ELEC_NEW_
									if (JD_FACTORY_ACBUS == storage_get_extmode(EXT_MODE_JD_FACTORY))
									{
									    jd_aurine_yuyue_oper(JD_OFF,type-1,index,addr,100);
									}
									else
									{
									    device_off(addr);
									}
									#else
									device_off(addr);
									#endif
									dprintf("485send  device_off: addr = %d\n", addr);
								}
								break;

							default:
								break;
						}
					}
				}
			}
		}
	}
}
#endif
#endif


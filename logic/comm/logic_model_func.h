/*********************************************************
  Copyright (C), 2006-2010, Aurine
  File name:	logic_model_func.h
  Author:   	
  Version:   	
  Date: 		
  Description:  公共命令
  
  History:                   
    1. Date:			 
       Author:				
       Modification:	
    2. ...
*********************************************************/
#ifndef __LOGIC_MODEL_FUNC_H__
#define __LOGIC_MODEL_FUNC_H__


#include "logic_include.h"


/*************************************************
Function:		gb2312ToUtf8
Description:	字符编码转换
Input:
1.ptDestText	目标字符串
2.nDestLength	长度
3.ptSrcText		源字符串
4.nSrcLength	长度

Output:			无
Return:			无
Others:
*************************************************/
void gb2312ToUtf8(char*  ptDestText, int  nDestLength, char*  ptSrcText, int  nSrcLength);



#endif

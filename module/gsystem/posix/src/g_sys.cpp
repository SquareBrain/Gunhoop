/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_sys.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-03-04
* @note 
*
*  1. 2014-03-04 duye Created this file
* 
*/

#include <unistd.h>
#include <stdarg.h>
#include <g_sys.h>

void GSys::gsleep(const GUint64 time)
{
    sleep(time);
}

void GSys::gusleep(const GUint64 time)
{
    usleep(time);
}

GUint64 GSys::gvsnprintf(GInt8* buffer, const GUint64 size, const GInt8* args, ...)
{
    va_list vaList;
	va_start(vaList, args);
    GUint64 strLen = vsnprintf(buffer, size, args, vaList);
	va_end(vaList);	
    return strLen;
}

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

#include <g_sys.h>

G_NS_GCOMMON_BEG

void Sys::Sleep(const GUint64 time)
{
    sleep(time);
}

void Sys::Usleep(const GUint64 time)
{
    usleep(time);
}

G_NS_GCOMMON_END

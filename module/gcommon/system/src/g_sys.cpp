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

static const GInt8* G_LOG_PREFIX = "gohoop.gcommon.system.sys";

G_NS_GCOMMON_BEG

void Sys::gsleep(const GUint64 time)
{
    sleep(time);
}

void Sys::gusleep(const GUint64 time)
{
    usleep(time);
}

G_NS_END

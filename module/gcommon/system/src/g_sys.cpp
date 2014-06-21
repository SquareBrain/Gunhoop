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

GUint32 System::GetMaxFileNameSize()
{
    return MAX_FILE_NAME_SIZE;
}

D_Void System::Sleep(const GUint64 time)
{
    sleep(time);
}

D_Void System::Usleep(const GUint64 time)
{
    usleep(time);
}

DUYE_POSIX_NS_END

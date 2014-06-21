/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		duye_system.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-03-04
* @note 
*
*  1. 2014-03-04 duye Created this file
* 
*/

#include <duye/posix/system/inc/duye_system.h>

DUYE_POSIX_NS_BEG

D_UInt32 System::GetMaxFileNameSize()
{
    return MAX_FILE_NAME_SIZE;
}

D_Void System::Sleep(const D_UInt64 time)
{
    sleep(time);
}

D_Void System::Usleep(const D_UInt64 time)
{
    usleep(time);
}

DUYE_POSIX_NS_END

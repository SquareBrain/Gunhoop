/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		duye_system_time.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-03-09
* @note 
*
*  1. 2014-03-09 duye Created this file
* 
*/

#include <duye/posix/system/inc/duye_system_time.h>

DUYE_POSIX_NS_BEG

SystemTime::SystemTime()
{
}

SystemTime::~SystemTime()
{
}

D_Bool SystemTime::GetSystemTimeA(D_Int8* sysTime, const D_UInt64 bufSize)
{
    if (sysTime == NULL && bufSize < 8)
    {
        return false;
    }
    
    const D_Int8* curSysTime = "20140309";
    
    sprintf(sysTime, "%s", curSysTime);

    return true;
}

D_Bool SystemTime::GetSystemTimeB(D_Int8* sysTime, const D_UInt64 bufSize)
{
    if (sysTime == NULL && bufSize < 10)
    {
        return false;
    }
    
    const D_Int8* curSysTime = "2014-03-09";
    
    sprintf(sysTime, "%s", curSysTime);

    return true;
}

D_Bool SystemTime::GetSystemTimeC(D_Int8* sysTime, const D_UInt64 bufSize)
{
    if (sysTime == NULL && bufSize < 10)
    {
        return false;
    }
    
    const D_Int8* curSysTime = "2014/03/09";
    
    sprintf(sysTime, "%s", curSysTime);

    return true;
}

D_Bool SystemTime::GetSystemTimeD(D_Int8* sysTime, const D_UInt64 bufSize)
{
    if (sysTime == NULL && bufSize < 10)
    {
        return false;
    }
    
    const D_Int8* curSysTime = "2014.03.09";
    
    sprintf(sysTime, "%s", curSysTime);

    return true;
}

D_Bool SystemTime::GetSystemTimeE(D_Int8* sysTime, const D_UInt64 bufSize)
{
    if (sysTime == NULL && bufSize < 14)
    {
        return false;
    }
    
    const D_Int8* curSysTime = "2014-03-09:999";
    
    sprintf(sysTime, "%s", curSysTime);

    return true;
}

DUYE_POSIX_NS_END

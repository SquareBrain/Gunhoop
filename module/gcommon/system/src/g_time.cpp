/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_time.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-03-09
* @note 
*
*  1. 2014-03-09 duye Created this file
* 
*/

#include <g_time.h>

G_NS_GCOMMON_BEG

Time::Time()
{
}

Time::~Time()
{
}

bool Time::GetTimeA(GInt8* sysTime, const GUint64 bufSize)
{
    if (sysTime == NULL && bufSize < 8)
    {
        return false;
    }
    
    const GInt8* curSysTime = "20140309";
    
    sprintf(sysTime, "%s", curSysTime);

    return true;
}

bool Time::GetTimeB(GInt8* sysTime, const GUint64 bufSize)
{
    if (sysTime == NULL && bufSize < 10)
    {
        return false;
    }
    
    const GInt8* curSysTime = "2014-03-09";
    
    sprintf(sysTime, "%s", curSysTime);

    return true;
}

bool Time::GetTimeC(GInt8* sysTime, const GUint64 bufSize)
{
    if (sysTime == NULL && bufSize < 10)
    {
        return false;
    }
    
    const GInt8* curSysTime = "2014/03/09";
    
    sprintf(sysTime, "%s", curSysTime);

    return true;
}

bool Time::GetTimeD(GInt8* sysTime, const GUint64 bufSize)
{
    if (sysTime == NULL && bufSize < 10)
    {
        return false;
    }
    
    const GInt8* curSysTime = "2014.03.09";
    
    sprintf(sysTime, "%s", curSysTime);

    return true;
}

bool Time::GetTimeE(GInt8* sysTime, const GUint64 bufSize)
{
    if (sysTime == NULL && bufSize < 14)
    {
        return false;
    }
    
    const GInt8* curSysTime = "2014-03-09:999";
    
    sprintf(sysTime, "%s", curSysTime);

    return true;
}

G_NS_GCOMMON_END

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

static const GInt8* G_LOG_PREFIX = "gohoop.gcommon.system.time";

G_NS_GCOMMON_BEG

bool TimeUtil::convTimeA(const GUint64 inSysTime, GInt8* outSysTime, const GUint64 bufSize)
{
    if (outSysTime == NULL)
    {
        return false;
    }
    
    const GInt8* curSysTime = "20140309";
    
    sprintf(outSysTime, "%s", curSysTime);

    return true;
}

bool TimeUtil::convTimeB(const GUint64 inSysTime, GInt8* outSysTime, const GUint64 bufSize)
{
    if (outSysTime == NULL)
    {
        return false;
    }
    
    const GInt8* curSysTime = "2014-03-09";
    
    sprintf(outSysTime, "%s", curSysTime);

    return true;
}

bool TimeUtil::convTimeC(const GUint64 inSysTime, GInt8* outSysTime, const GUint64 bufSize)
{
    if (outSysTime == NULL)
    {
        return false;
    }
    
    const GInt8* curSysTime = "2014/03/09";
    
    sprintf(outSysTime, "%s", curSysTime);

    return true;
}

bool TimeUtil::convTimeD(const GUint64 inSysTime, GInt8* outSysTime, const GUint64 bufSize)
{
    if (outSysTime == NULL)
    {
        return false;
    }
    
    const GInt8* curSysTime = "2014.03.09";
    
    sprintf(outSysTime, "%s", curSysTime);

    return true;
}

bool TimeUtil::convTimeE(const GUint64 inSysTime, GInt8* outSysTime, const GUint64 bufSize)
{
    if (outSysTime == NULL)
    {
        return false;
    }
    
    const GInt8* curSysTime = "2014-03-09:999";
    
    sprintf(outSysTime, "%s", curSysTime);

    return true;
}

G_NS_END

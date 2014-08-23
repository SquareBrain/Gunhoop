/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_timeconv.cpp
* @version     
* @brief      
* @author	duye
* @date		2014-03-09
* @note 
*
*  1. 2014-03-09 duye Created this file
* 
*/
#include <g_timeconv.h>

using namespace gcom;

GResult TimeConv::convTime(const GInt8* format, GInt8* buffer, const GUint32 size)
{
	if (buffer == nullptr || size < 0)
    {
    	return G_NO;
    }
    
	return G_YES;
}

GResult TimeConv::convTimeA(const GUint64 inSysTime, GInt8* outSysTime, const GUint64 bufSize)
{
	if (outSysTime == NULL)
	{
		return G_NO;
	}

	const GInt8* curSysTime = "20140309";

	sprintf(outSysTime, "%s", curSysTime);

	return G_YES;
}

GResult TimeConv::convTimeB(const GUint64 inSysTime, GInt8* outSysTime, const GUint64 bufSize)
{
	if (outSysTime == NULL)
	{
	return G_NO;
	}

	const GInt8* curSysTime = "2014-03-09";

	sprintf(outSysTime, "%s", curSysTime);

	return G_YES;
}

GResult TimeConv::convTimeC(const GUint64 inSysTime, GInt8* outSysTime, const GUint64 bufSize)
{
	if (outSysTime == NULL)
	{
		return G_NO;
	}

	const GInt8* curSysTime = "2014/03/09";

	sprintf(outSysTime, "%s", curSysTime);

	return G_YES;
}

GResult TimeConv::convTimeD(const GUint64 inSysTime, GInt8* outSysTime, const GUint64 bufSize)
{
	if (outSysTime == NULL)
	{
		return G_NO;
	}

	const GInt8* curSysTime = "2014.03.09";

	sprintf(outSysTime, "%s", curSysTime);

	return G_YES;
}

GResult TimeConv::convTimeE(const GUint64 inSysTime, GInt8* outSysTime, const GUint64 bufSize)
{
	if (outSysTime == NULL)
	{
		return false;
	}

	const GInt8* curSysTime = "2014-03-09:999";

	sprintf(outSysTime, "%s", curSysTime);

	return G_YES;
}

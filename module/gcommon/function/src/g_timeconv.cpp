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

using namespace gsys;
using namespace gcom;

GResult TimeConv::convTime(const GInt8* format, GInt8* buffer, const GUint32 size)
{
	// millisecond
	return convTime(gsys::getSysTime(), format, buffer, size);
}

GResult TimeConv::convTime(const std::string& format, std::string& buffer)
{
	GInt8* timeBuffer[128] = {0};
	if (convTime(format, timeBuffer, 128) != G_YES)
	{
		return G_NO;
	}
	
	buffer.assign(timeBuffer);
	
	return G_YES;
}

GResult TimeConv::convTime(const GUint64 millisecond, const GInt8* format, GInt8* buffer, const GUint32 size)
{
	if (format == nullptr || buffer == nullptr || size == 0)
	{
		return G_NO;	
	}
	
	return G_YES;
}

GResult TimeConv::convTime(const GUint64 millisecond, const std::string format, std::string& buffer)
{
	GInt8* timeBuffer[128] = {0};
	if (convTime(millisecond, format, timeBuffer, 128) != G_YES)
	{
		return G_NO;	
	}
	
	buffer.assign(timeBuffer);
	
	return G_YES;
}

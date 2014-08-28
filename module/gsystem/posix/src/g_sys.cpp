/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_sys.cpp
* @version     
* @brief      
* @author	duye
* @date		2014-03-04
* @note 
*
*  1. 2014-03-04 duye Created this file
* 
*/

#include <unistd.h>
#include <stdarg.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <g_sys.h>

using namespace gsys;

void System::sleep(const GUint64 time)
{
	::sleep(time);
}

void System::usleep(const GUint64 time)
{
	::usleep(time);
}

GUint64 System::pformat(GInt8* buffer, const GUint64 size, const GInt8* args, ...)
{
	va_list vaList;
	va_start(vaList, args);
	GUint64 strLen = vsnprintf(buffer, size, args, vaList);
	va_end(vaList);	
	return strLen;
}

GResult System::shell(const GInt8* cmd)
{
	G_ASSERT(cmd != NULL);

	FILE* retStream = popen(cmd, "r");
	if (retStream == NULL)
	{
		return G_NO;
	}
	
	pclose(retStream);

	return G_YES;      
}

GResult System::shell(const GInt8* cmd, GInt8* buffer, const GUint32 size)
{
	G_ASSERT(cmd != NULL && buffer != NULL && size > 0);

	FILE* retStream = popen(cmd, "r");
	if (retStream == NULL)
	{
		return G_NO;
	}

	memset(buffer, 0, size);
	fread(buffer, sizeof(GInt8), size, retStream);
	
	pclose(retStream);

	return G_YES;      
}

GUint64 System::getSysTime()
{
	struct timeval now;
	struct timezone tz;
	if (gettimeofday(&now, &tz) < 0)
	{
		return 0;
	}
	
	return (GUint64)(now.tv_sec) * 1000000 + now.tv_usec;
}

GResult System::getSysTime(const GInt8* format, GInt8* buffer, const GUint32 size)
{
        struct timeval tv;
        struct tm      tm;
        size_t         len = 28;
 
        gettimeofday(&tv, NULL);
        localtime_r(&tv.tv_sec, &tm);
        strftime(buf, len, "%Y-%m-%d %H:%M:%S", &tm);
        len = strlen(buf);
        sprintf(buf + len, ".%06.6d", (int)(tv.tv_usec));
 
        return buf;		
}

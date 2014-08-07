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

void GSys::sleep(const GUint64 time)
{
	::sleep(time);
}

void GSys::usleep(const GUint64 time)
{
	::usleep(time);
}

GUint64 GSys::format(GInt8* buffer, const GUint64 size, const GInt8* args, ...)
{
	va_list vaList;
	va_start(vaList, args);
	GUint64 strLen = vsnprintf(buffer, size, args, vaList);
	va_end(vaList);	
	return strLen;
}

GResult GSys::shell(const GInt8* cmd, const GShellMode mode, GInt8* buffer, const GUint32 size)
{
	G_ASSERT(cmd != NULL && buffer != NULL && size > 0);

	GInt8* pipeMode = NULL;
	switch (mode)
	{
	case G_SHELL_R:
		pipeMode = (GInt8*)"r";
		break;
	case G_SHELL_W:
		pipeMode = (GInt8*)"w";
		break;
	default:
		return G_NO;
		break;
	}


	FILE* retStream = popen(cmd, pipeMode);
	if (retStream == NULL)
	{
		return G_NO;
	}

	memset(buffer, 0, size);
	fread(buffer, sizeof(GInt8), size, retStream);
	
	pclose(retStream);

	return G_YES;      
}

GUint64 GSys::getSysTime()
{
	struct timeval now;
	struct timezone tz;
	if (gettimeofday(&now, &tz) < 0)
	{
		return 0;
	}
	
	return (GUint64)(now.tv_sec) * 1000 + now.tv_usec / 1000;
}

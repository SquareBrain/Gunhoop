/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_sys.cpp
* @version     
* @brief      
* @author   duye
* @date	    2014-03-04
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

namespace gsys {

void System::sleep(const GUint64 time)
{
    ::sleep(time);
}

void System::usleep(const GUint64 time)
{
    ::usleep(time);
}

GInt64 System::pformat(GInt8* buffer, const GUint64 size, const GInt8* args, ...)
{
    IS_NULL_RX(buffer, -1);
    va_list vaList;
    va_start(vaList, args);
    GUint64 strLen = vsnprintf(buffer, size, args, vaList);
    va_end(vaList);	
    return strLen;
}

GResult System::shell(const GInt8* cmd)
{
    IS_NULL_RX(cmd, G_NO);

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
    IS_NULL_RX(cmd, G_NO);
    IS_NULL_RX(buffer, G_NO);

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

GUint64 System::sysTime()
{
    struct timeval now;
    struct timezone tz;
    if (gettimeofday(&now, &tz) < 0)
    {
    	return 0;
    }
	
    return (GUint64)(now.tv_sec) * 1000000 + now.tv_usec;
}

GResult System::sysTime(const GInt8* format, GInt8* buffer, const GUint32 size)
{
    IS_NULL_RX(format, G_NO);
    IS_NULL_RX(buffer, G_NO);
	
    struct timeval tv;
    struct tm tm;
    
    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &tm);
    //strftime(buffer, size, "%Y-%m-%d %H:%M:%S", &tm);
    GUint32 len = strftime(buffer, size, format, &tm);
    if (len + 7 >= size)
    {
    	return G_NO;	
    }
    
    snprintf(buffer + len, size - len, ".%6d", (GInt32)tv.tv_usec);

    return G_YES;		
}

GResult System::optArg(GInt32 argc, GInt8** argv, GInt8* cmd, GInt64& value)
{
    IS_NULL_RX(argv, G_NO);
    IS_NULL_RX(cmd, G_NO);
	
    if (getopt(argc, argv, cmd) != cmd[0])
    {
    	return G_NO;
    }
	
    const std::string temp_str(optarg);
    value = (GInt64)std::stoi(temp_str);
    return G_YES;
}

GResult System::optArg(GInt32 argc, GInt8** argv, GInt8* cmd, GUint64& value)
{
    IS_NULL_RX(argv, G_NO);
    IS_NULL_RX(cmd, G_NO);
	
    if (getopt(argc, argv, cmd) != cmd[0])
    {
    	return G_NO;
    }
    
    const std::string temp_str(optarg);
    value = (GUint64)std::stoi(temp_str);	
    return G_YES;
}

GResult System::optArg(GInt32 argc, GInt8** argv, GInt8* cmd, std::string& value)
{
    IS_NULL_RX(argv, G_NO);
    IS_NULL_RX(cmd, G_NO);
	
    if (getopt(argc, argv, cmd) != cmd[0])
    {
    	return G_NO;
    }
	
    value.assign(optarg);
    return G_YES;
}
}

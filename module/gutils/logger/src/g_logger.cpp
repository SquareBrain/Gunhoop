/***********************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_logger.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-07-01
* @note 
*
*  1. 2014-07-01 duye Created this file
* 
*/
#include <stdarg.h>
#include <stdio.h>
#include <g_logger_impl.h>
#include <g_logger.h>

static const GUint64 DEF_ONE_LINE_BUF_SIZE = 1024;

G_NS_GUTILS_BEG
  
GResult LoggerUtil::init()
{
    return Logger::GetInstance()->init();
}

GResult LoggerUtil::uninit()
{
    return Logger::GetInstance()->uninit();
}

void LoggerUtil::printError(const GInt8* module, 
    const GInt8* file, 
    const GUint32 line, 
    const GInt8* function,
    const GInt8* args, ...)
{
    printLog((GInt32)LOG_ERROR, module, file, line, function, args); 
}

void LoggerUtil::printWarn(const GInt8* module, 
    const GInt8* file, 
    const GUint32 line, 
    const GInt8* function,
    const GInt8* args, ...)
{
    printLog((GInt32)LOG_WARN, module, file, line, function, args);
}

void LoggerUtil::printInfo(const GInt8* module, 
    const GInt8* file, 
    const GUint32 line, 
    const GInt8* function,
    const GInt8* args, ...)
{
    printLog((GInt32)LOG_INFO, module, file, line, function, args);
}

void LoggerUtil::printDebug(const GInt8* module, 
    const GInt8* file, 
    const GUint32 line, 
    const GInt8* function,
    const GInt8* args, ...)
{
    printLog((GInt32)LOG_DEBUG, module, file, line, function, args);
}

void LoggerUtil::printTrace(const GInt8* module, 
    const GInt8* file, 
    const GUint32 line, 
    const GInt8* function,
    const GInt8* args, ...)
{
    printLog((GInt32)LOG_TRACE, module, file, line, function, args);
}

void LoggerUtil::printLog(const GInt32 logLevel, 
    const GInt8* module, 
    const GInt8* file, 
    const GUint32 line, 
    const GInt8* function,
    const GInt8* args, ...)
{
    va_list vaList;
	va_start(vaList, args);
	Logger::GetInstance()->printLog((const LogLevel)logLevel, module, file, line, function, args, vaList);
	va_end(vaList);		        
}

G_NS_END

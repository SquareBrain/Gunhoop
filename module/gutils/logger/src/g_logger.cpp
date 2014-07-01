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
    const GInt8* log)
{
    Logger::GetInstance()->printLog(LOG_ERROR, module, file, line, function, log);    
}

void LoggerUtil::printWarn(const GInt8* module, 
    const GInt8* file, 
    const GUint32 line, 
    const GInt8* function,
    const GInt8* log)
{
}

void LoggerUtil::printInfo(const GInt8* module, 
    const GInt8* file, 
    const GUint32 line, 
    const GInt8* function,
    const GInt8* log)
{
}

void LoggerUtil::printDebug(const GInt8* module, 
    const GInt8* file, 
    const GUint32 line, 
    const GInt8* function,
    const GInt8* log)
{
}

void LoggerUtil::printTrace(const GInt8* module, 
    const GInt8* file, 
    const GUint32 line, 
    const GInt8* function,
    const GInt8* log)
{
}

G_NS_END

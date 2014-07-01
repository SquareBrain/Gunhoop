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
* @date     2014-06-18
* @note 
*
*  1. 2014-06-18 duye Created this file
* 
*/
#include <cstdio>
#include <g_logger.h>

G_NS_GUTILS_BEG

GResult LoggerUtil::Init()
{
    return true;
}
       
GResult LoggerUtil::Uninit()
{
    return true;
}

void LoggerUtil::PrintLog(LogLevel logLevel, 
    const GInt8* file, 
    const GUint32 line, 
    const GInt8* function,
    const GInt8* module,
    const GInt8* log)
{
                   
}

G_NS_END

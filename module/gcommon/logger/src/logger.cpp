/***********************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		logger.cpp
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
#include <logger.h>

G_NS_GCOMMON_BEG

GResult Logger::Init()
{
    return true;
}
       
GResult Logger::Uninit()
{
    return true;
}
      
void Logger::PrintError(const GInt8* prefix, const GInt8* str)
{
    printf("<ERROR><%s>%s", prefix, str);
}
   
void Logger::PrintWarn(const GInt8* prefix, const GInt8* str)
{
    printf("<WARN><%s>%s", prefix, str);
}
       
void Logger::PrintInfo(const GInt8* prefix, const GInt8* str)
{
    printf("<INFO><%s>%s", prefix, str);
}
       
void Logger::PrintDebug(const GInt8* prefix, const GInt8* str)
{
    printf("<DEBUG><%s>%s", prefix, str);
}

void Logger::PrintTrace(const GInt8* prefix, const GInt8* str)
{
    printf("<TRACE><%s>%s", prefix, str);
}

G_NS_GCOMMON_END

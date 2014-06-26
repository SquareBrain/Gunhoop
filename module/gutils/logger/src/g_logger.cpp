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
      
void LoggerUtil::PrintError(const GInt8* prefix, const GInt8* str)
{
    printf("<ERROR><%s>%s", prefix, str);
}
   
void LoggerUtil::PrintWarn(const GInt8* prefix, const GInt8* str)
{
    printf("<WARN><%s>%s", prefix, str);
}
       
void LoggerUtil::PrintInfo(const GInt8* prefix, const GInt8* str)
{
    printf("<INFO><%s>%s", prefix, str);
}
       
void LoggerUtil::PrintDebug(const GInt8* prefix, const GInt8* str)
{
    printf("<DEBUG><%s>%s", prefix, str);
}

void LoggerUtil::PrintTrace(const GInt8* prefix, const GInt8* str)
{
    printf("<TRACE><%s>%s", prefix, str);
}

G_NS_END

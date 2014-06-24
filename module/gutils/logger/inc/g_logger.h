/***********************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_logger.h
* @version     
* @brief      
* @author   duye
* @date     2014-2-18
* @note 
*
*  2. 2014-06-21 duye move to gohoop 
*  1. 2014-02-18 duye Created this file
* 
*/

#pragma once

#include <g_type.h>

#define G_LOG_INIT() \
	GCommon::Logger::Init();
    
#define G_LOG_UNINIT() \
	GCommon::Logger::Uninit();

#define G_LOG_ERROR(prefix, x) \
	GCommon::Logger::PrintError(prefix, "error");

#define G_LOG_WARN(prefix, x) \
	GCommon::Logger::PrintWarn(prefix, "warn");

#define G_LOG_INFO(prefix, x) \
	GCommon::Logger::PrintWarn(prefix, "info");

#define G_LOG_DEBUG(prefix, x) \
	GCommon::Logger::PrintWarn(prefix, "debug");    

#define G_LOG_TRACE(prefix, x) \
	GCommon::Logger::PrintWarn(prefix, "trace");  

G_NS_GCOMMON_BEG

// brief : posix socket wrapper class
class Logger
{
public:
	// brief : init
	// note		
    static GResult Init();

	// brief : uninit
	// note		    
    static GResult Uninit();

	// brief : print error
	// @para [in]str : error information
	// note		    
    static void PrintError(const GInt8* prefix, const GInt8* str);

	// brief : print warn
	// @para [in]str : error information
	// note		    
    static void PrintWarn(const GInt8* prefix, const GInt8* str);

	// brief : print info
	// @para [in]str : error information
	// note		    
    static void PrintInfo(const GInt8* prefix, const GInt8* str);

	// brief : print debug
	// @para [in]str : error information
	// note		    
    static void PrintDebug(const GInt8* prefix, const GInt8* str);

	// brief : print trace
	// @para [in]str : error information
	// note		    
    static void PrintTrace(const GInt8* prefix, const GInt8* str);
};

G_NS_GCOMMON_END

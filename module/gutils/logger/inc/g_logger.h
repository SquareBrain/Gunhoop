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
	GUtils::LoggerUtil::Init();
    
#define G_LOG_UNINIT() \
	GUtils::LoggerUtil::Uninit();

#define G_LOG_ERROR(prefix, x) \
	GUtils::LoggerUtil::PrintError(prefix, x);

#define G_LOG_WARN(prefix, x) \
	GUtils::LoggerUtil::PrintWarn(prefix, x);

#define G_LOG_INFO(prefix, x) \
	GUtils::LoggerUtil::PrintWarn(prefix, x);

#define G_LOG_DEBUG(prefix, x) \
	GUtils::LoggerUtil::PrintWarn(prefix, x);    

#define G_LOG_TRACE(prefix, x) \
	GUtils::LoggerUtil::PrintWarn(prefix, x);  

G_NS_GUTILS_BEG

// brief : posix socket wrapper class
class LoggerUtil
{
public:	
    /**
     * init
     * @note 
     */	 	
    static GResult Init();
	
    /**
     * uninit
     * @note 
     */	 	
    static GResult Uninit();
	
    /**
     * print error
     * @para [in] str : error information
     * @note 
     */	 	
    static void PrintError(const GInt8* prefix, const GInt8* str);
	
    /**
     * print warning
     * @para [in] str : warning information
     * @note 
     */	
    static void PrintWarn(const GInt8* prefix, const GInt8* str);

    /**
     * print print normal information
     * @para [in] str : normal information
     * @note 
     */		
    static void PrintInfo(const GInt8* prefix, const GInt8* str);
	
    /**
     * print print debug information
     * @para [in] str : debug information
     * @note 
     */			
    static void PrintDebug(const GInt8* prefix, const GInt8* str);

    /**
     * print print trace information
     * @para [in] str : trace information
     * @note 
     */			
    static void PrintTrace(const GInt8* prefix, const GInt8* str);
};

G_NS_END

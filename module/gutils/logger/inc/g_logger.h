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

#include <g_logger_impl.h>

#define G_LOG_INIT() \
	GUtils::LoggerUtil::Init();
    
#define G_LOG_UNINIT() \
	GUtils::LoggerUtil::Uninit();

#define G_LOG_ERROR(module, args...) \
	GUtils::LoggerUtil::PrintError(__FILE__, __LINE__, __FUNCTION__, module, ##args);

#define G_LOG_WARN(module, args...) \
	GUtils::LoggerUtil::PrintWarn(__FILE__, __LINE__, __FUNCTION__, module, ##args);

#define G_LOG_INFO(module, args...) \
	GUtils::LoggerUtil::PrintInfo(__FILE__, __LINE__, __FUNCTION__, module, ##args);

#define G_LOG_DEBUG(module, args...) \
	GUtils::LoggerUtil::PrintDebug(__FILE__, __LINE__, __FUNCTION__, module,##args);    

#define G_LOG_TRACE(module, args...) \
	GUtils::LoggerUtil::PrintTrace(__FILE__, __LINE__, __FUNCTION__, module, ##args);  

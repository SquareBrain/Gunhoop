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
	GUtils::LoggerUtil::init();
    
#define G_LOG_UNINIT() \
	GUtils::LoggerUtil::uninit();

#define G_LOG_ERROR(module, args...) \
	GUtils::LoggerUtil::printError(__FILE__, __LINE__, __FUNCTION__, module, ##args);

#define G_LOG_WARN(module, args...) \
	GUtils::LoggerUtil::printWarn(__FILE__, __LINE__, __FUNCTION__, module, ##args);

#define G_LOG_INFO(module, args...) \
	GUtils::LoggerUtil::printInfo(__FILE__, __LINE__, __FUNCTION__, module, ##args);

#define G_LOG_DEBUG(module, args...) \
	GUtils::LoggerUtil::printDebug(__FILE__, __LINE__, __FUNCTION__, module,##args);    

#define G_LOG_TRACE(module, args...) \
	GUtils::LoggerUtil::printTrace(__FILE__, __LINE__, __FUNCTION__, module, ##args);  

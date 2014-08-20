/***********************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_logger.cpp
* @version     
* @brief      
* @author	duye
* @date		2014-07-01
* @note 
*
*  1. 2014-07-01 duye Created this file
* 
*/
#include <stdio.h>
#include <g_logger_impl.h>
#include <g_logger.h>

GResult GLogger::init()
{
	return GLoggerImpl::GetInstance()->init();
}

GResult GLogger::uninit()
{
	return GLoggerImpl::GetInstance()->uninit();
}

void GLogger::printError(const GInt8* module, 
	const GInt8* file, 
	const GUint32 line, 
	const GInt8* function,
	const GInt8* args, ...)
{
	printLog((GInt32)LOG_ERROR, module, file, line, function, args); 
}

void GLogger::printWarn(const GInt8* module, 
	const GInt8* file, 
	const GUint32 line, 
	const GInt8* function,
	const GInt8* args, ...)
{
	printLog((GInt32)LOG_WARN, module, file, line, function, args);
}

void GLogger::printInfo(const GInt8* module, 
	const GInt8* file, 
	const GUint32 line, 
	const GInt8* function,
	const GInt8* args, ...)
{
	printLog((GInt32)LOG_INFO, module, file, line, function, args);
}

void GLogger::printDebug(const GInt8* module, 
    const GInt8* file, 
    const GUint32 line, 
    const GInt8* function,
    const GInt8* args, ...)
{
	printLog((GInt32)LOG_DEBUG, module, file, line, function, args);
}

void GLogger::printTrace(const GInt8* module, 
	const GInt8* file, 
	const GUint32 line, 
	const GInt8* function,
	const GInt8* args, ...)
{
	printLog((GInt32)LOG_TRACE, module, file, line, function, args);
}

void GLogger::printLog(const GInt32 logLevel, 
	const GInt8* module, 
	const GInt8* file, 
	const GUint32 line, 
	const GInt8* function,
	const GInt8* args, ...)
{
	GLoggerImpl::GetInstance()->printLog((const GLogLevel)logLevel, module, file, line, function, args);		        
}

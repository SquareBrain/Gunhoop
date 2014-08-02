/***********************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file		g_logger.h
* @version     
* @brief      
* @author   	duye
* @date     	2014-2-18
* @note 
*
*  2. 2014-06-21 duye move to gohoop 
*  1. 2014-02-18 duye Created this file
* 
*/

#pragma once

#include <g_system.h>

/**
 * @brief init log system
 */	
#define G_LOG_INIT() GLogger::init()

/**
 * @brief uninit log system
 */	    
#define G_LOG_UNINIT() GLogger::uninit()

/**
 * @brief print error log
 * @param [in] module : module name
 * @param [in] args : parameters
 */	
#define G_LOG_ERROR(module, args...) GLogger::printError(module, __FILE__, __LINE__, __FUNCTION__, ##args)

/**
 * @brief print warning log
 * @param [in] module : module name
 * @param [in] args : parameters
 */	
#define G_LOG_WARN(module, args...) GLogger::printWarn(module, __FILE__, __LINE__, __FUNCTION__, ##args)

/**
 * @brief print info log
 * @param [in] module : module name
 * @param [in] args : parameters
 */	
#define G_LOG_INFO(module, args...) GLogger::printInfo(module, __FILE__, __LINE__, __FUNCTION__, ##args)

/**
 * @brief print debug log
 * @param [in] module : module name
 * @param [in] args : parameters
 */	
#define G_LOG_DEBUG(module, args...) GLogger::printDebug(module, __FILE__, __LINE__, __FUNCTION__, ##args)

/**
 * @brief print trace log
 * @param [in] module : module name
 * @param [in] args : parameters
 */	
#define G_LOG_TRACE(module, args...) GLogger::printTrace(module, __FILE__, __LINE__, __FUNCTION__, ##args)  

/**
 * system log tool
 */	
class GLogger
{
public:	
	/**
	 * @brief init
	 */	 	
    static GResult init();
	
	/**
	 * @brief uninit
	 */	 	
    static GResult uninit();
	
	/**
	 * @brief print log
	 * @param [in] module : module name
	 * @param [in] file : file path
	 * @param [in] line : line number
	 * @param [in] function : function name
	 * @param [in] args : log content
	 */	 	
    static void printError(const GInt8* module, 
        const GInt8* file, 
        const GUint32 line, 
        const GInt8* function,
        const GInt8* args, ...);

	/**
	 * @brief print log
	 * @param [in] module : module name
	 * @param [in] file : file path
	 * @param [in] line : line number
	 * @param [in] function : function name
	 * @param [in] args : log content
	 */	 	
    static void printWarn(const GInt8* module, 
        const GInt8* file, 
        const GUint32 line, 
        const GInt8* function,
        const GInt8* args, ...);   

	/**
	 * @brief print log
	 * @param [in] module : module name
	 * @param [in] file : file path
	 * @param [in] line : line number
	 * @param [in] function : function name
	 * @param [in] args : log content
	 * @note 
	 */	 	
    static void printInfo(const GInt8* module, 
        const GInt8* file, 
        const GUint32 line, 
        const GInt8* function,
        const GInt8* args, ...);  

	/**
	 * @brief print log
	 * @param [in] module : module name
	 * @param [in] file : file path
	 * @param [in] line : line number
	 * @param [in] function : function name
	 * @param [in] args : log content
	 */	 	
    static void printDebug(const GInt8* module, 
        const GInt8* file, 
        const GUint32 line, 
        const GInt8* function,
        const GInt8* args, ...);  

	/**
	 * @brief print log
	 * @param [in] module : module name
	 * @param [in] file : file path
	 * @param [in] line : line number
	 * @param [in] function : function name
	 * @param [in] args : log content
	 */	 	
    static void printTrace(const GInt8* module, 
        const GInt8* file, 
        const GUint32 line, 
        const GInt8* function,
        const GInt8* args, ...);    

private:
    static void printLog(const GInt32 logLevel, 
        const GInt8* module, 
        const GInt8* file, 
        const GUint32 line, 
        const GInt8* function,
        const GInt8* args, ...);  
};

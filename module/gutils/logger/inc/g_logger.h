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

#include <g_system.h>

/**
 * init log system
 */	
#define G_LOG_INIT() GLogger::init()

/**
 * uninit log system
 */	    
#define G_LOG_UNINIT() GLogger::uninit()

/**
 * print error log
 * @param [in] module : module name
 * @param [in] args : parameters
 */	
#define G_LOG_ERROR(module, args...) GLogger::printError(module, __FILE__, __LINE__, __FUNCTION__, ##args)

/**
 * print warning log
 * @param [in] module : module name
 * @param [in] args : parameters
 */	
#define G_LOG_WARN(module, args...) GLogger::printWarn(module, __FILE__, __LINE__, __FUNCTION__, ##args)

/**
 * print info log
 * @param [in] module : module name
 * @param [in] args : parameters
 */	
#define G_LOG_INFO(module, args...) GLogger::printInfo(module, __FILE__, __LINE__, __FUNCTION__, ##args)

/**
 * print debug log
 * @param [in] module : module name
 * @param [in] args : parameters
 */	
#define G_LOG_DEBUG(module, args...) GLogger::printDebug(module, __FILE__, __LINE__, __FUNCTION__, ##args)

/**
 * print trace log
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
     * init
     */	 	
    static GResult init();
	
    /**
     * uninit
     */	 	
    static GResult uninit();
	
    /**
     * print log
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
     * print log
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
     * print log
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
     * print log
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

/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_sys.h
* @version     
* @brief      
* @author   duye
* @date     2014-03-04
* @note 
*
*  2. 2014-06-20 duye move to gohoop project 
*  1. 2014-03-04 duye Created this file
* 
*/

#pragma once

#include <g_type.h>

/** 
 * @brief POSIX GSystem common API wrapper
 */
class GSys
{
public:
    GSys() {}
    ~GSys() {}
   
    /**
     * @brief sleep
	 * @param [in] time : second
     * @note 
     */		
    static void sleep(const GUint64 time);
 
    /**
     * @brief usleep
	 * @param [in] time : millisecond
     * @note 
     */		
    static void usleep(const GUint64 time);

    /**
     * @brief usleep
	 * @param [out] buffer : out buffer
	 * @param [in] size : out buffer size
	 * @param [in] args : parameters
	 * @return string size
     * @note 
     */		
    static GUint64 format(GInt8* buffer, const GUint64 size, const GInt8* args, ...);    
};

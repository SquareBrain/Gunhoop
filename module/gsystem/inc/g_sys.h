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

#include <unistd.h>
#include <g_type.h>

/** 
 * POSIX GSystem common API wrapper
 */
class GSys
{
public:
    GSys() {}
    ~GSys() {}
   
    /**
     * sleep
	 * @param [in] time : second
     * @note 
     */		
    static void gsleep(const GUint64 time);
 
    /**
     * usleep
	 * @param [in] time : millisecond
     * @note 
     */		
    static void gusleep(const GUint64 time);
};
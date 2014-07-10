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

G_NS_GCOMMON_BEG

/** 
 * POSIX system common API wrapper
 */
class Sys
{
public:
    Sys() {}
    ~Sys() {}
   
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

G_NS_END
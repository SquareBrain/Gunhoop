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

	// brief : sleep
	// @para time : second
	// @return 
	// note:    
    static void Sleep(const GUint64 time);

	// brief : usleep  
	// @para time : millisecond
	// @return 
	// note:    
    static void Usleep(const GUint64 time);
};

G_NS_GCOMMON_END

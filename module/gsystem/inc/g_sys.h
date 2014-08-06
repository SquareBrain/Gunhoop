/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_sys.h
* @version     
* @brief      
* @author	duye
* @date		2014-03-04
* @note 
*
*  2. 2014-06-20 duye move to gohoop project 
*  1. 2014-03-04 duye Created this file
* 
*/

#pragma once

#include <g_type.h>

typedef enum
{
	G_SHELL_R = 1,
	G_SHELL_W
} GShellMode;

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

	/**
	 * @brief execute shell commond
	 * @param [in] cmd : commond line
	 * @param [in] mode : read/write for pipe
	 * @param [out] buffer : output buffer
	 * @param [in] size : output buffer size
	 * @return G_YES/G_NO
	 * @note 
	 */		
	static GResult shell(const GInt8* cmd, const GShellMode mode, GInt8* buffer, const GUint32 size);   
	
	/**
	 * @brief get system time
	 * @return time(microsecond)
	 * @note 
	 */		
	static GUint64 getSysTime();   
};

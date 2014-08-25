/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_timeconv.h
* @version     
* @brief      
* @author	duye
* @date		2014-03-09
* @note 
*
*  2. 2014-06-21 duye move to gohoop project 
*  1. 2014-03-09 duye Created this file
* 
*/
#pragma once

#include <g_system.h>
#include <g_namespace.h>

namespace gcom {

/** 
 * @brief time convert wrapper
 * time format:
 * ye - year
 * mo - mouth
 * da - day
 * ho - hour
 * mi - minute
 * se - second
 * ms - millisecond
 * ns - nansecond
 * we - week
 * 
 * time format e.g : "ye-mo-da ho:mi:se:ms"
 */
class TimeConv
{
public:
	/**
	 * @brief get system recording to time format
	 * @param [in]format : e.g "ye-mo-da ho:mi:se:ms"
	 * @param [out]buffer : time string recording to format
	 * @param [in]size : out buffer size
	 * @return G_YES/G_NO
	 * @note 
	 */			
	static GResult convTime(const GInt8* format, GInt8* buffer, const GUint32 size);
	
	/**
	 * @brief convert time recording to time format
	 * @param [in]millisecond : input time, unit is millisecond
	 * @param [in]format : e.g "ye-mo-da ho:mi:se:ms"
	 * @param [out]buffer : time string recording to format
	 * @param [in]size : out buffer size
	 * @return G_YES/G_NO
	 * @note 
	 */			
	static GResult convTime(const GUint64 millisecond, const GInt8* format, GInt8* buffer, const GUint32 size);	
};
}

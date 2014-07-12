/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_timeconv.h
* @version     
* @brief      
* @author   duye
* @date     2014-03-09
* @note 
*
*  2. 2014-06-21 duye move to gohoop project 
*  1. 2014-03-09 duye Created this file
* 
*/
#pragma once

#include <g_type.h>
#include <g_namespace.h>

G_NS_GCOMMON_BEG

/** 
 * time convert wrapper
 */
class TimeConv
{
public:
    /**
     * get system time format:20140309
	 * @param [in]sysTime : input system time with nanosecond
	 * @param [out]sysTimeStr : output system time with nanosecond
	 * @param [in] bufSize : output buffer size
	 * @return G_YES/G_NO
     * @note 
     */			
    static GResult convTimeA(const GUint64 inSysTime, GInt8* outSysTime, const GUint64 bufSize);  

    /**
     * get system time format:2014-03-09
	 * @param [in]sysTime : input system time with nanosecond
	 * @param [out]sysTimeStr : output system time with nanosecond
	 * @param [in] bufSize : output buffer size
	 * @return G_YES/G_NO
     * @note 
     */		
    static GResult convTimeB(const GUint64 inSysTime, GInt8* outSysTime, const GUint64 bufSize); 
 
    /**
     * get system time format:2014/03/09
	 * @param [in]sysTime : input system time with nanosecond
	 * @param [out]sysTimeStr : output system time with nanosecond
	 * @param [in] bufSize : output buffer size
	 * @return G_YES/G_NO
     * @note 
     */		
    static GResult convTimeC(const GUint64 inSysTime, GInt8* outSysTime, const GUint64 bufSize); 
  
    /**
     * get system time format:2014.03.09
	 * @param [in]sysTime : input system time with nanosecond
	 * @param [out]sysTimeStr : output system time with nanosecond
	 * @param [in] bufSize : output buffer size
	 * @return G_YES/G_NO
     * @note 
     */		
    static GResult convTimeD(const GUint64 inSysTime, GInt8* outSysTime, const GUint64 bufSize);  

    /**
     * get system time format:2014-03-09:999
	 * @param [in]sysTime : input system time with nanosecond
	 * @param [out]sysTimeStr : output system time with nanosecond
	 * @param [in] bufSize : output buffer size
	 * @return G_YES/G_NO
     * @note 
     */		
    static GResult convTimeE(const GUint64 inSysTime, GInt8* outSysTime, const GUint64 bufSize);
};

G_NS_END

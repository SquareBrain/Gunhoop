/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_time.h
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

G_NS_GCOMMON_BEG

// brief : POSIX condition wrapper
class Time
{
public:
    Time();
    ~Time();

 	// brief : get system time eg:20140309
	// @para [out]sysTime : output system time
	// @para [in]bufSize : sysTime buffer size
	// @return true/false
	// note:    
    bool GetTimeA(GInt8* sysTime, const GUint64 bufSize);  

 	// brief : get system time eg:2014-03-09
	// @para [out]sysTime : output system time
	// @para [in]bufSize : sysTime buffer size
	// @return true/false
	// note:    
    bool GetTimeB(GInt8* sysTime, const GUint64 bufSize); 

 	// brief : get system time eg:2014/03/09
	// @para [out]sysTime : output system time
	// @para [in]bufSize : sysTime buffer size
	// @return true/false
	// note:    
    bool GetTimeC(GInt8* sysTime, const GUint64 bufSize); 

 	// brief : get system time eg:2014.03.09
	// @para [out]sysTime : output system time
	// @para [in]bufSize : sysTime buffer size
	// @return true/false
	// note:    
    bool GetTimeD(GInt8* sysTime, const GUint64 bufSize);  

 	// brief : get system time eg:2014-03-09:999
	// @para [out]sysTime : output system time
	// @para [in]bufSize : sysTime buffer size
	// @return true/false
	// note:    
    bool GetTimeE(GInt8* sysTime, const GUint64 bufSize);
};

G_NS_GCOMMON_END

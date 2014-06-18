/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		duye_system_time.h
* @version     
* @brief      
* @author   duye
* @date     2014-03-09
* @note 
*
*  1. 2014-03-09 duye Created this file
* 
*/

#pragma once

#include <duye/posix/inc/duye_posix_def.h>

DUYE_POSIX_NS_BEG

// brief : POSIX condition wrapper
class SystemTime
{
public:
    SystemTime();
    ~SystemTime();

 	// brief : get system time eg:20140309
	// @para [out]sysTime : output system time
	// @para [in]bufSize : sysTime buffer size
	// @return true/false
	// note:    
    D_Bool GetSystemTimeA(D_Int8* sysTime, const D_UInt64 bufSize);  

 	// brief : get system time eg:2014-03-09
	// @para [out]sysTime : output system time
	// @para [in]bufSize : sysTime buffer size
	// @return true/false
	// note:    
    D_Bool GetSystemTimeB(D_Int8* sysTime, const D_UInt64 bufSize); 

 	// brief : get system time eg:2014/03/09
	// @para [out]sysTime : output system time
	// @para [in]bufSize : sysTime buffer size
	// @return true/false
	// note:    
    D_Bool GetSystemTimeC(D_Int8* sysTime, const D_UInt64 bufSize); 

 	// brief : get system time eg:2014.03.09
	// @para [out]sysTime : output system time
	// @para [in]bufSize : sysTime buffer size
	// @return true/false
	// note:    
    D_Bool GetSystemTimeD(D_Int8* sysTime, const D_UInt64 bufSize);  

 	// brief : get system time eg:2014-03-09:999
	// @para [out]sysTime : output system time
	// @para [in]bufSize : sysTime buffer size
	// @return true/false
	// note:    
    D_Bool GetSystemTimeE(D_Int8* sysTime, const D_UInt64 bufSize);
};

DUYE_POSIX_NS_END

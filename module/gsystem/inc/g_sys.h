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

namespace gsys {

class System
{
public:
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
	static GUint64 pformat(GInt8* buffer, const GUint64 size, const GInt8* args, ...);   
	
	/**
	 * @brief execute shell commond
	 * @param [in] cmd : commond line
	 * @return G_YES/G_NO
	 * @note 
	 */		
	static GResult shell(const GInt8* cmd);  
	
	/**
	 * @brief execute shell commond
	 * @param [in] cmd : commond line
	 * @param [out] buffer : output buffer
	 * @param [in] size : output buffer size
	 * @return G_YES/G_NO
	 * @note 
	 */		
	static GResult shell(const GInt8* cmd, GInt8* buffer, const GUint32 size);   
	
	/**
	 * @brief get system time
	 * @return time(microsecond)
	 * @note 
	 */		
	static GUint64 getSysTime();   
	
	/**
	 * @brief get system time
	 * @param [in] format : output time format
	 * @param [out] buffer : output string time
	 * @param [in] size : output buffer size
	 * @return void
	 * @note 
	 * format1 "h:m:s"
	 * format2 "h:m:s:ms"
	 * format3 "h:m:s:ms:us"
	 * format3 "y-m-d h:m:s"
	 * format4 "y-m-d h:m:s:ms"
	 * format5 "y-m-d h:m:s:ms:us"
	 * 
	 * format1 "h:m:s w"
	 * format2 "h:m:s:ms w"
	 * format3 "h:m:s:ms:us w"
	 * format3 "y-m-d h:m:s w"
	 * format4 "y-m-d h:m:s:ms w"
	 * format5 "y-m-d h:m:s:ms:us w"
	 * 
	 * y:yead
	 * m:mouth
	 * d:day
	 * w:week
	 * h:hour
	 * m:minute
	 * s:second
	 * ms:millisecond
	 * us:microsecond
	 */		
	static GResult getSysTime(const GInt8* format, GInt8* buffer, const GUint32 size);
	
	/**
	 * @brief getopt
	 * @param [in] argc : argument count
	 * @param [in] argv : argument list
	 * @param [in] cmd : need parser command
	 * @param [out] value : out value
	 * @return G_YES/G_NO
	 */
	static GResult getOptArg(GInt32 argc, GInt8** argv, GInt8* cmd, GInt64& value);
	static GResult getOptArg(GInt32 argc, GInt8** argv, GInt8* cmd, GUInt64& value);
	static GResult getOptArg(GInt32 argc, GInt8** argv, GInt8* cmd, std::string& value);
};

}

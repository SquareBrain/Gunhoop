/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		sys.h
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

G_NS_GCOMMON_BEG

static const GUint32 MAX_FILE_NAME_SIZE = 127;

// brief : POSIX condition wrapper
class System
{
public:
    System() {}
    ~System() {}

 	// brief : get max file name size
	// @para 
	// @return file name size
	// note:    
    static GUint32 GetMaxFileNameSize();     
    
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

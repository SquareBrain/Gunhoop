/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		duye_system.h
* @version     
* @brief      
* @author   duye
* @date     2014-03-04
* @note 
*
*  1. 2014-03-04 duye Created this file
* 
*/

#pragma once

#include <unistd.h>
#include <duye/posix/inc/duye_posix_def.h>

DUYE_POSIX_NS_BEG

static const D_UInt32 MAX_FILE_NAME_SIZE = 127;

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
    static D_UInt32 GetMaxFileNameSize();     
    
	// brief : sleep
	// @para time : second
	// @return 
	// note:    
    static D_Void Sleep(const D_UInt64 time);

	// brief : usleep  
	// @para time : millisecond
	// @return 
	// note:    
    static D_Void Usleep(const D_UInt64 time);
};

DUYE_POSIX_NS_END

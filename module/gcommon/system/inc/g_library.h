/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		library.h
* @version     
* @brief      
* @author   duye
* @date     2014-06-20
* @note 
*
*  1. 2014-06-20 duye Created this file
* 
*/

#pragma once

#include <dlfcn.h>

G_NS_GCOMMON_BEG

// brief : shared library wrapper
class SharedLibrary
{
public:
    SharedLibrary();
    ~SharedLibrary();

 	// brief : load shared library
	// @para [in]libFilePath : library path
	// @para [in]flags : RTLD_LAZY|RTLD_NOW
	// @return library handle on success, or NULL on error
    static void* OpenLib(const GInt8* libFilePath, const GInt32 flags = RTLD_LAZY);

 	// brief : close shared library
	// @para [in]handle : library handle
	// @return true on success, or false on error    
    static bool CloseLib(void* handle);
};

G_NS_GCOMMON_END

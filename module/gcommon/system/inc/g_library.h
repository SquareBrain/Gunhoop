/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_library.h
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
#include <g_type.h>

G_NS_GCOMMON_BEG

/** 
 * shared library wrapper
 */
class Library
{
public:
    Library();
    ~Library();

    /**
     * load shared library
	 * @param [in] libFilePath : library path
	 * @param [in] flags : RTLD_LAZY|RTLD_NOW
     * @return handle/NULL
     * @note 
     */		
    static void* openLib(const GInt8* libFilePath, const GInt32 flags = RTLD_LAZY);

    /**
     * close shared library
	 * @param [in] handle : library handle
     * @return true/false
     * @note 
     */		
    static bool closeLib(void* handle);
};

G_NS_END
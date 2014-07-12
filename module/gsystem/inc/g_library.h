/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_GLibrary.h
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

/** 
 * shared GLibrary wrapper
 */
class GLibrary
{
public:
    GLibrary();
    ~GLibrary();

    /**
     * load shared GLibrary
	 * @param [in] libFilePath : GLibrary path
	 * @param [in] flags : RTLD_LAZY|RTLD_NOW
     * @return handle/NULL
     * @note 
     */		
    static void* openLib(const GInt8* libFilePath, const GInt32 flags = RTLD_LAZY);

    /**
     * close shared GLibrary
	 * @param [in] handle : GLibrary handle
     * @return true/false
     * @note 
     */		
    static bool closeLib(void* handle);
};
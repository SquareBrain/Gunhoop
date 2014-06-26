/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_library.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-06-20
* @note 
*
*  1. 2014-06-20 duye Created this file
* 
*/

#include <g_library.h>

static const GInt8* LOG_PREFIX = "gohoop.gcommon.system.library";

G_NS_GCOMMON_BEG

Library::Library()
{
}

Library::~Library()
{
}

void* Library::openLib(const GInt8* libFilePath, const GInt32 flags)
{
    return dlopen(libFilePath, flags);
}

bool Library::closeLib(void* handle)
{
    return dlclose(handle) == 0 ? true : false;    
}

G_NS_END

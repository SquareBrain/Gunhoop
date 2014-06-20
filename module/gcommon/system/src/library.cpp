/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		library.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-06-20
* @note 
*
*  1. 2014-06-20 duye Created this file
* 
*/

#include "library.h"

G_NS_GCOMMON_BEG

SharedLibrary::SharedLibrary()
{
}

SharedLibrary::~SharedLibrary()
{
}

void* SharedLibrary::OpenLib(const GInt8* libFilePath, const GInt32 flags)
{
    return dlopen(libFilePath, flags);
}

bool SharedLibrary::CloseLib(void* handle)
{
    return dlclose(handle) == 0 ? true : false;    
}

G_NS_GCOMMON_END

/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_GLibrary.cpp
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

GLibrary::GLibrary()
{
}

GLibrary::~GLibrary()
{
}

void* GLibrary::openLib(const GInt8* libFilePath, const GInt32 flags)
{
    return dlopen(libFilePath, flags);
}

bool GLibrary::closeLib(void* handle)
{
    return dlclose(handle) == 0 ? true : false;    
}

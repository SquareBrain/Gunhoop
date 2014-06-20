/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		duye_shared_library.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-03-19
* @note 
*
*  1. 2014-03-19 duye Created this file
* 
*/

#include <duye/posix/system/inc/duye_shared_library.h>

DUYE_POSIX_NS_BEG

SharedLibrary::SharedLibrary()
{
}

SharedLibrary::~SharedLibrary()
{
}

D_Void* SharedLibrary::OpenLib(const D_Int8* libFilePath, const D_Int32 flags)
{
    return dlopen(libFilePath, flags);
}

D_Bool SharedLibrary::CloseLib(D_Void* handle)
{
    return dlclose(handle) == 0 ? true : false;    
}

DUYE_POSIX_NS_END

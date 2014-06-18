/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		duye_file.cpp
* @version     
* @brief      
* @author   duye
* @date     2013-11-16
* @note 
*
*  1. 2013-11-16 duye Created this file
* 
*/

#include <duye/posix/stdio/inc/duye_file.h>

DUYE_POSIX_NS_BEG

File::File() : m_fd(-1), m_flags(0)
{
    m_path[0] = '\0';
}

File::~File() 
{
    Close();
}

D_Bool File::Create(const D_Int8* filePath)
{
    D_Int32 fd = creat(filePath, D_CREATE_MODE);
    if (fd != -1)
    {
        close(fd);
    }
    
    return (fd != -1 ? true : false);
}

D_Bool File::OpenR(const D_Int8* filePath)
{
    const D_Int32 flags = O_RDONLY;
    return OrgOpen(filePath, flags);
}

D_Bool File::OpenW(const D_Int8* filePath)
{
    const D_Int32 flags = O_WRONLY;
    return OrgOpen(filePath, flags);
}

D_Bool File::OpenWA(const D_Int8* filePath)
{
    const D_Int32 flags = O_WRONLY | O_APPEND;
    return OrgOpen(filePath, flags);
}

D_Bool File::OpenWC(const D_Int8* filePath)
{
    const D_Int32 flags = O_WRONLY | O_CREAT;
    return OrgOpen(filePath, flags, D_CREATE_MODE);
}

D_Bool File::OpenWCA(const D_Int8* filePath)
{
    const D_Int32 flags = O_WRONLY | O_CREAT | O_APPEND;
    return OrgOpen(filePath, flags, D_CREATE_MODE);
}

D_Bool File::OpenRW(const D_Int8* filePath)
{
    const D_Int32 flags = O_RDWR;
    return OrgOpen(filePath, flags);
}

D_Bool File::OpenRWA(const D_Int8* filePath)
{
    const D_Int32 flags = O_RDWR | O_APPEND;
    return OrgOpen(filePath, flags);
}

D_Bool File::OpenRWC(const D_Int8* filePath)
{
    const D_Int32 flags = O_RDWR | O_CREAT;
    return OrgOpen(filePath, flags, D_CREATE_MODE);
}

D_Bool File::OpenRWCA(const D_Int8* filePath)
{
    const D_Int32 flags = O_RDWR | O_CREAT | O_APPEND;
    return OrgOpen(filePath, flags, D_CREATE_MODE);
}

D_UInt64 File::Read(D_Int8* buffer, const D_UInt64 size)
{
    D_ASSERT(buffer != NULL && size > 0);
    return read(m_fd, buffer, size);
}

D_UInt64 File::Write(const D_Int8* buffer, const D_UInt64 size)
{
    D_ASSERT(buffer != NULL && size > 0);
    return write(m_fd, buffer, size);
}

D_Bool File::Close()
{
    D_Bool ret = true;
    if (m_fd == -1)
    {
        return ret;
    }

    ret = (close(m_fd) == 0 ? true : false);
    m_fd = -1;
    m_path[0] = '\0';
    m_flags = 0;

    return ret;
}

D_Bool File::OrgOpen(const D_Int8* filePath, const D_Int32 flags, const D_UInt32 mode)
{
    D_ASSERT(filePath != NULL);
    
    if (m_fd != -1)
    {
        return true;
    }

    D_UInt32 pathLen = strlen(filePath);
    if (pathLen >= D_PATH_MAX)
    {
        return false;
    }
    
    m_fd = open(filePath, flags, mode);
    if (m_fd != -1)
    {
        memcpy(m_path, filePath, pathLen + 1);
        m_flags = flags;   
    }

    return (m_fd != -1 ? true : false);
}

DUYE_POSIX_NS_END

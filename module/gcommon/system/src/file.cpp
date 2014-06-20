/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		file.cpp
* @version     
* @brief      
* @author   duye
* @date     2013-06-20
* @note 
*
*  1. 2013-06-20 duye Created this file
* 
*/

#include "file.h"

G_NS_GCOMMON_BEG

File::File() : m_fd(-1), m_flags(0)
{
    m_path[0] = '\0';
}

File::~File() 
{
    Close();
}

bool File::Create(const GInt8* filePath)
{
    GInt32 fd = creat(filePath, D_CREATE_MODE);
    if (fd != -1)
    {
        close(fd);
    }
    
    return (fd != -1 ? true : false);
}

bool File::OpenR(const GInt8* filePath)
{
    const GInt32 flags = O_RDONLY;
    return OrgOpen(filePath, flags);
}

bool File::OpenW(const GInt8* filePath)
{
    const GInt32 flags = O_WRONLY;
    return OrgOpen(filePath, flags);
}

bool File::OpenWA(const GInt8* filePath)
{
    const GInt32 flags = O_WRONLY | O_APPEND;
    return OrgOpen(filePath, flags);
}

bool File::OpenWC(const GInt8* filePath)
{
    const GInt32 flags = O_WRONLY | O_CREAT;
    return OrgOpen(filePath, flags, D_CREATE_MODE);
}

bool File::OpenWCA(const GInt8* filePath)
{
    const GInt32 flags = O_WRONLY | O_CREAT | O_APPEND;
    return OrgOpen(filePath, flags, D_CREATE_MODE);
}

bool File::OpenRW(const GInt8* filePath)
{
    const GInt32 flags = O_RDWR;
    return OrgOpen(filePath, flags);
}

bool File::OpenRWA(const GInt8* filePath)
{
    const GInt32 flags = O_RDWR | O_APPEND;
    return OrgOpen(filePath, flags);
}

bool File::OpenRWC(const GInt8* filePath)
{
    const GInt32 flags = O_RDWR | O_CREAT;
    return OrgOpen(filePath, flags, D_CREATE_MODE);
}

bool File::OpenRWCA(const GInt8* filePath)
{
    const GInt32 flags = O_RDWR | O_CREAT | O_APPEND;
    return OrgOpen(filePath, flags, D_CREATE_MODE);
}

GUint64 File::Read(GInt8* buffer, const GUint64 size)
{
    D_ASSERT(buffer != NULL && size > 0);
    return read(m_fd, buffer, size);
}

GUint64 File::Write(const GInt8* buffer, const GUint64 size)
{
    D_ASSERT(buffer != NULL && size > 0);
    return write(m_fd, buffer, size);
}

bool File::Close()
{
    bool ret = true;
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

bool File::OrgOpen(const GInt8* filePath, const GInt32 flags, const GUint32 mode)
{
    D_ASSERT(filePath != NULL);
    
    if (m_fd != -1)
    {
        return true;
    }

    GUint32 pathLen = strlen(filePath);
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

G_NS_GCOMMON_END

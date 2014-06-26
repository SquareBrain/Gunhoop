/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_file.cpp
* @version     
* @brief      
* @author   duye
* @date     2013-06-20
* @note 
*
*  1. 2013-06-20 duye Created this file
* 
*/

#include <g_file.h>

static const GInt8* LOG_PREFIX = "gohoop.gcommon.system.file";
static const GUint32 G_CREATE_MODE = 0x775;

G_NS_GCOMMON_BEG

GResult FileUtil::createFile(const GInt8* filePath)
{
    GInt32 fd = creat(filePath, G_CREATE_MODE);
    if (fd != -1)
    {
        close(fd);
        return G_YES;
    }
    
    return G_NO;
}

File::File() : m_fd(-1), m_flags(0), m_mode(0), m_pathLen(0)
{
    m_path[0] = 0;
}

File::File(const GInt8* filePath)
{
    GUint32 len = strlen(filePath);
    if (len < G_PATH_MAX)
    {
        memcpy(m_path, filePath, len);
        m_path[len] = 0;
        m_pathLen = len;
    }
}

File::~File() 
{
    Close();
}

GResult File::setFilePath(const GInt8* filePath)
{
    if (m_pathLen > 0)
    {
        return G_NO;
    }
    
    GUint32 len = strlen(filePath);
    if (len < G_PATH_MAX)
    {
        memcpy(m_path, filePath, len);
        m_path[len] = 0;
        m_pathLen = len;
    }    
    else
    {
        return G_NO;   
    }

    return G_YES;
}

GUint32 File::getMode() const
{
    return m_mode;
}

GResult File::openR()
{
    return orgOpen(O_RDONLY);
}

GResult File::openW()
{
    return orgOpen(O_WRONLY);
}

GResult File::openWA()
{
    return orgOpen(O_WRONLY | O_APPEND);
}

GResult File::openWC()
{
    return orgOpen(O_WRONLY | O_CREAT, G_CREATE_MODE);
}

GResult File::openWCA()
{
    return orgOpen(O_WRONLY | O_CREAT | O_APPEND, G_CREATE_MODE);
}

GResult File::openRW()
{
    return orgOpen(O_RDWR);
}

GResult File::openRWA()
{
    return orgOpen(O_RDWR | O_APPEND);
}

GResult File::openRWC()
{
    return orgOpen(O_RDWR | O_CREAT, G_CREATE_MODE);
}

GResult File::openRWCA()
{
    return orgOpen(O_RDWR | O_CREAT | O_APPEND, G_CREATE_MODE);
}

GInt64 File::readFile(GInt8* buffer, const GUint64 size)
{
    G_ASSERT(buffer != NULL && size > 0);

    if (m_fd <= 0)
    {
        return G_NO;
    }
    
    return read(m_fd, buffer, size);
}

GInt64 File::readLine(const GUint64 offset, GInt8* buffer, const GUint64 size)
{
    G_ASSERT(buffer != NULL && size > 0);
    
    if (m_fd <= 0)
    {
        return G_NO;
    }
    
    return read(m_fd, buffer, size);    
}

GInt64 File::writeFile(const GInt8* buffer, const GUint64 size)
{
    G_ASSERT(buffer != NULL && size > 0);

    if (m_fd <= 0)
    {
        return G_NO;
    }
    
    return write(m_fd, buffer, size);
}

GResult File::closeFile()
{
    if (m_fd < 0)
    {
        return G_NO;
    }

    GResult ret = (close(m_fd) != -1 ? G_YES : G_NO);
    
    m_fd = -1;
    m_path[0] = 0;
    m_flags = 0;

    return ret;
}

GResult File::orgOpen(const GInt32 flags, const GUint32 mode)
{    
    if (m_fd > 0)
    {
        return G_NO;
    }

    if (m_pathLen == 0)
    {
        return G_NO;   
    }
    
    m_fd = open(m_path, flags, mode);
    if (m_fd > 0)
    {
        m_flags = flags;   
        m_mode = mode;
    }

    return (m_fd != -1 ? true : false);
}

G_NS_END

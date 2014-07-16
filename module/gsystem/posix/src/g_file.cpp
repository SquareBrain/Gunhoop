/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @GFile		g_file.cpp
* @version     
* @brief      
* @author   duye
* @date     2013-06-20
* @note 
*
*  1. 2013-06-20 duye Created this GFile
* 
*/
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <g_file.h>

// default create GFile permissions
static const GUint32 G_CREATE_MODE = 0x775;

GResult GFileUtil::createFile(const GInt8* filePath)
{
    GInt32 fd = creat(filePath, G_CREATE_MODE);
    if (fd != -1)
    {
        ::close(fd);
        return G_YES;
    }
    
    return G_NO;
}

GResult GFileUtil::createFile(const GInt8* filePath, const GUint64& initSize)
{
    
    return G_NO;
}

GFile::GFile() : m_fd(-1), m_flags(0), m_pathLen(0)
{
    m_error[0] = 0;
    m_path[0] = 0;
}

GFile::GFile(const GInt8* filePath) : m_fd(-1), m_flags(0), m_pathLen(0)
{
    GUint32 len = strlen(filePath);
    if (len < G_PATH_MAX)
    {
        memcpy(m_path, filePath, len);
        m_path[len] = 0;
        m_pathLen = len;
    }

    m_error[0] = 0;
}

GFile::~GFile() 
{
    close();
}

GResult GFile::setPath(const GInt8* filePath)
{
    if (m_pathLen > 0)
    {   
        setError("file path has exist");
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
        setError("file path too long");
        return G_NO;   
    }

    return G_YES;
}

GResult GFile::open(const GUint64 flags)
{
    GInt32 openFlags = 0;
    if (flags | G_OPEN_READ)
    {
        openFlags = O_RDONLY;
    }
    else if (flags | G_OPEN_WRITE)
    {
        openFlags = O_WRONLY | O_CREAT;
    }
    else if (flags | G_OPEN_RDWR)
    {
        openFlags = O_RDWR | O_CREAT;        
    }
    else if (flags | G_OPEN_APPEND)
    {
        if (openFlags == 0)
        {
            return G_NO;
        }

        openFlags |= O_APPEND;
    }

    if (openFlags == 0)
    {
        setError("input open mode error");
        return G_NO;
    }
    
    return orgOpen(openFlags, G_CREATE_MODE);          
}

GResult GFile::close()
{
    if (m_fd < 0)
    {
        setError("file don't open");
        return G_NO;
    }

    GResult ret = (::close(m_fd) != -1 ? G_YES : G_NO);
    
    m_fd = -1;
    m_path[0] = 0;
    m_flags = 0;

    return ret;
}

GInt64 GFile::getSize()
{
    if (m_fd <= 0)
    {
        setError("file don't open");
        return G_NO;
    }    

    return (GInt64)(m_fileStat.st_size);
}

GInt64 GFile::seek(const GInt64 offset, const SeekFlags& flags)
{
    if (m_fd <= 0)
    {
        setError("file don't open");
        return G_NO;
    }  

    GInt32 sysFlags = -1;
    
    switch(flags)
    {
    case G_SEEK_BEG:
        sysFlags = SEEK_SET;
        break;
    case G_SEEK_CUR:
        sysFlags = SEEK_CUR;
        break;
    case G_SEEK_END:
        sysFlags = SEEK_END;
        break;
    default:
        return G_NO;
        break;
    }
    
    return ::lseek(m_fd, offset, sysFlags);
}

GInt64 GFile::read(GInt8* buffer, const GUint64 size)
{
    if (buffer == NULL || size <= 0)
    {
        setError("input parameter is error");
        return G_NO;        
    }

    if (m_fd <= 0)
    {
        setError("file don't open");
        return G_NO;
    }
    
    return ::read(m_fd, buffer, size);
}

GInt64 GFile::write(const GInt8* data, const GUint64 length)
{
    if (data == NULL || length <= 0)
    {
        setError("input parameter is error");
        return G_NO;        
    }

    if (m_fd <= 0)
    {
        setError("file don't open");
        return G_NO;
    }
    
    return ::write(m_fd, data, length);
}

GInt8* GFile::getError()
{
    return m_error;
}

GResult GFile::orgOpen(const GInt32 flags, const GUint32 mode)
{    
    if (m_fd > 0)
    {
        setError("file had opened");
        return G_NO;
    }

    if (m_pathLen == 0)
    {
        setError("hasn't set file path");
        return G_NO;   
    }
    
    m_fd = ::open(m_path, flags, mode);
    if (m_fd > 0)
    {
        m_flags = flags;
        fstat(m_fd, &m_fileStat);
    }
    else
    {
        setError("open file failed, check whether exist this file path");
    }

    return (m_fd != -1 ? true : false);
}

void GFile::setError(const GInt8* args, ...)
{
	GSys::format(m_error, G_ERROR_BUF_SIZE, args);
}

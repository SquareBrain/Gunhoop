/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @GFile		g_GFile.cpp
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

GResult GGFileUtil::createGFile(const GInt8* GFilePath)
{
    GInt32 fd = creat(GFilePath, G_CREATE_MODE);
    if (fd != -1)
    {
        close(fd);
        return G_YES;
    }
    
    return G_NO;
}

GFile::GFile() : m_fd(-1), m_flags(0), m_pathLen(0)
{
    m_error[0] = 0;
    m_path[0] = 0;
}

GFile::GFile(const GInt8* GFilePath) : m_fd(-1), m_flags(0), m_pathLen(0)
{
    GUint32 len = strlen(GFilePath);
    if (len < G_PATH_MAX)
    {
        memcpy(m_path, GFilePath, len);
        m_path[len] = 0;
        m_pathLen = len;
    }

    m_error[0] = 0;
}

GFile::~GFile() 
{
    closeGFile();
}

GResult GFile::setGFilePath(const GInt8* GFilePath)
{
    if (m_pathLen > 0)
    {   
        setError("GFile path has exist");
        return G_NO;
    }
    
    GUint32 len = strlen(GFilePath);
    if (len < G_PATH_MAX)
    {
        memcpy(m_path, GFilePath, len);
        m_path[len] = 0;
        m_pathLen = len;
    }    
    else
    {
        setError("GFile path too long");
        return G_NO;   
    }

    return G_YES;
}

GResult GFile::openGFile(const GUint64 flags)
{
    GInt32 openFlags = 0;
    if (flags | ONLY_READ)
    {
        openFlags = O_RDONLY;
    }
    else if (flags | ONLY_WRITE)
    {
        openFlags = O_WRONLY | O_CREAT;
    }
    else if (flags | READ_WRITE)
    {
        openFlags = O_RDWR | O_CREAT;        
    }
    else if (flags | OPEN_APPEND)
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

GResult GFile::closeGFile()
{
    if (m_fd < 0)
    {
        setError("GFile don't open");
        return G_NO;
    }

    GResult ret = (close(m_fd) != -1 ? G_YES : G_NO);
    
    m_fd = -1;
    m_path[0] = 0;
    m_flags = 0;

    return ret;
}

GInt64 GFile::getGFileSize()
{
    if (m_fd <= 0)
    {
        setError("GFile don't open");
        return G_NO;
    }    

    return (GInt64)(m_GFileStat.st_size);
}

GInt64 GFile::readGFile(GInt8* buffer, const GUint64 size)
{
    if (buffer == NULL || size <= 0)
    {
        setError("input parameter is error");
        return G_NO;        
    }

    if (m_fd <= 0)
    {
        setError("GFile don't open");
        return G_NO;
    }
    
    return read(m_fd, buffer, size);
}

GInt64 GFile::writeGFile(const GInt8* data, const GUint64 length)
{
    if (data == NULL || length <= 0)
    {
        setError("input parameter is error");
        return G_NO;        
    }

    if (m_fd <= 0)
    {
        setError("GFile don't open");
        return G_NO;
    }
    
    return write(m_fd, data, length);
}

GResult GFile::getLastError(GInt8* error, const GUint32 size)
{
    GUint32 len = (size <= strlen(m_error) ? size - 1 : strlen(m_error));
    memcpy(error, m_error, len); 
    error[len] = 0;
    return G_YES;
}

GResult GFile::orgOpen(const GInt32 flags, const GUint32 mode)
{    
    if (m_fd > 0)
    {
        setError("GFile had opened");
        return G_NO;
    }

    if (m_pathLen == 0)
    {
        setError("hasn't set GFile path");
        return G_NO;   
    }
    
    m_fd = open(m_path, flags, mode);
    if (m_fd > 0)
    {
        m_flags = flags;
        fstat(m_fd, &m_GFileStat);
    }
    else
    {
        setError("open GFile failed, check whether exist this GFile path");
    }

    return (m_fd != -1 ? true : false);
}

void GFile::setError(const GInt8* args, ...)
{
    va_list vaList;
	va_start(vaList, args);    
	vsnprintf(m_error, G_ERROR_BUF_SIZE, args, vaList);
    va_end(vaList);	
}

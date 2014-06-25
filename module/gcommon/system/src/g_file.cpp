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

bool FileUtil::createFile(const GInt8* filePath)
{
    GInt32 fd = creat(filePath, G_CREATE_MODE);
    if (fd != -1)
    {
        close(fd);
    }
    
    return (fd != -1 ? true : false);
}

File::File() : m_fd(-1), m_flags(0)
{
    m_path[0] = 0;
}

File::~File() 
{
    Close();
}

bool File::openR(const GInt8* filePath)
{
    return orgOpen(filePath, O_RDONLY);
}

bool File::openW(const GInt8* filePath)
{
    return orgOpen(filePath, O_WRONLY);
}

bool File::openWA(const GInt8* filePath)
{
    return orgOpen(filePath, O_WRONLY | O_APPEND);
}

bool File::openWC(const GInt8* filePath)
{
    return orgOpen(filePath, O_WRONLY | O_CREAT, G_CREATE_MODE);
}

bool File::openWCA(const GInt8* filePath)
{
    return orgOpen(filePath, O_WRONLY | O_CREAT | O_APPEND, G_CREATE_MODE);
}

bool File::openRW(const GInt8* filePath)
{
    return orgOpen(filePath, O_RDWR);
}

bool File::openRWA(const GInt8* filePath)
{
    return orgOpen(filePath, O_RDWR | O_APPEND);
}

bool File::openRWC(const GInt8* filePath)
{
    return orgOpen(filePath, O_RDWR | O_CREAT, G_CREATE_MODE);
}

bool File::openRWCA(const GInt8* filePath)
{
    return orgOpen(filePath, O_RDWR | O_CREAT | O_APPEND, G_CREATE_MODE);
}

GUint64 File::oead(GInt8* buffer, const GUint64 size)
{
    G_ASSERT(buffer != NULL && size > 0);
    return read(m_fd, buffer, size);
}

GUint64 File::writeFile(const GInt8* buffer, const GUint64 size)
{
    G_ASSERT(buffer != NULL && size > 0);
    return write(m_fd, buffer, size);
}

bool File::closeFile()
{
    bool ret = true;
    if (m_fd == -1)
    {
        return ret;
    }

    ret = (close(m_fd) == 0 ? true : false);
    m_fd = -1;
    m_path.clear();
    m_flags = 0;

    return ret;
}

bool File::orgOpen(const GInt8* filePath, const GInt32 flags, const GUint32 mode)
{
    G_ASSERT(filePath != NULL);
    
    if (m_fd != -1)
    {
		if (m_path.compare(filePath) == 0)
		{
			return true;
		}
		else
		{
			closeFile();
			return orgOpen(filePath, flags, mode);
		}
    }

    GUint32 pathLen = strlen(filePath);
    if (pathLen >= G_PATH_MAX)
    {
        return false;
    }
    
    m_fd = open(filePath, flags, mode);
    if (m_fd != -1)
    {
		m_path.assign(filePath);
        m_flags = flags;   
    }

    return (m_fd != -1 ? true : false);
}

G_NS_GCOMMON_END

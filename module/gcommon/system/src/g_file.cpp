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

static const GUint32 G_CREATE_MODE = 0x775;

G_NS_GCOMMON_BEG

File::File() : m_fd(-1), m_flags(0)
{
}

File::~File() 
{
    Close();
}

bool File::Create(const GInt8* filePath)
{
    GInt32 fd = creat(filePath, G_CREATE_MODE);
    if (fd != -1)
    {
        close(fd);
    }
    
    return (fd != -1 ? true : false);
}

bool File::Create(const std::string& filePath)
{
	return Create(filePath.c_str()); 
}

bool File::OpenR(const GInt8* filePath)
{
    return OrgOpen(filePath, O_RDONLY);
}

bool File::OpenR(const std::string& filePath)
{
	return OpenR(filePath.c_str());
}

bool File::OpenW(const GInt8* filePath)
{
    return OrgOpen(filePath, O_WRONLY);
}

bool File::OpenW(const std::string& filePath)
{
	return OpenW(filePath.c_str());
}

bool File::OpenWA(const GInt8* filePath)
{
    return OrgOpen(filePath, O_WRONLY | O_APPEND);
}

bool File::OpenWA(const std::string& filePath)
{
	return OpenWA(filePath.c_str());
}

bool File::OpenWC(const GInt8* filePath)
{
    return OrgOpen(filePath, O_WRONLY | O_CREAT, G_CREATE_MODE);
}

bool File::OpenWC(const std::string& filePath)
{
	return OpenWC(filePath.c_str());
}

bool File::OpenWCA(const GInt8* filePath)
{
    return OrgOpen(filePath, O_WRONLY | O_CREAT | O_APPEND, G_CREATE_MODE);
}

bool File::OpenWCA(const std::string& filePath)
{
	return OpenWCA(filePath.c_str());
}

bool File::OpenRW(const GInt8* filePath)
{
    return OrgOpen(filePath, O_RDWR);
}

bool File::OpenRW(const std::string& filePath)
{
	return OpenRW(filePath.c_str());
}

bool File::OpenRWA(const GInt8* filePath)
{
    return OrgOpen(filePath, O_RDWR | O_APPEND);
}

bool File::OpenRWA(const std::string& filePath)
{
	return OpenRWA(filePath.c_str());
}

bool File::OpenRWC(const GInt8* filePath)
{
    return OrgOpen(filePath, O_RDWR | O_CREAT, G_CREATE_MODE);
}

bool File::OpenRWC(const std::string& filePath)
{
	return OpenRWC(filePath.c_str());
}

bool File::OpenRWCA(const GInt8* filePath)
{
    return OrgOpen(filePath, O_RDWR | O_CREAT | O_APPEND, G_CREATE_MODE);
}

bool File::OpenRWCA(const std::string& filePath)
{
	return OpenRWCA(filePath.c_str());
}

GUint64 File::Read(GInt8* buffer, const GUint64 size)
{
    G_ASSERT(buffer != NULL && size > 0);
    return read(m_fd, buffer, size);
}

GUint64 File::Read(std::string& buffer, const GUint64 size)
{
	buffer.resize(size);
	return read(m_fd, (void*)buffer.c_str(), size);
}

GUint64 File::Write(const GInt8* buffer, const GUint64 size)
{
    G_ASSERT(buffer != NULL && size > 0);
    return write(m_fd, buffer, size);
}

GUint64 File::Write(const std::string& buffer, const GUint64 size)
{
	return Write(buffer.c_str(), size);
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
    m_path.clear();
    m_flags = 0;

    return ret;
}

bool File::OrgOpen(const GInt8* filePath, const GInt32 flags, const GUint32 mode)
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
			Close();
			return OrgOpen(filePath, flags, mode);
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

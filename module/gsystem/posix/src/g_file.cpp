/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @GFile	g_file.cpp
* @version     
* @brief      
* @author	duye
* @date		2013-06-20
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

using namespace gsys;

// default create GFile permissions
static const GUint32 G_FILE_MASK = 0x775;

GResult FileUtil::createFile(const GInt8* filePath)
{
	return createFile(filePath, 0);
}

GResult FileUtil::createFile(const GInt8* filePath, const GUint64& initSize)
{
	GInt32 fd = ::creat(filePath, G_FILE_MASK);
	if (fd == -1)
	{
		return G_NO;
	}

	if (ftruncate(fd, initSize) == -1)
	{
		::close(fd);
		return G_NO;
	}

	::close(fd);

	return G_YES;
}

bool FileUtil::isExist(const GInt8* filePath)
{
	if (filePath == nullptr)
    {
    	return false;
    }
    
	if (access(filePath, 0) < 0)
    {
    	return false;
    }

    return true;
}

GResult FileUtil::removeFile(const GInt8* filePath)
{
	if (filePath == nullptr)
	{
		return G_NO;
	}
	
	GInt8 cmd[128] = {0};
	sprintf(cmd, "rm %s -f", filePath);
	return System::shell(cmd);
}

File::File() : m_fd(-1), m_flags(0), m_pathLen(0)
{
	m_error[0] = 0;
	m_path[0] = 0;
}

File::File(const GInt8* filePath) : m_fd(-1), m_flags(0), m_pathLen(0)
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

File::~File() 
{
	close();
}

GResult File::open(const FileOpenFlags fileOpenFlags)
{
	return open(fileOpenFlags, G_FILE_MASK);          
}

GResult File::open(const FileOpenFlags fileOpenFlags, const GInt32 mode)
{
	GInt32 openFlags = 0;
	if (fileOpenFlags | G_OPEN_READ)
	{
		openFlags = O_RDONLY;
	}
	else if (fileOpenFlags | G_OPEN_WRITE)
	{
		openFlags = O_WRONLY | O_CREAT;
	}
	else if (fileOpenFlags | G_OPEN_RDWR)
	{
		openFlags = O_RDWR | O_CREAT;        
	}
	else if (fileOpenFlags | G_OPEN_APPEND)
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

	return orgOpen(openFlags, mode);          
}

GResult File::close()
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

GInt64 File::getSize()
{
	if (m_fd <= 0)
	{
		setError("file don't open");
		return G_NO;
	} 

    struct stat	fileStat;
	fstat(m_fd, &fileStat); 

	return (GInt64)(fileStat.st_size);
}

GInt64 File::seek(const GInt64 offset, const FileSeekFlags& flags)
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

GInt64 File::tell()
{
	return seek(0, G_SEEK_CUR);
}

GInt64 File::read(GInt8* buffer, const GUint64 size)
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

GInt64 File::write(const GInt8* data, const GUint64 length)
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

GInt8* File::getError()
{
	return m_error;
}

GResult File::orgOpen(const GInt32 flags, const GUint32 mode)
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
	}
	else
	{
		setError("open file failed, check whether exist this file path");
	}

	return (m_fd != -1 ? true : false);
}

void File::setError(const GInt8* args, ...)
{
	System::pformat(m_error, G_ERROR_BUF_SIZE, args);
}

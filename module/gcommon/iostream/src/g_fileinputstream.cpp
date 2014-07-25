/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_fileinputstream.cpp
* @version     
* @brief      
* @author   ylh
* @date     2014-07-11
* @note 
*
*  1. 2014-07-11 ylh Created this file
* 
*/

#include <g_sync.h>
#include <g_fileinputstream.h>

using namespace std;
using namespace GCommon;

FileInputStream::FileInputStream(std::shared_ptr<GFile> file)
	: m_file(file)
	, m_mtx(PTHREAD_MUTEX_NORMAL)
{
	if (m_file == NULL)
	{
		throw invalid_argument(EXCEPTION_DESCRIPTION("invalid_argument"));
	}
}

FileInputStream::FileInputStream(const std::string& filePath)
	: m_file((shared_ptr<GFile>(new GFile(filePath.c_str()))))
{
	if (m_file->open(G_OPEN_READ) == G_NO)
	{
		throw ios_base::failure(EXCEPTION_DESCRIPTION("ios_base::failure"));
	}
}

FileInputStream::~FileInputStream()
{
}

GInt32 FileInputStream::available() throw(std::ios_base::failure)
{
	// TODO: .........
	GInt64 current = m_file->tell();
	GInt64 end = m_file->seek(0, );
	GInt64 available = end - current;
	
	m_file->seek(current, );
	
	return available;
}

void FileInputStream::close() throw(std::ios_base::failure)
{
	// m_file->close();
	// do nothing
}

GInt32 FileInputStream::read() throw(std::ios_base::failure, std::logic_error)
{
	GInt8 buffer[1] = {0};
	return  this->read(buffer, 1);
}

GInt32 FileInputStream::read(GInt8* buffer, GInt32 bufferLen, GInt32 offset, GInt32 len) throw(std::ios_base::failure, std::logic_error)
{
	GInt32 ret = -1;
	synchronized(m_mtx)
	{
		if (!buffer)
		{
			throw invalid_argument(EXCEPTION_DESCRIPTION("invalid_argument"));
		}
		else if (offset < 0 || len < 0 ||bufferLen < offset + len)
		{
			throw out_of_range(EXCEPTION_DESCRIPTION("out_of_range"));
		}
		else if (len == 0)
		{
			return 0;
		}

		ret = m_file->read(buffer + offset, len);
		if (ret < 0)
		{
			throw ios_base::failure(EXCEPTION_DESCRIPTION("ios_base::failure"));
		}
		if (ret == 0)
		{
			ret = -1;
		}
	}	
	return ret;
}

GInt64 FileInputStream::skip(GInt64 num) throw(std::ios_base::failure)
{
	synchronized(m_mtx)
	{
		if (num == 0)
		{
			return 0;
		}
		if (num  < 0)
		{
			throw out_of_range("");
		}

		// TODO: not complete
		if (m_file->seek(num, G_SEEK_CUR))
		{
			throw ios_base::failure(EXCEPTION_DESCRIPTION("ios_base::failure"));
		}	
	}
	return 0;
}

std::shared_ptr<GFile> FileInputStream::GetFile()
{
	return m_file;
}


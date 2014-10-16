/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_bufferinputstream.cpp
* @version     
* @brief      
* @author   ylh
* @date     2014-07-15
* @note 
*
*  1. 2014-07-15 ylh Created this file
* 
*/

#include <algorithm>
#include <g_security_obj.h>
#include <g_bufferinputstream.h>

using namespace std;

namespace gcom {

BufferInputStream::BufferInputStream(GInt8* buffer, GInt32 bufferLen, GInt32 offset, GInt32 len)
	: m_mtx(PTHREAD_MUTEX_NORMAL)
{
	if (!buffer)
	{
		throw invalid_argument(EXCEPTION_DESCRIPTION("invalid_argument"));
		return;
	}
	
	if (bufferLen < 0 ||
		offset < 0 || 
		len < 0 || 
		offset > bufferLen)
	{
		throw out_of_range(EXCEPTION_DESCRIPTION("out_of_range"));
		return;
	}

	m_buffer = buffer;
	m_bufferLen = bufferLen;

	m_count = offset + len;
	if (m_count > bufferLen)
	{
		m_count = bufferLen;
	}

	m_pos = offset;
	m_mark = m_pos;
}

BufferInputStream::BufferInputStream(GInt8* buffer, GInt32 bufferLen)
	: m_mtx(PTHREAD_MUTEX_NORMAL)
{
	if (!buffer)
	{
		throw invalid_argument(EXCEPTION_DESCRIPTION("invalid_argument"));
		return;
	}
	
	if (bufferLen < 0)
	{
		throw out_of_range(EXCEPTION_DESCRIPTION("out_of_range"));
		return;
	}
	
	m_buffer = buffer;
	m_bufferLen = bufferLen;

	m_count = bufferLen;

	m_pos = 0;
	m_mark = m_pos;
}

BufferInputStream::~BufferInputStream()
{
}

GInt32 BufferInputStream::available() throw(std::ios_base::failure)
{
	synchronized(m_mtx)
	{
		return m_count - m_pos;
	}
	return 0;
}

void BufferInputStream::close() throw(std::ios_base::failure)
{
	// do nothing
}

void BufferInputStream::mark(GInt32 readlimit)
{
	synchronized(m_mtx)
	{
		m_mark = m_pos;
	}
}

bool BufferInputStream::markSupported()
{
	return true;
}

GInt32 BufferInputStream::read() throw(std::ios_base::failure, std::logic_error)
{
	synchronized(m_mtx)
	{
		if (m_pos < m_count)
		{
			return static_cast<GInt32>(m_buffer[m_pos ++] & 0xFF);
		}
	}
	return -1;
}

GInt32 BufferInputStream::read(GInt8 * buffer, GInt32 bufferLen, GInt32 offset, GInt32 len) throw(std::ios_base::failure, std::logic_error)
{
	GInt32 num = 0;
	synchronized(m_mtx)
	{
		if (m_pos >= m_count)
		{
			return -1;
		}
		if (!buffer)
		{
			throw invalid_argument(EXCEPTION_DESCRIPTION("invalid_argument"));
		}
		if (bufferLen < 0 || offset < 0 || len < 0 || bufferLen < offset + len)
		{
			throw out_of_range(EXCEPTION_DESCRIPTION("out_of_range"));
		}
		if (len == 0)
		{
			return 0;
		}
		
		num = min(m_count - m_pos, len);
		memcpy(buffer + offset, m_buffer + m_pos, num);
		m_pos += num;
	}
	return num;
}

void BufferInputStream::reset() throw(std::ios_base::failure)
{
	synchronized(m_mtx)
	{
		m_pos = m_mark;
	}
}

GInt64 BufferInputStream::skip(GInt64 num) throw(std::ios_base::failure)
{
	GInt64 ret = 0;
	synchronized(m_mtx)
	{
		if (num < 0)
		{
			num = 0L;
		}
		ret = min(static_cast<GInt64>(m_count - m_pos), num);
		m_pos += ret;
	}
	return ret;
}
}

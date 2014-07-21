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
#include <g_bufferinputstream.h>

using namespace std;
using namespace GCommon;



BufferInputStream::BufferInputStream(GInt8* pBuffer, GInt32 iBufferLen, GInt32 iOffset, GInt32 iLen) throw(std::logic_error)
{
	if (!pBuffer)
	{
		throw invalid_argument("");
		return;
	}
	
	if (iBufferLen < 0 ||
		iOffset < 0 || 
		iLen < 0 || 
		iOffset > iBufferLen)
	{
		throw out_of_range("");
		return;
	}

	m_pBuffer = pBuffer;
	m_iBufferLen = iBufferLen;

	m_iCount = iOffset + iLen;
	if (m_iCount > iBufferLen)
	{
		m_iCount = iBufferLen;
	}

	m_iPos = iOffset;
	m_iMark = m_iPos;
}

BufferInputStream::BufferInputStream(GInt8* pBuffer, GInt32 iBufferLen) throw(std::logic_error)
{
	if (!pBuffer)
	{
		throw invalid_argument("");
		return;
	}
	
	if (iBufferLen < 0)
	{
		throw out_of_range("");
		return;
	}

	m_pBuffer = pBuffer;
	m_iBufferLen = iBufferLen;

	m_iCount = iBufferLen;

	m_iPos = 0;
	m_iMark = m_iPos;
}

BufferInputStream::~BufferInputStream()
{
}

GInt32 BufferInputStream::available() throw(std::ios_base::failure)
{
	return m_iCount - m_iPos;
}

void BufferInputStream::close() throw(std::ios_base::failure)
{
	// do nothing
}

void BufferInputStream::mark(GInt32 iReadlimit)
{
	m_iMark = m_iPos;
}

bool BufferInputStream::markSupported()
{
	return true;
}

GInt32 BufferInputStream::read() throw(std::ios_base::failure, std::logic_error)
{
	if (m_iPos < m_iCount)
	{
		return static_cast<GInt32>(m_pBuffer[m_iPos ++] & 0xFF);
	}
	return -1;
}

GInt32 BufferInputStream::read(GInt8 * pBuffer, GInt32 iBufferLen, GInt32 iOffset, GInt32 iLen) throw(std::ios_base::failure, std::logic_error)
{
	if (m_iPos >= m_iCount)
	{
		return -1;
	}
	if (!pBuffer)
	{
		throw invalid_argument("");
	}
	if (iBufferLen < 0 || iOffset < 0 || iLen < 0 || iBufferLen < iOffset + iLen)
	{
		throw out_of_range("");
	}
	if (iLen == 0)
	{
		return 0;
	}
	
	GInt32 iNum = min(m_iCount - m_iPos, iLen);
	memcpy(pBuffer + iOffset, m_pBuffer + m_iPos, iNum);
	m_iPos += iNum;
	return iNum;
}

void BufferInputStream::reset() throw(std::ios_base::failure)
{
	m_iPos = m_iMark;
}

GInt64 BufferInputStream::skip(GInt64 lNum) throw(std::ios_base::failure)
{
	if (lNum < 0)
	{
		lNum = 0L;
	}
	GInt64 lRet = min(static_cast<GInt64>(m_iCount - m_iPos), lNum);
	m_iPos += lRet;
	return lRet;
}


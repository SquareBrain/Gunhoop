/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_filterinputstream.cpp
* @version     
* @brief      
* @author   ylh
* @date     2014-07-05
* @note 
*
*  1. 2014-07-05 ylh Created this file
* 
*/

#include <g_filterinputstream.h>
#include <g_private_define.h>

using namespace GCommon;



FilterInputStream::FilterInputStream(std::shared_ptr<InputStream> in)
	: m_in(in)
{
	if (!m_in)
	{
		throw invalid_argument(EXCEPTION_DESCRIPTION("invalid_argument"));
	}
}

FilterInputStream::~FilterInputStream()
{
}

GInt32 FilterInputStream::available() throw(std::ios_base::failure)
{
	return m_in->available();
}

void FilterInputStream::close() throw(std::ios_base::failure)
{
	m_in->close();
}

void FilterInputStream::mark(GInt32 iReadlimit)
{
	m_in->mark(iReadlimit);
}

bool FilterInputStream::markSupported()
{	
	return m_in->markSupported();
}

GInt32 FilterInputStream::read() throw(std::ios_base::failure, std::logic_error)
{
	return  m_in->read();
}

GInt32 FilterInputStream::read(GInt8 * pBuffer, GInt32 iBufferLen) throw(std::ios_base::failure, std::logic_error)
{
	return m_in->read(pBuffer, iBufferLen);
}

GInt32 FilterInputStream::read(GInt8 * pBuffer, GInt32 iBufferLen, GInt32 iOffset, GInt32 iLen) throw(std::ios_base::failure, std::logic_error)
{
	return  m_in->read(pBuffer, iBufferLen, iOffset, iBufferLen);
}

void FilterInputStream::reset() throw(std::ios_base::failure)
{
	m_in->reset();
}

GInt64 FilterInputStream::skip(GInt64 lNum) throw(std::ios_base::failure)
{
	return m_in->skip(lNum);	
}


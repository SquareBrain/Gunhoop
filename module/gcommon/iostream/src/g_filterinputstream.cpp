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

using namespace std;
namespace gcom {

FilterInputStream::FilterInputStream(std::shared_ptr<InputStream> in)
	: m_in(in)
{
	if (m_in == NULL)
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

void FilterInputStream::mark(GInt32 readLimit)
{
	m_in->mark(readLimit);
}

bool FilterInputStream::markSupported()
{	
	return m_in->markSupported();
}

GInt32 FilterInputStream::read() throw(std::ios_base::failure, std::logic_error)
{
	return  m_in->read();
}

GInt32 FilterInputStream::read(GInt8* buffer, GInt32 bufferLen) throw(std::ios_base::failure, std::logic_error)
{
	return m_in->read(buffer, bufferLen);
}

GInt32 FilterInputStream::read(GInt8* buffer, GInt32 bufferLen, GInt32 offset, GInt32 len) throw(std::ios_base::failure, std::logic_error)
{
	return  m_in->read(buffer, bufferLen, offset, len);
}

void FilterInputStream::reset() throw(std::ios_base::failure)
{
	m_in->reset();
}

GInt64 FilterInputStream::skip(GInt64 num) throw(std::ios_base::failure)
{
	return m_in->skip(num);	
}
}

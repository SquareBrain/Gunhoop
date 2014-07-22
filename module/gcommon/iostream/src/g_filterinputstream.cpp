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



FilterInputStream::FilterInputStream(InputStream& in)
	: pIn(&in)
{
}

FilterInputStream::~FilterInputStream()
{
}

GInt32 FilterInputStream::available() throw(std::ios_base::failure)
{
	return pIn->available();
}

void FilterInputStream::close() throw(std::ios_base::failure)
{
	pIn->close();
}

void FilterInputStream::mark(GInt32 iReadlimit)
{
	pIn->mark(iReadlimit);
}

bool FilterInputStream::markSupported()
{	
	return pIn->markSupported();
}

GInt32 FilterInputStream::read() throw(std::ios_base::failure, std::logic_error)
{
	return  pIn->read();
}

GInt32 FilterInputStream::read(GInt8 * pBuffer, GInt32 iBufferLen) throw(std::ios_base::failure, std::logic_error)
{
	return pIn->read(pBuffer, iBufferLen);
}

GInt32 FilterInputStream::read(GInt8 * pBuffer, GInt32 iBufferLen, GInt32 iOffset, GInt32 iLen) throw(std::ios_base::failure, std::logic_error)
{
	return  pIn->read(pBuffer, iBufferLen, iOffset, iBufferLen);
}

void FilterInputStream::reset() throw(std::ios_base::failure)
{
	pIn->reset();
}

GInt64 FilterInputStream::skip(GInt64 lNum) throw(std::ios_base::failure)
{
	return pIn->skip(lNum);	
}


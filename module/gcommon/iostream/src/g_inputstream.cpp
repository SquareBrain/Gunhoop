/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_inputstream.cpp
* @version     
* @brief      
* @author   ylh
* @date     2014-07-03
* @note 
*
*  1. 2014-07-03 ylh Created this file
* 
*/

#include <g_inputstream.h>
#include <g_private_define.h>

using namespace std;
using namespace GCommon;



GInt8 InputStream::ms_szSkipBuffer[SKIP_BUFFER_SIZE] ={0};

InputStream::InputStream()
{
}

InputStream::~InputStream()
{
}

GInt32 InputStream::available() throw(std::ios_base::failure)
{
	return 0;
}

void InputStream::close() throw(std::ios_base::failure)
{
}

void InputStream::mark(GInt32 iReadlimit)
{
}

bool InputStream::markSupported()
{
	return false;
}

GInt32 InputStream::read(GInt8 * pBuffer, GInt32 iBufferLen) throw(std::ios_base::failure, std::logic_error)
{
	return this->read(pBuffer, iBufferLen, 0, iBufferLen);
}

GInt32 InputStream::read(GInt8 * pBuffer, GInt32 iBufferLen, GInt32 iOffset, GInt32 iLen) throw(std::ios_base::failure, std::logic_error)
{
	if (!pBuffer)
	{
		throw invalid_argument(EXCEPTION_DESCRIPTION("invalid_argument"));
	}
	else if (iOffset < 0 || iLen < 0 ||iBufferLen < iOffset + iLen)
	{
		throw out_of_range(EXCEPTION_DESCRIPTION("out_of_range"));
	}
	else if (iLen == 0)
	{
		return 0;
	}

	GInt32 iChar = this->read();
	if (iChar == -1)
	{
		return -1;
	}
	pBuffer[iOffset] = static_cast<GInt8>(iChar);

	GInt32 iIndex = 1;
	for (; iIndex < iLen; iIndex ++)
	{
		iChar = read();
		if (iChar == -1)
		{
			break;
		}
		pBuffer[iOffset + iIndex] = static_cast<GInt8>(iChar);
	}

	return iIndex;
}

void InputStream::reset() throw(std::ios_base::failure)
{
}

GInt64 InputStream::skip(GInt64 lNum) throw(std::ios_base::failure)
{
	GInt64 lRemaining = lNum;
	GInt32 iReadLen; 

	while (lRemaining > 0)
	{
		iReadLen = read(ms_szSkipBuffer, SKIP_BUFFER_SIZE, 0,
			static_cast<GInt32>(lRemaining > SKIP_BUFFER_SIZE ? SKIP_BUFFER_SIZE : lRemaining));
		if (iReadLen < 0)
		{
			break;
		}
		lRemaining -= static_cast<GInt64>(iReadLen);
	}

	return lNum - lRemaining;	
}

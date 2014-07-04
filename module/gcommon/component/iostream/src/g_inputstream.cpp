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

using namespace GCommon;



InputStream::InputStream()
{
}

InputStream::~InputStream()
{
}

GInt32 InputStream::available()
{
	return 0;
}

void InputStream::close()
{
}

void InputStream::mark(GInt32 iReadlimit)
{
}

bool InputStream::markSupported()
{
	return false;
}

GInt32 InputStream::read(GInt8 * pBuffer, GInt32 iBufferLen, GInt32 iLen)
{
	return read(pBuffer, iBufferLen, 0, iLen);
}

GInt32 InputStream::read(GInt8 * pBuffer, GInt32 iBufferLen, GInt32 iOffset, GInt32 iLen)
{
	if (!pBuffer)
	{
		return -2;
	}
	else if (iOffset < 0 || iLen < 0 ||iBufferLen < iOffset + iLen)
	{
		return -3;
	}
	else if (iLen == 0)
	{
		return 0;
	}

	GInt32 iChar = read();
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

void InputStream::reset()
{
}

GInt64 InputStream::skip(GInt64 lNum)
{
	GInt64 lRemaining = lNum;
	GInt32 iReadLen; 

	while (lRemaining > 0)
	{
		iReadLen = read(ms_szSkipBuffer, 0, 
			static_cast<GInt32>(lRemaining > SKIP_BUFFER_SIZE ? SKIP_BUFFER_SIZE : lRemaining));
		if (iReadLen < 0)
		{
			break;
		}
		lRemaining -= static_cast<GInt64>(iReadLen);
	}

	return lNum - lRemaining;	
}


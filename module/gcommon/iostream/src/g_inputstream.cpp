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

using namespace std;
using namespace GCommon;

GInt8 InputStream::ms_skipbuffer[SKIP_BUFFER_SIZE] ={0};

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

void InputStream::mark(GInt32 readLimit)
{
}

bool InputStream::markSupported()
{
	return false;
}

GInt32 InputStream::read(GInt8* buffer, GInt32 bufferLen) throw(std::ios_base::failure, std::logic_error)
{
	return this->read(buffer, bufferLen, 0, bufferLen);
}

GInt32 InputStream::read(GInt8* buffer, GInt32 bufferLen, GInt32 offset, GInt32 len) throw(std::ios_base::failure, std::logic_error)
{
	if (!buffer)
	{
		throw invalid_argument(EXCEPTION_DESCRIPTION("invalid_argument"));
	}
	else if (offset < 0 || len < 0 ||bufferLen < offset + len)
	{
		throw out_of_range(EXCEPTION_DESCRIPTION("out_of_range"));
	}
	
	GInt32 i, c;
	for (i = 0; i < len; i ++)
	{
		try
		{
			if ((c = read()) < 0)
				return i == 0 ? -1 : i;
			buffer[offset + i] = c;
		}
		catch (const exception& e)
		{
			if (i == 0)
				throw e;
			return i;
		}
	}

	return i;
}

void InputStream::reset() throw(std::ios_base::failure)
{
}

GInt64 InputStream::skip(GInt64 num) throw(std::ios_base::failure)
{
	GInt64 remaining = num;
	GInt32 readlen; 

	while (remaining > 0)
	{
		readlen = read(ms_skipbuffer, SKIP_BUFFER_SIZE, 0,
			static_cast<GInt32>(remaining > SKIP_BUFFER_SIZE ? SKIP_BUFFER_SIZE : remaining));
		if (readlen < 0)
		{
			break;
		}
		remaining -= static_cast<GInt64>(readlen);
	}

	return num - remaining;	
}

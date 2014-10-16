/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_outputstream.cpp
* @version     
* @brief      
* @author   ylh
* @date     2014-07-31
* @note 
*
*  1. 2014-07-31 ylh Created this file
* 
*/
#include <g_outputstream.h>
	
using namespace std;
namespace gcom {

OutputStream::OutputStream()
{
}

OutputStream::~OutputStream()
{
}

void OutputStream::write(GInt8* buffer, GInt32 bufferLen) throw(std::ios_base::failure, std::logic_error)
{
	return this->write(buffer, bufferLen, 0, bufferLen);
}

void OutputStream::write(GInt8* buffer, GInt32 bufferLen, GInt32 offset, GInt32 len) throw(std::ios_base::failure, std::logic_error)
{
	if (!buffer)
	{
		throw invalid_argument(EXCEPTION_DESCRIPTION("invalid_argument"));
	}
	else if (offset < 0 || len < 0 ||bufferLen < offset + len)
	{
		throw out_of_range(EXCEPTION_DESCRIPTION("out_of_range"));
	}
	
	for (GInt32 i = 0; i < len; i ++)
	{
		write(buffer[offset + i]);
	}
}

void OutputStream::flush() throw(std::ios_base::failure)
{
}

void OutputStream::close() throw(std::ios_base::failure)
{
}
}

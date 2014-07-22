/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_datainputstream.cpp
* @version     
* @brief      
* @author   ylh
* @date     2014-07-08
* @note 
*
*  1. 2014-07-08 ylh Created this file
* 
*/

#include <g_datainputstream.h>

using namespace std;
using namespace GCommon;

DataInputStream::DataInputStream(std::shared_ptr<InputStream> in)
	: FilterInputStream(in)
{
}

DataInputStream::~DataInputStream()
{
}

bool DataInputStream::readBool() throw(std::ios_base::failure, GCommon::EOFException)
{
	register GInt32 c = read();
	if (c < 0)
	{
		throw EOFException();
	}
	return (c != 0);
}

GInt8 DataInputStream::readChar() throw(std::ios_base::failure, GCommon::EOFException)
{
	register GInt32 c = read();
	if (c < 0)
	{
		throw EOFException();
	}
	return static_cast<GInt8>(c);
}

GInt16 DataInputStream::readShort() throw(std::ios_base::failure, GCommon::EOFException)
{
	register GInt8 buffer[2] = {0};
	readFully(buffer, 2);
	return static_cast<GInt16>(((buffer[0] & 0xff) << 8) | (buffer[1] & 0xff));
}

GInt32 DataInputStream::readInt() throw(std::ios_base::failure, GCommon::EOFException)
{
	register GInt8 buffer[4] = {0};
	readFully(buffer, 4);
	return static_cast<GInt32>(((buffer[0] & 0xff) << 24) | ((buffer[1] & 0xff) << 16) |
            ((buffer[2] & 0xff) << 8) | (buffer[3] & 0xff));
}

GInt64 DataInputStream::readLong() throw(std::ios_base::failure, GCommon::EOFException)
{
	register GInt8 buffer[8] = {0};
	readFully(buffer, 8);
	GInt64 i1 = static_cast<GInt64>(((buffer[0] & 0xff) << 24) | ((buffer[1] & 0xff) << 16) |
		((buffer[2] & 0xff) << 8) | (buffer[3] & 0xff));
	GInt64 i2 = static_cast<GInt64>(((buffer[4] & 0xff) << 24) | ((buffer[5] & 0xff) << 16) |
		((buffer[6] & 0xff) << 8) | (buffer[7] & 0xff));
	return static_cast<GInt64>(((i1 & 0xffffffffL) << 32) |(i2 & 0xffffffffL));
}

GUint8 DataInputStream::readUnsignedChar() throw(std::ios_base::failure, GCommon::EOFException)
{	
	register GInt32 c = read();
	if (c < 0)
	{
		throw EOFException();
	}
	return static_cast<GUint8>(c);
}

GUint16 DataInputStream::readUnsignedShort() throw(std::ios_base::failure, GCommon::EOFException)
{
	register GInt8 buffer[2] = {0};
	readFully(buffer, 2);
	return static_cast<GUint16>(((buffer[0] & 0xff) << 8) | (buffer[1] & 0xff));
}

GUint32 DataInputStream::readUnsignedInt() throw(std::ios_base::failure, GCommon::EOFException)
{
	register GInt8 buffer[4] = {0};
	readFully(buffer, 4);
	return static_cast<GUint32>(((buffer[0] & 0xff) << 24) | ((buffer[1] & 0xff) << 16) |
			((buffer[2] & 0xff) << 8) | (buffer[3] & 0xff));
}

GUint64 DataInputStream::readUnsignedLong() throw(std::ios_base::failure, GCommon::EOFException)
{
	register GInt8 buffer[8] = {0};
	readFully(buffer, 8);
	GUint64 i1 = static_cast<GUint64>(((buffer[0] & 0xff) << 24) | ((buffer[1] & 0xff) << 16) |
		((buffer[2] & 0xff) << 8) | (buffer[3] & 0xff));
	GUint64 i2 = static_cast<GUint64>(((buffer[4] & 0xff) << 24) | ((buffer[5] & 0xff) << 16) |
		((buffer[6] & 0xff) << 8) | (buffer[7] & 0xff));
	return static_cast<GUint64>(((i1 & 0xffffffffUL) << 32) |(i2 & 0xffffffffUL));
}

std::string DataInputStream::readLine() throw(std::ios_base::failure, GCommon::EOFException)
{
	return string("");
}

void DataInputStream::readFully(GInt8* buffer, GInt32 bufferLen) throw(std::ios_base::failure, std::logic_error, GCommon::EOFException)
{
	readFully(buffer, bufferLen, 0, bufferLen);
}

void DataInputStream::readFully(GInt8* buffer, GInt32 bufferLen, GInt32 offset, GInt32 len) throw(std::ios_base::failure, std::logic_error, GCommon::EOFException)
{
	if (!buffer)
	{
		throw invalid_argument(EXCEPTION_DESCRIPTION("invalid_argument"));
	}
	
	if (bufferLen < 0 ||
		offset < 0 ||
		len < 0)
	{
		throw out_of_range(EXCEPTION_DESCRIPTION("out_of_range"));
	}

	while (len > 0)
	{
		GInt32 num = read(buffer, bufferLen, offset, len);
		if (num < 0)
		{
			throw EOFException();
		}
		len -= num;
		offset += num;
	}
}


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
#include <g_private_define.h>

using namespace std;
using namespace GCommon;

DataInputStream::DataInputStream(std::shared_ptr<InputStream> in)
	: FilterInputStream(in)
{
}

DataInputStream::~DataInputStream()
{
}

int DataInputStream::readToBuff(int count)
{
	if (count > MAX_BUFFER_SIZE)
	{
		return -2;
	}

	int offset = 0;
	while(offset < count)
	{
		int bytesRead = read(m_buffer, MAX_BUFFER_SIZE, offset, count - offset);
		if(bytesRead == -1) 
			return bytesRead;
		offset += bytesRead;
	}
	return offset;
}

bool DataInputStream::readBool() throw(std::ios_base::failure)
{
	GInt32 iTmp = read();
	return (iTmp != 0);
}

GInt8 DataInputStream::readChar() throw(std::ios_base::failure)
{
	return read();
}

GInt16 DataInputStream::readShort() throw(std::ios_base::failure)
{
	if (readToBuff(2) < 0)
	{
		return -1;
	}

	return static_cast<GInt16>(((m_buffer[0] & 0xff) << 8) | (m_buffer[1] & 0xff));
}

GInt32 DataInputStream::readInt() throw(std::ios_base::failure)
{
	if (readToBuff(4) < 0)
	{
		return -1;
	}

	return static_cast<GInt32>(((m_buffer[0] & 0xff) << 24) | ((m_buffer[1] & 0xff) << 16) |
            ((m_buffer[2] & 0xff) << 8) | (m_buffer[3] & 0xff));
}

GInt64 DataInputStream::readLong() throw(std::ios_base::failure)
{
	if (readToBuff(8) < 0)
	{
		return -1;
	}

	GInt64 i1 = static_cast<GInt64>(((m_buffer[0] & 0xff) << 24) | ((m_buffer[1] & 0xff) << 16) |
		((m_buffer[2] & 0xff) << 8) | (m_buffer[3] & 0xff));

	GInt64 i2 = static_cast<GInt64>(((m_buffer[4] & 0xff) << 24) | ((m_buffer[5] & 0xff) << 16) |
		((m_buffer[6] & 0xff) << 8) | (m_buffer[7] & 0xff));

	return static_cast<GInt64>(((i1 & 0xffffffffL) << 32) |(i2 & 0xffffffffL));
}

GUint8 DataInputStream::readUnsignedChar() throw(std::ios_base::failure)
{
	return static_cast<GUint8>(read());
}

GUint16 DataInputStream::readUnsignedShort() throw(std::ios_base::failure)
{
	if (readToBuff(2) < 0)
	{
		return -1;
	}

	return static_cast<GUint16>(((m_buffer[0] & 0xff) << 8) | (m_buffer[1] & 0xff));
}

GUint32 DataInputStream::readUnsignedInt() throw(std::ios_base::failure)
{
	if (readToBuff(4) < 0)
	{
		return -1;
	}

	return static_cast<GUint32>(((m_buffer[0] & 0xff) << 24) | ((m_buffer[1] & 0xff) << 16) |
			((m_buffer[2] & 0xff) << 8) | (m_buffer[3] & 0xff));
}

GUint64 DataInputStream::readUnsignedLong() throw(std::ios_base::failure)
{
	if (readToBuff(8) < 0)
	{
		return -1;
	}
	
	GUint64 i1 = static_cast<GUint64>(((m_buffer[0] & 0xff) << 24) | ((m_buffer[1] & 0xff) << 16) |
		((m_buffer[2] & 0xff) << 8) | (m_buffer[3] & 0xff));

	GUint64 i2 = static_cast<GUint64>(((m_buffer[4] & 0xff) << 24) | ((m_buffer[5] & 0xff) << 16) |
		((m_buffer[6] & 0xff) << 8) | (m_buffer[7] & 0xff));

	return static_cast<GUint64>(((i1 & 0xffffffffUL) << 32) |(i2 & 0xffffffffUL));
}

std::string DataInputStream::readLine() throw(std::ios_base::failure)
{
	
}

void DataInputStream::readFully(GInt8* pBuffer, GInt32 iBufferLen) throw(std::ios_base::failure)
{
	readFully(pBuffer, iBufferLen, 0, iBufferLen);
}

void DataInputStream::readFully(GInt8* pBuffer, GInt32 iBufferLen, GInt32 iOff, GInt32 iLen) throw(std::ios_base::failure)
{
	if (!pBuffer ||
		iBufferLen < 0 ||
		iOff < 0 ||
		iLen < 0)
	{
		return;
	}

	while (iLen > 0)
	{
		GInt32 iNumRead = read(pBuffer, iBufferLen, iOff, iLen);
		if (iNumRead < 0)
		{
			return;
		}
		iLen -= iNumRead;
		iOff += iNumRead;
	}
}

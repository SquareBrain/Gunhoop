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

using namespace GCommon;



DataInputStream::DataInputStream(InputStream& in)
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

bool DataInputStream::readBool()
{
	GInt32 iTmp = read();
	return (iTmp != 0);
}

GInt8 DataInputStream::readChar()
{
	return read();
}

GInt16 DataInputStream::readShort()
{
	if (readToBuff(2) < 0)
	{
		return -1;
	}

	return static_cast<GInt16>(((m_buffer[0] & 0xff) << 8) | (m_buffer[1] & 0xff));
}

GInt32 DataInputStream::readInt()
{
	if (readToBuff(4) < 0)
	{
		return -1;
	}

	return static_cast<GInt32>(((m_buffer[0] & 0xff) << 24) | ((m_buffer[1] & 0xff) << 16) |
            ((m_buffer[2] & 0xff) << 8) | (m_buffer[3] & 0xff));
}

GInt64 DataInputStream::readLong()
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

GUint8 DataInputStream::readUnsignedChar()
{
	return static_cast<GUint8>(read());
}

GUint16 DataInputStream::readUnsignedShort()
{
	if (readToBuff(2) < 0)
	{
		return -1;
	}

	return static_cast<GUint16>(((m_buffer[0] & 0xff) << 8) | (m_buffer[1] & 0xff));
}

GUint32 DataInputStream::readUnsignedInt()
{
	if (readToBuff(4) < 0)
	{
		return -1;
	}

	return static_cast<GUint32>(((m_buffer[0] & 0xff) << 24) | ((m_buffer[1] & 0xff) << 16) |
			((m_buffer[2] & 0xff) << 8) | (m_buffer[3] & 0xff));
}

GUint64 DataInputStream::readUnsignedLong()
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

void DataInputStream::readFully(GInt8* pBuffer, GInt32 iBufferLen)
{
	readFully(pBuffer, iBufferLen, 0, iBufferLen);
}

void DataInputStream::readFully(GInt8* pBuffer, GInt32 iBufferLen, GInt32 iOff, GInt32 iLen)
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

GInt32 DataInputStream::skipBytes(GInt32 iNum)
{
	return 0;
}


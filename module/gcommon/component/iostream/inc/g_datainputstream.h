/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_datainputstream.h
* @version     
* @brief      
* @author   ylh
* @date     2014-07-08
* @note 
*
*  1. 2014-07-08 ylh Created this file
* 
*/

#pragma once

#include <g_common.h>
#include <g_filterinputstream.h>
#include <g_datainput.h>



G_NS_GCOMMON_BEG

class DataInputStream : public FilterInputStream, public DataInput
{
public:
	DataInputStream(InputStream& in); 
	virtual ~DataInputStream();

	virtual bool readBool();
	virtual GInt8 readChar();
	virtual GInt16 readShort();
	virtual GInt32 readInt();
	virtual GInt64 readLong();
	virtual GUint8 readUnsignedChar();
	virtual GUint16 readUnsignedShort();
	virtual GUint32 readUnsignedInt();
	virtual GUint64 readUnsignedLong();

	virtual void readFully(GInt8* pBuffer, GInt32 iBufferLen);
	virtual void readFully(GInt8* pBuffer, GInt32 iBufferLen, GInt32 iOff, GInt32 iLen);

	virtual GInt32 skipBytes(GInt32 iNum);

private:
	GInt32 readToBuff(int count);

private:
	enum
	{
		MAX_BUFFER_SIZE = 8
	};
	GInt8 m_buffer[MAX_BUFFER_SIZE];
};

G_NS_END



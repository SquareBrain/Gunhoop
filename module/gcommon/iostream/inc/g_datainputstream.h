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

#include <g_system.h>
#include <g_namespace.h>
#include <g_filterinputstream.h>
#include <g_datainput.h>

G_NS_GCOMMON_BEG

class DataInputStream : public FilterInputStream, public DataInput
{
public:
	DataInputStream(std::shared_ptr<InputStream> in); 
	virtual ~DataInputStream();

	virtual bool readBool() throw(std::ios_base::failure);
	virtual GInt8 readChar() throw(std::ios_base::failure);
	virtual GInt16 readShort() throw(std::ios_base::failure);
	virtual GInt32 readInt() throw(std::ios_base::failure);
	virtual GInt64 readLong() throw(std::ios_base::failure);
	virtual GUint8 readUnsignedChar() throw(std::ios_base::failure);
	virtual GUint16 readUnsignedShort() throw(std::ios_base::failure);
	virtual GUint32 readUnsignedInt() throw(std::ios_base::failure);
	virtual GUint64 readUnsignedLong() throw(std::ios_base::failure);
	virtual std::string readLine() throw(std::ios_base::failure);

	virtual void readFully(GInt8* pBuffer, GInt32 iBufferLen) throw(std::ios_base::failure, std::logic_error);
	virtual void readFully(GInt8* pBuffer, GInt32 iBufferLen, GInt32 iOff, GInt32 iLen) throw(std::ios_base::failure, std::logic_error);

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


/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_datainput.h
* @version     
* @brief      
* @author   ylh
* @date     2014-07-04
* @note 
*
*  1. 2014-07-04 ylh Created this file
* 
*/

#pragma once

#include <g_type.h>



G_NS_GCOMMON_BEG

class DataInput
{
public:
	DataInput() {}
	virtual ~DataInput() {}

	virtual bool readBool() = 0;
	virtual GInt8 readChar() = 0;
	virtual GInt16 readShort() = 0;
	virtual GInt32 readInt() = 0;
	virtual GInt64 readLong() = 0;
	virtual GUint8 readUnsignedChar() = 0;
	virtual GUint16 readUnsignedShort() = 0;
	virtual GUint32 readUnsignedInt() = 0;
	virtual GUint64 readUnsignedLong() = 0;

	virtual void readFully(GInt8* pBuffer, GInt32 iBufferLen) = 0;
	virtual void readFully(GInt8* pBuffer, GInt32 iBufferLen, GInt32 iOff, GInt32 iLen) = 0;

	virtual GInt32 skipBytes(GInt32 iNum) = 0;
};

G_NS_END



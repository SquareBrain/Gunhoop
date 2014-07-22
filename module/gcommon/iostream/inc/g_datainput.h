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

#include <exception>
#include <stdexcept>
#include <iostream>
#include <g_system.h>
#include <g_namespace.h>

G_NS_GCOMMON_BEG

class DataInput
{
public:
	DataInput() {}
	virtual ~DataInput() {}

	virtual bool readBool() throw(std::ios_base::failure) = 0;
	virtual GInt8 readChar() throw(std::ios_base::failure) = 0;
	virtual GInt16 readShort() throw(std::ios_base::failure) = 0;
	virtual GInt32 readInt() throw(std::ios_base::failure) = 0;
	virtual GInt64 readLong() throw(std::ios_base::failure) = 0;
	virtual GUint8 readUnsignedChar() throw(std::ios_base::failure) = 0;
	virtual GUint16 readUnsignedShort() throw(std::ios_base::failure) = 0;
	virtual GUint32 readUnsignedInt() throw(std::ios_base::failure) = 0;
	virtual GUint64 readUnsignedLong() throw(std::ios_base::failure) = 0;
	
	virtual std::String readLine() throw(std::ios_base::failure) = 0;

	virtual void readFully(GInt8* pBuffer, GInt32 iBufferLen) throw(std::ios_base::failure) = 0;
	virtual void readFully(GInt8* pBuffer, GInt32 iBufferLen, GInt32 iOff, GInt32 iLen) throw(std::ios_base::failure) = 0;

	virtual GInt32 skipBytes(GInt32 iNum) = 0;
};

G_NS_END



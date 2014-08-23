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
#include <g_io_exception.h>

namespace gcom {

class DataInput
{
public:
	DataInput() {}
	virtual ~DataInput() {}

	virtual bool readBool() throw(std::ios_base::failure, GCommon::EOFException) = 0;
	virtual GInt8 readChar() throw(std::ios_base::failure, GCommon::EOFException) = 0;
	virtual GInt16 readShort() throw(std::ios_base::failure, GCommon::EOFException) = 0;
	virtual GInt32 readInt() throw(std::ios_base::failure, GCommon::EOFException) = 0;
	virtual GInt64 readLong() throw(std::ios_base::failure, GCommon::EOFException) = 0;
	virtual GUint8 readUnsignedChar() throw(std::ios_base::failure, GCommon::EOFException) = 0;
	virtual GUint16 readUnsignedShort() throw(std::ios_base::failure, GCommon::EOFException) = 0;
	virtual GUint32 readUnsignedInt() throw(std::ios_base::failure, GCommon::EOFException) = 0;
	virtual GUint64 readUnsignedLong() throw(std::ios_base::failure, GCommon::EOFException) = 0;
	virtual std::string readLine() throw(std::ios_base::failure, GCommon::EOFException) = 0;

	virtual void readFully(GInt8* buffer, GInt32 bufferLen) throw(std::ios_base::failure, std::logic_error, GCommon::EOFException) = 0;
	virtual void readFully(GInt8* buffer, GInt32 bufferLen, GInt32 offset, GInt32 len) throw(std::ios_base::failure, std::logic_error, GCommon::EOFException) = 0;
};
}

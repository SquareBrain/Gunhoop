/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_outputstream.h
* @version     
* @brief      
* @author   ylh
* @date     2014-07-29
* @note 
*
*  1. 2014-07-29 ylh Created this file
* 
*/

#pragma once

#include <exception>
#include <stdexcept>
#include <iostream>
#include <g_system.h>
#include <g_namespace.h>
#include <g_io_exception.h>

G_NS_GCOMMON_BEG

class OutputStream
{
public:
	OutputStream();
	virtual ~OutputStream();

	/**
        * @brief This method writes a single byte to the output stream.  The byte written
     	  * is the low eight bits of the int passed and a argument.
     	  * Subclasses must provide an implementation of this abstract method.
     	  * @param [in] c The byte to be written to the output stream, passed as
     	  *          the low eight bits of an int
     	  * @exception IOException If an error occurs
     	  */
	void write(GInt32 c) throw(std::ios_base::failure, std::logic_error) = 0;

	void write(GInt8* buffer, GInt32 bufferLen) throw(std::ios_base::failure, std::logic_error);

	void write(GInt8* buffer, GInt32 bufferLen, GInt32 offset, GInt32 len) throw(std::ios_base::failure, std::logic_error);

	void flush() throw(std::ios_base::failure);

	void close() throw(std::ios_base::failure);
};

G_NS_END



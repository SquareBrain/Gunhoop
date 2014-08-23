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

namespace gcom {

class OutputStream
{
public:
	OutputStream();
	virtual ~OutputStream();

	/**
        * @brief This method writes a single byte to the output stream. The byte written
     	  * is the low eight bits of the int passed and a argument.
     	  * Subclasses must provide an implementation of this abstract method.
     	  * @param [in] c The byte to be written to the output stream, passed as
     	  *          the low eight bits of an int
     	  * @exception IOException If an error occurs
     	  */
	virtual void write(GInt32 c) throw(std::ios_base::failure, std::logic_error) = 0;

	/**
        * @brief This method all the writes bytes from the passed array to the
        * output stream. This method is equivalent to write(b, buf.length, 0, buf.length)
        * which is exactly how it is implemented in this class.
     	  * @param [in] buffer the buffer into which the data is read.
        * @param [in] bufferLen the length of the buffer.
     	  * @exception IOException If an error occurs
     	  */
	virtual void write(GInt8* buffer, GInt32 bufferLen) throw(std::ios_base::failure, std::logic_error);

	/**
        * @brief This method writes len bytes from the specified buffer starting at 
        * index offset into the buffer.
        * This method in this class calls the single byte write() method in a loop 
        * until all bytes have been written. Subclasses should override this method
        * if possible in order to provide a more efficent implementation.
     	  * @param [in] buffer the buffer into which the data is read.
        * @param [in] bufferLen the length of the buffer.
        * @param [in] offset the start offset in array b at which the data is written.
        * @param [in] len the maximum number of bytes to read.
     	  * @exception IOException If an error occurs
     	  */
	virtual void write(GInt8* buffer, GInt32 bufferLen, GInt32 offset, GInt32 len) throw(std::ios_base::failure, std::logic_error);

	/**
        * @brief This method forces any data that may have been buffered to be written
        * to the underlying output device.  Please note that the host environment
        * might perform its own buffering unbeknowst to Java.  In that case, a
        * write made (for example, to a disk drive) might be cached in OS
        * buffers instead of actually being written to disk.
        * This method in this class does nothing.
     	  * @exception IOException If an error occurs
     	  */
	virtual void flush() throw(std::ios_base::failure);

	/**
        * @brief This method closes the stream.  Any internal or native resources
        * associated with this stream are freed.  Any subsequent attempt to
        * access the stream might throw an exception.
        * This method in this class does nothing.
     	  * @exception IOException If an error occurs
     	  */
	virtual void close() throw(std::ios_base::failure);
};
}

/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_fileinputstream.h
* @version     
* @brief      
* @author   ylh
* @date     2014-07-10
* @note 
*
*  1. 2014-07-10 ylh Created this file
* 
*/

#pragma once

#include <string>
#include <memory>

#include <g_system.h>
#include <g_namespace.h>
#include <g_inputstream.h>

namespace gcom {

class FileInputStream : public InputStream
{
public:
	FileInputStream(std::shared_ptr<GFile> file);
	FileInputStream(const std::string& filePath);
	virtual ~FileInputStream(); 

	/**
	  * @brief Returns the number of bytes that can be read (or skipped over) from this input stream without blocking by the next caller of a method for this input stream. The next caller might be the same thread or another thread.
	  * The available method for class InputStream always returns 0.
	  * This method should be overridden by subclasses.
	  * @return the number of bytes that can be read from this input stream without blocking.
        */
	virtual GInt32 available() throw(std::ios_base::failure);

	/**
        * @brief Closes this input stream and releases any system resources associated with the stream.
        * The close method of InputStream does nothing.
        */
	virtual void close() throw(std::ios_base::failure);

	/**
        * @brief Reads the next byte of data from the input stream. The value byte is returned as an int in the range 0 to 255. If no byte is available because the end of the stream has been reached, the value -1 is returned. This method blocks until input data is available, the end of the stream is detected, or an exception is thrown.
        * A subclass must provide an implementation of this method.
        * @return the next byte of data, or -1 if the end of the stream is reached.
        */
	virtual GInt32 read() throw(std::ios_base::failure, std::logic_error);

	/**
        * @brief The read(pBuffer, iLen) method for class InputStream has the same effect as:
        * read(pBuffer, 0, iLen) 
        * @param [out] pBuffer the buffer into which the data is read.
        * @param [in] iBufferLen the length of the buffer.
        * @return the total number of bytes read into the buffer, or -1 is there is no more data because the end of the stream has been reached.
        * @note 
        */
	virtual GInt32 read(GInt8* buffer, GInt32 bufferLen) throw(std::ios_base::failure, std::logic_error);

	/**
        * @brief Returns the number of bytes that can be read (or skipped over) from this input stream without blocking by the next caller of a method for this input stream. The next caller might be the same thread or another thread.
	  * The available method for class InputStream always returns 0.
	  * This method should be overridden by subclasses.
        * @param [out] pBuffer the buffer into which the data is read.
        * @param [in] iBufferLen the length of the buffer.
        * @param [in] iOffset the start offset in array b at which the data is written.
        * @param [in] iLen the maximum number of bytes to read.
        * @return the total number of bytes read into the buffer, or -1 if there is no more data because the end of the stream has been reached.
        */
	virtual GInt32 read(GInt8* buffer, GInt32 bufferLen, GInt32 offset, GInt32 len) throw(std::ios_base::failure, std::logic_error);

	/**
        * @brief Skips over and discards n bytes of data from this input stream. The skip method may, for a variety of reasons, end up skipping over some smaller number of bytes, possibly 0. This may result from any of a number of conditions; reaching end of file before n bytes have been skipped is only one possibility. The actual number of bytes skipped is returned. If n is negative, no bytes are skipped.
        * The skip method of InputStream creates a byte array and then repeatedly reads into it until n bytes have been read or the end of the stream has been reached. 
        * Subclasses are encouraged to provide a more efficient implementation of this method.
        * @param [in] iNum the number of bytes to be skipped.
        * @return the actual number of bytes skipped.
        */
	virtual GInt64 skip(GInt64 num) throw(std::ios_base::failure);

	std::shared_ptr<GFile> GetFile();

private:
	std::shared_ptr<GFile> m_file;
	GMutex m_mtx;
};
}

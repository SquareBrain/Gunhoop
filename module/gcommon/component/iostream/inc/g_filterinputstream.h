/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_filterinputstream.h
* @version     
* @brief      
* @author   ylh
* @date     2014-07-03
* @note 
*
*  1. 2014-07-03 ylh Created this file
* 
*/

#pragma once

#include <g_type.h>
#include <g_inputstream.h>

G_NS_GCOMMON_BEG

class FilterInputStream : public InputStream
{
public:
	FilterInputStream();
	virtual ~FilterInputStream();

	/**
        * @brief Returns the number of bytes that can be read (or skipped over) from this input stream without blocking by the next caller of a method for this input stream. The next caller might be the same thread or another thread.
	  * The available method for class InputStream always returns 0.
	  * This method should be overridden by subclasses.
        * @return the number of bytes that can be read from this input stream without blocking.
        */
	virtual GInt32 available();

	/**
        * @brief Closes this input stream and releases any system resources associated with the stream.
        * The close method of InputStream does nothing.
        */
	virtual void close();

	/**
        * @brief Marks the current position in this input stream. A subsequent call to the reset method repositions this stream at the last marked position so that subsequent reads re-read the same bytes.
        * The readlimit arguments tells this input stream to allow that many bytes to be read before the mark position gets invalidated.
        * The general contract of mark is that, if the method markSupported returns true, the stream somehow remembers all the bytes read after the call to mark and stands ready to supply those same bytes again if and whenever the method reset is called. However, the stream is not required to remember any data at all if more than readlimit bytes are read from the stream before reset is called.
        * The mark method of InputStream does nothing.
        * @param [in] iReadlimit the maximum limit of bytes that can be read before the mark position becomes invalid.
        */
	virtual void mark(GInt32 iReadlimit);

	/**
        * @brief Tests if this input stream supports the mark and reset methods. Whether or not mark and reset are supported is an invariant property of a particular input stream instance. The markSupported method of InputStream returns false.
        * @return true if this stream instance supports the mark and reset methods; false otherwise.
        */
	virtual bool markSupported();

	/**
        * @brief Reads the next byte of data from the input stream. The value byte is returned as an int in the range 0 to 255. If no byte is available because the end of the stream has been reached, the value -1 is returned. This method blocks until input data is available, the end of the stream is detected, or an exception is thrown.
        * A subclass must provide an implementation of this method.
        * @return the next byte of data, or -1 if the end of the stream is reached.
        */
	virtual GInt32 read();

	/**
        * @brief The read(pBuffer, iLen) method for class InputStream has the same effect as:
        * read(pBuffer, 0, iLen) 
        * @param [out] pBuffer the buffer into which the data is read.
        * @param [in] iLen the length of the buffer
        * @return the total number of bytes read into the buffer, or -1 is there is no more data because the end of the stream has been reached.
        * @note 
        */
	virtual GInt32 read(GInt8* pBuffer, GInt32 iLen);

	/**
        * @brief Returns the number of bytes that can be read (or skipped over) from this input stream without blocking by the next caller of a method for this input stream. The next caller might be the same thread or another thread.
	  * The available method for class InputStream always returns 0.
	  * This method should be overridden by subclasses.
        * @param [out] pBuffer the buffer into which the data is read.
        * @param [in] iOffset the start offset in array b at which the data is written.
        * @param [in] iLen the length of the buffer
        * @return the total number of bytes read into the buffer, or -1 if there is no more data because the end of the stream has been reached.
        */
	virtual GInt32 read(GInt8* pBuffer, GInt32 iOffset, GInt32 iLen);

	/**
        * @brief Repositions this stream to the position at the time the mark method was last called on this input stream.
  	  * The method reset for class InputStream does nothing
        */
	virtual void reset();

	/**
        * @brief Skips over and discards n bytes of data from this input stream. The skip method may, for a variety of reasons, end up skipping over some smaller number of bytes, possibly 0. This may result from any of a number of conditions; reaching end of file before n bytes have been skipped is only one possibility. The actual number of bytes skipped is returned. If n is negative, no bytes are skipped.
        * The skip method of InputStream creates a byte array and then repeatedly reads into it until n bytes have been read or the end of the stream has been reached. Subclasses are encouraged to provide a more efficient implementation of this method.
        * @param [in] iNum the number of bytes to be skipped.
        * @return the actual number of bytes skipped.
        */
	virtual GInt32 skip(GInt32 iNum);
};

G_NS_END


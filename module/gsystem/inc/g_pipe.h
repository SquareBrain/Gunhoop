/*************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_pipe.h
* @version     
* @brief      
* @author	duye
* @date		2013-11-15
* @note 
*
*  2. 2014-06-20 duye move to gohoop project
*  1. 2013-11-15 duye Created this file
* 
*/

#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <g_type.h>

namespace gsys {
	
/** 
 * @brief pipe base class, be inherited by WriteGPipe and ReadGPipe class
 */
class Pipe
{
public:
	Pipe() {}
	virtual ~Pipe() {}

	/**
	 * @brief Open the pipe
	 * @param [in] pipeName : the GPipe name 
	 * @return true/false
	 * @note 
	 */			
	virtual bool open(const GInt8* pipeName) = 0;

protected:
	/**
	 * @brief Open the pipe
	 * @param [in] pipeName : the GPipe name
	 * @param [in] mode : open mode
	 * @return true/false
	 * @note 
	 */		
	bool orgOpen(const GInt8* pipeName, const GInt32 mode);
    
protected:
	/** 
	 * @brief pipe descriptor
	 */	
	GInt32	m_pipefd;
};

/** 
 * @brief Be used to write GPipe
 */
class GWritePipe : public Pipe
{
public:
	GWritePipe() {}
	virtual ~GWritePipe() {}

	/**
	 * @brief Open the pipe
	 * @param [in] GPipeName : the GPipe name
	 * @return true/false
	 * @note 
	 */			
	virtual bool open(const GInt8* pipeName);	

	/**
	 * @brief Write data to pipe
	 * @param [in] data : write data
	 * @param [in] length : data length
	 * @return size/-1
	 * @note 
	 */		
	GInt64 write(const GInt8* data, const GUint64 length);
	
private:
	/**
	 * @brief prevent copying
	 * @note
	 */	
	GWritePipe(const GWritePipe&);

	/**
	 * prevent copying
	 * @note
	 */		
	void operator=(const GWritePipe&);
};

/** 
 * @brief be used to read GPipe
 */
class GReadPipe : public Pipe
{
public:
	GReadPipe() {}
	virtual ~GReadPipe() {}

	/**
	 * @brief open pipe
	 * @param [in] pipeName : GPipe name
	 * @return true/false
	 * @note 
	 */    
	virtual bool open(const GInt8* pipeName);

	/**
	 * @brief read data from pipe
	 * @param [out] buffer : output buffer
	 * @param [in] size : output buffer size
	 * @return size/-1
	 * @note 
	 */  	
	GInt64 read(GInt8* buffer, const GUint64 size);  

private:
	/**
	 * @brief prevent copying
	 * @note
	 */	
	GReadPipe(const GReadPipe&);

	/**
	* @brief prevent copying
	* @note
	*/		
	void operator=(const GReadPipe&);	
};
}

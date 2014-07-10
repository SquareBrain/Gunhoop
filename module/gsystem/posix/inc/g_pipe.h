/*************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_pipe.h
* @version     
* @brief      
* @author   duye
* @date     2013-11-15
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

G_NS_GCOMMON_BEG 

/** 
 * Pipe base class, be inherited by WritePipe and ReadPipe class
 */
class Pipe
{
public:
	Pipe() {}
	virtual ~Pipe() {}
	
    /**
     * Open the pipe
	 * @param [in] pipeName : the pipe name 
	 * @return true/false
     * @note 
     */			
	virtual bool openPipe(const GInt8* pipeName) = 0;

protected:
    /**
     * Open the pipe
	 * @param [in] pipeName : the pipe name
	 * @param [in] mode : open mode
	 * @return true/false
     * @note 
     */		
    bool orgOpen(const GInt8* pipeName, const GInt32 mode);
    
protected:
    /** 
     * pipe descriptor
     */	
	GInt32		m_pipefd;
};

/** 
 * Be used to write pipe
 */
class WritePipe : public Pipe
{
public:
	WritePipe() {}
	virtual ~WritePipe() {}
	
    /**
     * Open the pipe
	 * @param [in] pipeName : the pipe name
	 * @return true/false
     * @note 
     */			
	virtual bool openPipe(const GInt8* pipeName);	
	
    /**
     * Write data to pipe
	 * @param [in] data : write data
	 * @param [in] length : data length
	 * @return size/-1
     * @note 
     */		
	GInt64 writeData(const GInt8* data, const GUint64 length);
	
private:
    /**
     * prevent copying
     * @note
     */	
	WritePipe(const WritePipe&);

    /**
     * prevent copying
     * @note
     */		
	void operator=(const WritePipe&);
};

/** 
 * be used to read pipe
 */
class ReadPipe : public Pipe
{
public:
    ReadPipe() {}
    virtual ~ReadPipe() {}

    /**
     * open pipe
     * @param [in] pipeName : pipe name
	 * @return true/false
     * @note 
     */    
    virtual bool openPipe(const GInt8* pipeName);

    /**
     * read data from pipe
	 * @param [out] buffer : output buffer
	 * @param [in] size : output buffer size
	 * @return size/-1
     * @note 
     */  	
	GInt64 readData(GInt8* buffer, const GUint64 size);    
};

G_NS_END 

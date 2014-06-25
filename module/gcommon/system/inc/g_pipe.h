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
	virtual bool open(const GInt8* pipeName) = 0;

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
	virtual bool open(const GInt8* pipeName);	
	
    /**
     * Write data to pipe
	 * @param [in] data 
	 * @param [in] dataLen 
	 * @return size/-1
     * @note 
     */		
	GInt32 write(const GInt8* data, const GUint32 dataLen);
	
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
    virtual bool open(const GInt8* pipeName);

    /**
     * read data from pipe
	 * @param [out] buffer : template buffer
	 * @param [in] bufferSize : template buffer size
	 * @return size/-1
     * @note 
     */  	
	GInt32 read(GInt8* buffer, const GUint32 bufferSize);    
};

G_NS_GCOMMON_END 

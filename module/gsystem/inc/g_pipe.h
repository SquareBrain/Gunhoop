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

/** 
 * GPipe base class, be inherited by WriteGPipe and ReadGPipe class
 */
class GPipe
{
public:
	GPipe() {}
	virtual ~GPipe() {}
	
    /**
     * Open the pipe
	 * @param [in] pipeName : the GPipe name 
	 * @return true/false
     * @note 
     */			
	virtual bool openGPipe(const GInt8* pipeName) = 0;

protected:
    /**
     * Open the pipe
	 * @param [in] pipeName : the GPipe name
	 * @param [in] mode : open mode
	 * @return true/false
     * @note 
     */		
    bool orgOpen(const GInt8* pipeName, const GInt32 mode);
    
protected:
    /** 
     * GPipe descriptor
     */	
	GInt32		m_pipefd;
};

/** 
 * Be used to write GPipe
 */
class GWritePipe : public GPipe
{
public:
	GWritePipe() {}
	virtual ~GWritePipe() {}
	
    /**
     * Open the pipe
	 * @param [in] GPipeName : the GPipe name
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
	GWritePipe(const GWritePipe&);

    /**
     * prevent copying
     * @note
     */		
	void operator=(const GWritePipe&);
};

/** 
 * be used to read GPipe
 */
class GReadPipe : public GPipe
{
public:
    GReadPipe() {}
    virtual ~GReadPipe() {}

    /**
     * open pipe
     * @param [in] pipeName : GPipe name
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

private:
    /**
     * prevent copying
     * @note
     */	
	GReadPipe(const GReadPipe&);

    /**
     * prevent copying
     * @note
     */		
	void operator=(const GReadPipe&);	
};

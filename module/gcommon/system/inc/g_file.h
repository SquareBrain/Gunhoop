/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_file.h
* @version     
* @brief      
* @author   duye
* @date     2014-06-20
* @note 
*
*  1. 2014-06-20 duye Created this file
* 
*/

#pragma once

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <g_type.h>

G_NS_GCOMMON_BEG

/** 
 * file operation wrapper
 */
class File
{
public:
    File();
    ~File();

    /**
     * create file
     * @param [in] filePath : file path
     * @return true/false
     * @note 
     */		
	bool createFile(const GInt8* filePath);    

    /**
     * open file for reading
     * @param [in] filePath : file path
     * @return true/false
     * @note 
     */		
	bool openR(const GInt8* filePath);

    /**
     * open file for writing
     * @param [in] filePath : file path
     * @return true/false
     * @note 
     */		
	bool openW(const GInt8* filePath);

    /**
     * open file for writing, and appended content
     * @param [in] filePath : file path
     * @return true/false
     * @note 
     */			
	bool openWA(const GInt8* filePath);	

    /**
     * open file for writing, and create it when isn't exist
     * @param [in] filePath : file path
     * @return true/false
     * @note 
     */		
	bool openWC(const GInt8* filePath);

    /**
     * open file for writing, and create it when isn't exist, and append content
     * @param [in] filePath : file path
     * @return true/false
     * @note 
     */		
	bool openWCA(const GInt8* filePath);	

    /**
     * open file for reading and writing
     * @param [in] filePath : file path
     * @return true/false
     * @note 
     */		
	bool openRW(const GInt8* filePath);

    /**
     * open file for reading and writeing, and appended content
     * @param [in] filePath : file path
     * @return true/false
     * @note 
     */		
	bool openRWA(const GInt8* filePath);	
	
    /**
     * open file for randing and writing, and craete it when isn't exist
     * @param [in] filePath : file path
     * @return true/false
     * @note 
     */		
	bool openRWC(const GInt8* filePath);

    /**
     * open file for rand and write, and craete it when isn't exist, and append content
     * @param [in] filePath : file path
     * @return true/false
     * @note 
     */			
	bool openRWCA(const GInt8* filePath);	

    /**
     * read file
     * @param [out] buffer : output buffer
	 * @param [in] size : the size of buffer     
     * @return the size of read
     * @note 
     */			
	GUint64 readFile(GInt8* buffer, const GUint64 size);

    /**
     * read a line    
 	 * @param [in] offset : the offset from the begin position
	 * @param [out] buffer : output buffer
	 * @param [in] size : the size of buffer    
     * @return the size of readed
     * @note 
     */		
	GUint64 readLine(const GUint64 offset, GInt8* buffer, const GUint64 size);

    /**
     * write file
	 * @param [in] buffer : input buffer
	 * @param [in] size : the size of buffer
     * @return the size of writed
     * @note 
     */		
	GUint64 writeFile(const GInt8* buffer, const GUint64 size);
	
    /**
     * close file
	 * @param [in] buffer : input buffer
     * @return true/false
     * @note 
     */		
	bool closeFile();

private:
    /**
     * origin API for open file
	 * @param [in] filePath : file path
	 * @param [in] flags : operation flags
	 * @param [in] mode : operation mode
     * @return true/false
     * @note 
     */			
	bool orgOpen(const GInt8* filePath, const GInt32 flags, const GUint32 mode = 0);

private:
    GInt32			m_fd;
    GInt32			m_flags;
	GInt8		    m_path[G_PATH_MAX];
};

G_NS_GCOMMON_END

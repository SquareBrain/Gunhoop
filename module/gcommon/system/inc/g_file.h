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
#include <g_type.h>

static const GUint32 DEF_ERROR_BUF_SIZE = 256;

G_NS_GCOMMON_BEG

class FileUtil
{
public:
    /**
     * create file
     * @param [in] filePath : file path
     * @return G_YES/G_NO
     * @note 
     */	
    static GResult createFile(const GInt8* filePath); 
};

/** 
 * file operation wrapper
 */
class File
{
public:
    File();

    /**
     * set file path
     * @param [in] filePath : file path
     * @note 
     */	    
    explicit File(const GInt8* filePath);
    
    ~File();

    /**
     * set file path
     * @param [in] filePath : file path
     * @return G_YES/G_NO
     * @note 
     */		
	GResult setFilePath(const GInt8* filePath);

    /**
     * get file mode
     * @return mode
     * @note 
     */		
	GUint32 getMode() const;	
	
    /**
     * open file for reading
     * @return G_YES/G_NO
     * @note 
     */		
	GResult openR();

    /**
     * open file for writing
     * @return G_YES/G_NO
     * @note 
     */		
	GResult openW();

    /**
     * open file for writing, and appended content
     * @return G_YES/G_NO
     * @note 
     */			
	GResult openWA();	

    /**
     * open file for writing, and create it when isn't exist
     * @return G_YES/G_NO
     * @note 
     */		
	GResult openWC();

    /**
     * open file for writing, and create it when isn't exist, and append content
     * @return G_YES/G_NO
     * @note 
     */		
	GResult openWCA();	

    /**
     * open file for reading and writing
     * @return G_YES/G_NO
     * @note 
     */		
	GResult openRW();

    /**
     * open file for reading and writeing, and appended content
     * @return G_YES/G_NO
     * @note 
     */		
	GResult openRWA();	
	
    /**
     * open file for randing and writing, and craete it when isn't exist
     * @return G_YES/G_NO
     * @note 
     */		
	GResult openRWC();

    /**
     * open file for rand and write, and craete it when isn't exist, and append content
     * @return G_YES/G_NO
     * @note 
     */			
	GResult openRWCA();	

    /**
     * get file size
     * @param [out] buffer : output buffer
	 * @param [in] size : the size of buffer     
     * @return size/-1
     * @note 
     */			
	GInt64 getFileSize();
	
    /**
     * read file
     * @param [out] buffer : output buffer
	 * @param [in] size : the size of buffer     
     * @return size/-1
     * @note 
     */			
	GInt64 readFile(GInt8* buffer, const GUint64 size);

    /**
     * read a line    
 	 * @param [in] offset : the offset from the begin position
	 * @param [out] buffer : output buffer
	 * @param [in] size : the size of buffer    
     * @return size/-1
     * @note 
     */		
	GInt64 readLine(const GUint64 offset, GInt8* buffer, const GUint64 size);

    /**
     * write file
	 * @param [in] buffer : input buffer
	 * @param [in] size : the size of buffer
     * @return size/-1
     * @note 
     */		
	GInt64 writeFile(const GInt8* buffer, const GUint64 size);
	
    /**
     * close file
	 * @param [in] buffer : input buffer
     * @return G_YES/G_NO
     * @note 
     */		
	GResult closeFile();

    /**
     * get last error string
	 * @param [out] error : output buffer
	 * @param [in] size : output buffer size
     * @return G_YES/G_NO
     * @note 
     */		
	GResult getLastError(GInt8* error, const GUint32 size);	

private:
    /**
     * origin API for open file
	 * @param [in] flags : operation flags
	 * @param [in] mode : operation mode
     * @return G_YES/G_NO
     * @note 
     */			
	GResult orgOpen(const GInt32 flags, const GUint32 mode = 0);
	
    /**
     * origin set program running error
	 * @param [in] error : error string
     * @note 
     */		
	void setError(const GInt8* error);

private:
    GInt32			m_fd;
    struct stat     m_fileStat;
    GInt32			m_flags;
	GInt8		    m_path[G_PATH_MAX];
	GUint32         m_pathLen;
	GInt8           m_error[DEF_ERROR_BUF_SIZE];
};

G_NS_END
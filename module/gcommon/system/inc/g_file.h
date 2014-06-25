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
     * @param [in] filePath : file path
     * @return G_YES/G_NO
     * @note 
     */		
	GResult openR();

    /**
     * open file for writing
     * @param [in] filePath : file path
     * @return G_YES/G_NO
     * @note 
     */		
	GResult openW(const GInt8* filePath);

    /**
     * open file for writing, and appended content
     * @param [in] filePath : file path
     * @return G_YES/G_NO
     * @note 
     */			
	GResult openWA(const GInt8* filePath);	

    /**
     * open file for writing, and create it when isn't exist
     * @param [in] filePath : file path
     * @return G_YES/G_NO
     * @note 
     */		
	GResult openWC(const GInt8* filePath);

    /**
     * open file for writing, and create it when isn't exist, and append content
     * @param [in] filePath : file path
     * @return G_YES/G_NO
     * @note 
     */		
	GResult openWCA(const GInt8* filePath);	

    /**
     * open file for reading and writing
     * @param [in] filePath : file path
     * @return G_YES/G_NO
     * @note 
     */		
	GResult openRW(const GInt8* filePath);

    /**
     * open file for reading and writeing, and appended content
     * @param [in] filePath : file path
     * @return G_YES/G_NO
     * @note 
     */		
	GResult openRWA(const GInt8* filePath);	
	
    /**
     * open file for randing and writing, and craete it when isn't exist
     * @param [in] filePath : file path
     * @return G_YES/G_NO
     * @note 
     */		
	GResult openRWC(const GInt8* filePath);

    /**
     * open file for rand and write, and craete it when isn't exist, and append content
     * @param [in] filePath : file path
     * @return G_YES/G_NO
     * @note 
     */			
	GResult openRWCA(const GInt8* filePath);	

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

private:
    /**
     * origin API for open file
	 * @param [in] filePath : file path
	 * @param [in] flags : operation flags
	 * @param [in] mode : operation mode
     * @return G_YES/G_NO
     * @note 
     */			
	GResult orgOpen(const GInt8* filePath, const GInt32 flags, const GUint32 mode = 0);

private:
    GInt32			m_fd;
    GInt32			m_flags;
    GUint32         m_mode;
	GInt8		    m_path[G_PATH_MAX];
	GUint32         m_pathLen;
};

G_NS_GCOMMON_END

/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @GFile		g_file.h
* @version     
* @brief      
* @author   duye
* @date     2014-06-20
* @note 
*
*  1. 2014-06-20 duye Created this GFile
* 
*/

#pragma once

#include <sys/stat.h>
#include <g_type.h>

/** 
 * file open flags
 */
typedef enum
{
    ONLY_READ = 1,
    ONLY_WRITE = 2,
    READ_WRITE = 4,
    OPEN_APPEND = 8
} OpenFlags;

/** 
 * file utility
 */
class GGFileUtil
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
class GFile
{
public:
    GFile();

    /**
     * set file path
     * @param [in] GFilePath : GFile path
     * @note 
     */	    
    explicit GFile(const GInt8* filePath);
    
    ~GFile();

    /**
     * set file path
     * @param [in] GFilePath : file path
     * @return G_YES/G_NO
     * @note 
     */		
	GResult setFilePath(const GInt8* filePath);
	
    /**
     * open file for reading
     * @param [in] flags : open mode flags
     * @return G_YES/G_NO
     * @note 
     */		
	GResult openFile(const GUint64 flags);

    /**
     * close file
     * @return G_YES/G_NO
     * @note 
     */		
    GResult closeFile();
    
    /**
     * get file size
     * @param [out] buffer : output buffer
	 * @param [in] size : the size of buffer     
     * @return size/G_NO
     * @note 
     */			
	GInt64 getFileSize();
	
    /**
     * read file
     * @param [out] buffer : output buffer
	 * @param [in] size : the size of buffer     
     * @return size/G_NO
     * @note 
     */			
	GInt64 readFile(GInt8* buffer, const GUint64 size);

    /**
     * write file
	 * @param [in] buffer : input buffer
	 * @param [in] size : the size of buffer
     * @return size/G_NO
     * @note 
     */		
	GInt64 writeFile(const GInt8* data, const GUint64 length);

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
	GResult orgOpen(const GInt32 flags, const GUint32 mode);
	
    /**
     * origin set program running error
	 * @param [in] error : error string
     * @note 
     */		
	void setError(const GInt8* args, ...);

private:
    GInt32			m_fd;
    struct stat     m_fileStat;
    GInt32			m_flags;
	GInt8		    m_path[G_PATH_MAX];
	GUint32         m_pathLen;
	GInt8           m_error[G_ERROR_BUF_SIZE];
};
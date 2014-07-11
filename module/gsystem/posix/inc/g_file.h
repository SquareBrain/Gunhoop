/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @GFile		g_GFile.h
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
 * GFile open flags
 */
typedef enum
{
    ONLY_READ = 1,
    ONLY_WRITE = 2,
    READ_WRITE = 4,
    OPEN_APPEND = 8
} OpenFlags;

/** 
 * GFile utility
 */
class GGFileUtil
{
public:
    /**
     * create GFile
     * @param [in] GFilePath : GFile path
     * @return G_YES/G_NO
     * @note 
     */	
    static GResult createGFile(const GInt8* GFilePath); 
};

/** 
 * GFile operation wrapper
 */
class GFile
{
public:
    GFile();

    /**
     * set GFile path
     * @param [in] GFilePath : GFile path
     * @note 
     */	    
    explicit GFile(const GInt8* GFilePath);
    
    ~GFile();

    /**
     * set GFile path
     * @param [in] GFilePath : GFile path
     * @return G_YES/G_NO
     * @note 
     */		
	GResult setGFilePath(const GInt8* GFilePath);
	
    /**
     * open GFile for reading
     * @param [in] flags : open mode flags
     * @return G_YES/G_NO
     * @note 
     */		
	GResult openGFile(const GUint64 flags);

    /**
     * close GFile
     * @return G_YES/G_NO
     * @note 
     */		
    GResult closeGFile();
    
    /**
     * get GFile size
     * @param [out] buffer : output buffer
	 * @param [in] size : the size of buffer     
     * @return size/G_NO
     * @note 
     */			
	GInt64 getGFileSize();
	
    /**
     * read GFile
     * @param [out] buffer : output buffer
	 * @param [in] size : the size of buffer     
     * @return size/G_NO
     * @note 
     */			
	GInt64 readGFile(GInt8* buffer, const GUint64 size);

    /**
     * write GFile
	 * @param [in] buffer : input buffer
	 * @param [in] size : the size of buffer
     * @return size/G_NO
     * @note 
     */		
	GInt64 writeGFile(const GInt8* data, const GUint64 length);

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
     * origin API for open GFile
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
    struct stat     m_GFileStat;
    GInt32			m_flags;
	GInt8		    m_path[G_PATH_MAX];
	GUint32         m_pathLen;
	GInt8           m_error[G_ERROR_BUF_SIZE];
};
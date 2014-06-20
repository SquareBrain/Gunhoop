/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		file.h
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

G_NS_GCOMMON_BEG

const GUint32 G_CREATE_MODE = 0x775;

// brief : file operation wrapper
class File
{
public:
    File();
    ~File();

 	// brief : create file
	// @para [in]filePath : file path
	// @return true on success, or false on error
	bool Create(const GInt8* filePath);    

 	// brief : open file for read
	// @para [in]filePath : file path
	// @return true on success, or false on error	
	bool OpenR(const GInt8* filePath);

 	// brief : open file for write
	// @para [in]filePath : file path
	// @return true on success, or false on error	
	bool OpenW(const GInt8* filePath);

 	// brief : open file for write, and append content
	// @para [in]filePath : file path
	// @return true on success, or false on error	
	bool OpenWA(const GInt8* filePath);	

 	// brief : open file for write, and create it when isn't exist
	// @para [in]filePath : file path
	// @return true on success, or false on error	
	bool OpenWC(const GInt8* filePath);

 	// brief : open file for write, and create it when isn't exist, and append content
	// @para [in]filePath : file path
	// @return true on success, or false on error	
	bool OpenWCA(const GInt8* filePath);	

 	// brief : open file for read and write
	// @para [in]filePath : file path
	// @return true on success, or false on error	
	bool OpenRW(const GInt8* filePath);

 	// brief : open file for read and write, and append content
	// @para [in]filePath : file path
	// @return true on success, or false on error	
	bool OpenRWA(const GInt8* filePath);	

 	// brief : open file for rand and write, and craete it when isn't exist
	// @para [in]filePath : file path
	// @return true on success, or false on error	
	bool OpenRWC(const GInt8* filePath);

 	// brief : open file for rand and write, and craete it when isn't exist, and append content
	// @para [in]filePath : file path
	// @return true on success, or false on error	
	bool OpenRWCA(const GInt8* filePath);	

 	// brief : read file
	// @para [out]buffer : output buffer
	// @para [in]size : the size of buffer
	// @return the size of read
	GUint64 Read(GInt8* buffer, const GUint64 size);

 	// brief : read file
 	// @para [in]offset : the offset from the begin position
	// @para [out]buffer : output buffer
	// @para [in]size : the size of buffer
	// @return the size of read
	GUint64 ReadLine(const GUint64 offset, GInt8* buffer, const GUint64 size);	

 	// brief : write file
	// @para [in]buffer : input buffer
	// @para [in]size : the size of buffer
	// @return the size of write
	GUint64 Write(const GInt8* buffer, const GUint64 size);	

 	// brief : close file
	// @return true on success, or false on error
	bool Close();

private:
 	// brief : origin API for open file
	// @para [in]filePath : file path
	// @para [in]flags : operation flags
	// @return true on success, or false on error
	bool OrgOpen(const GInt8* filePath, const GInt32 flags, const GUint32 mode = 0);

private:
    GInt32     m_fd;
    GInt8      m_path[D_PATH_MAX];
    GInt32     m_flags;
};

DUYE_POSIX_NS_END

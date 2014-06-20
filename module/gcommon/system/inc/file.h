/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		duye_file.h
* @version     
* @brief      
* @author   duye
* @date     2013-11-15
* @note 
*
*  1. 2013-11-15 duye Created this file
* 
*/

#pragma once

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <duye/posix/inc/duye_posix_def.h>

DUYE_POSIX_NS_BEG

const D_UInt32 D_CREATE_MODE = 0x775;

// brief : file operation wrapper
class File
{
public:
    File();
    ~File();

 	// brief : create file
	// @para [in]filePath : file path
	// @return true on success, or false on error
	D_Bool Create(const D_Int8* filePath);    

 	// brief : open file for read
	// @para [in]filePath : file path
	// @return true on success, or false on error	
	D_Bool OpenR(const D_Int8* filePath);

 	// brief : open file for write
	// @para [in]filePath : file path
	// @return true on success, or false on error	
	D_Bool OpenW(const D_Int8* filePath);

 	// brief : open file for write, and append content
	// @para [in]filePath : file path
	// @return true on success, or false on error	
	D_Bool OpenWA(const D_Int8* filePath);	

 	// brief : open file for write, and create it when isn't exist
	// @para [in]filePath : file path
	// @return true on success, or false on error	
	D_Bool OpenWC(const D_Int8* filePath);

 	// brief : open file for write, and create it when isn't exist, and append content
	// @para [in]filePath : file path
	// @return true on success, or false on error	
	D_Bool OpenWCA(const D_Int8* filePath);	

 	// brief : open file for read and write
	// @para [in]filePath : file path
	// @return true on success, or false on error	
	D_Bool OpenRW(const D_Int8* filePath);

 	// brief : open file for read and write, and append content
	// @para [in]filePath : file path
	// @return true on success, or false on error	
	D_Bool OpenRWA(const D_Int8* filePath);	

 	// brief : open file for rand and write, and craete it when isn't exist
	// @para [in]filePath : file path
	// @return true on success, or false on error	
	D_Bool OpenRWC(const D_Int8* filePath);

 	// brief : open file for rand and write, and craete it when isn't exist, and append content
	// @para [in]filePath : file path
	// @return true on success, or false on error	
	D_Bool OpenRWCA(const D_Int8* filePath);	

 	// brief : read file
	// @para [out]buffer : output buffer
	// @para [in]size : the size of buffer
	// @return the size of read
	D_UInt64 Read(D_Int8* buffer, const D_UInt64 size);

 	// brief : read file
 	// @para [in]offset : the offset from the begin position
	// @para [out]buffer : output buffer
	// @para [in]size : the size of buffer
	// @return the size of read
	D_UInt64 ReadLine(const D_UInt64 offset, D_Int8* buffer, const D_UInt64 size);	

 	// brief : write file
	// @para [in]buffer : input buffer
	// @para [in]size : the size of buffer
	// @return the size of write
	D_UInt64 Write(const D_Int8* buffer, const D_UInt64 size);	

 	// brief : close file
	// @return true on success, or false on error
	D_Bool Close();

private:
 	// brief : origin API for open file
	// @para [in]filePath : file path
	// @para [in]flags : operation flags
	// @return true on success, or false on error
	D_Bool OrgOpen(const D_Int8* filePath, const D_Int32 flags, const D_UInt32 mode = 0);

private:
    D_Int32     m_fd;
    D_Int8      m_path[D_PATH_MAX];
    D_Int32     m_flags;
};

DUYE_POSIX_NS_END

/*************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_shm.h
* @version     
* @brief      
* @author   duye
* @date     2014-02-22
* @note 
*
*  2. 2014-06-20 duye move to gohoop project 
*  1. 2014-02-22 duye Created this file
* 
*/

#pragma once

#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <g_type.h>

G_NS_GCOMMON_BEG 

// brief : shared memory for ipc
// 
class Shm
{
public:
    typedef enum 
    { 
        // open shm failed
        OPEN_SHM_FAILED = -2,
        // munmap failed
        MUNMAP_SHM_FAILED = -3,
        // mmap failed
        MMAP_SHM_FAILED = -4,
        // sync shm failed
        SYNC_SHM_FAILED = -5,
        // write shm parameter failed
        WRITE_SHM_PARA_FAILED = -6,
        // read shm parameter failed
        READ_SHM_PARA_FAILED = -7,
        // shm not init
        SHM_NO_INIT = -8,
        // shm path is empty
        SHM_PATH_EMPTY = -9
        // uninit
    } ErrorCode;
    
public:
    Shm();
	// brief : constructor
	// @para [in]path : shm mapping file path
	// @para [in]size : shm size
	// note    
    Shm(const GInt8* path, const GUint32 size);
    Shm(const std::string& path, const GUint32 size);
    ~Shm();

	// brief : set shm mapping file path
	// note  
    void SetPath(const GInt8* path);
    void SetPath(const std::string& path);
    const std::string& GetPath() const;

	// brief : set/get shm mapping file size
	// note  
    void SetSize(const GUint32 size);
    GUint32 GetSize() const;    
    
	// brief : init the shm
	// return : successed : 0, failed : error code
	// note  
    GResult Init();

	// brief : uninit the shm
	// return : successed : 0, failed : error code
	// note      
	GResult Uninit();

 	// brief : sync the shm
	// return : successed : 0, failed : error code
	// note      
    GResult Sync();

	// brief : write data to shm
	// @para [in]offset : offset of shm
	// @para [in]data : input data
	// @para [in]size : write size
	// return : successed : 0, failed : error code
	// note   
    GResult Write(const GUint32 offset, const GInt8* data, const GUint32 size);

	// brief : read data from shm
	// @para [in]offset : offset of shm
	// @para [in]data : output data
	// @para [in]size : read size
	// return : successed : 0, failed : error code
	// note   
    GResult Read(const GUint32 offset, GInt8* data, const GUint32 size);

private:
	std::string		m_shmPath;
	GUint32			m_shmSize;	
	void*			m_shmAddr;
};

G_NS_GCOMMON_END

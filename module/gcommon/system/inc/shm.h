/*************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     duye_shm.h
* @version     
* @brief      
* @author   duye
* @date     2014-02-22
* @note 
*
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

#include <duye/posix/inc/duye_posix_def.h>

DUYE_POSIX_NS_BEG 

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
    Shm(const D_Int8* path, const D_UInt32 size);
    Shm(const std::string& path, const D_UInt32 size);
    ~Shm();

	// brief : set shm mapping file path
	// note  
    void SetPath(const D_Int8* path);
    void SetPath(const std::string& path);
    const std::string& GetPath() const;

	// brief : set/get shm mapping file size
	// note  
    void SetSize(const D_UInt32 size);
    D_UInt32 GetSize() const;    
    
	// brief : init the shm
	// return : successed : 0, failed : error code
	// note  
    D_Result Init();

	// brief : uninit the shm
	// return : successed : 0, failed : error code
	// note      
	D_Result Uninit();

 	// brief : sync the shm
	// return : successed : 0, failed : error code
	// note      
    D_Result Sync();

	// brief : write data to shm
	// @para [in]offset : offset of shm
	// @para [in]data : input data
	// @para [in]size : write size
	// return : successed : 0, failed : error code
	// note   
    D_Result Write(const D_UInt32 offset, const D_Int8* data, const D_UInt32 size);

	// brief : read data from shm
	// @para [in]offset : offset of shm
	// @para [in]data : output data
	// @para [in]size : read size
	// return : successed : 0, failed : error code
	// note   
    D_Result Read(const D_UInt32 offset, D_Int8* data, const D_UInt32 size);

private:
	std::string     m_shmPath;
	D_UInt32        m_shmSize;	
	D_Void*         m_shmAddr;
};

DUYE_POSIX_NS_END

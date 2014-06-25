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
#include <g_type.h>

G_NS_GCOMMON_BEG 

/** 
 * shm error code
 */
typedef enum 
{ 
    /** 
     * open shm failed
     */        
    OPEN_SHM_FAILED = -2,
    /** 
     * munmap failed
     */           
    MUNMAP_SHM_FAILED = -3,
    /** 
     * mmap failed
     */           
    MMAP_SHM_FAILED = -4,
    /** 
     * sync shm failed
     */           
    SYNC_SHM_FAILED = -5,
    /** 
     * write shm parameter failed
     */           
    WRITE_SHM_PARA_FAILED = -6,
    /** 
     * read shm parameter failed
     */           
    READ_SHM_PARA_FAILED = -7,
    /** 
     * shm not init
     */           
    SHM_NO_INIT = -8,
    /** 
     * shm path is empty
     */           
    SHM_PATH_EMPTY = -9
} ShmErrorCode;

/** 
 * shared memory for ipc
 */
class Shm
{
public:
    Shm();   
    /**
     * constructor
	 * @para [in] shmPath : shm mapping file path
	 * @para [in] shmSize : shm size
	 * @return true/false
     * @note 
     */		
    Shm(const GInt8* shmPath, const GUint32 shmSize);
    ~Shm();

    /**
     * set shm mapping file path
	 * @para [in] shmPath : shm mapping file path     
     * @note 
     */	
    void SetShmPath(const GInt8* shmPath);

    /**
     * set shm mapping file path
	 * @return shm mapping file path   
     * @note 
     */	
    GInt8* GetShmPath() const;

	// brief : set/get shm mapping file size
	// note  
    void SetShmSize(const GUint32 size);
    GUint32 GetShmSize() const;    
    
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
	GInt8*		    m_shmPath[G_PATH_MAX];
	GUint32			m_shmSize;	
	void*			m_shmAddr;
};

G_NS_GCOMMON_END

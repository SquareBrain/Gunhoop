/*************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     	g_shm.h
* @version     
* @brief      
* @author   	duye
* @date     	2014-02-22
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

/** 
 * @brief shm error code
 */
typedef enum 
{ 
	/** 
	 * @brief open shm failed
	 */        
    OPEN_SHM_FAILED = -2,
	/** 
	 * @brief munmap failed
	 */           
    MUNMAP_SHM_FAILED = -3,
	/** 
	 * @brief mmap failed
	 */           
    MMAP_SHM_FAILED = -4,
	/** 
	 * @brief sync shm failed
	 */           
    SYNC_SHM_FAILED = -5,
	/** 
	 * @brief write shm parameter failed
	 */           
    WRITE_SHM_PARA_FAILED = -6,
	/** 
	 * @brief read shm parameter failed
	 */           
    READ_SHM_PARA_FAILED = -7,
	/** 
	 * @brief shm not init
	 */           
    SHM_NO_INIT = -8,
	/** 
	 * @brief shm path is empty
	 */           
    SHM_PATH_EMPTY = -9
} GShmErrorCode;

/** 
 * @brief shared memory for ipc
 */
class GShm
{
public:
    GShm();   
	/**
	 * @brief constructor
	 * @param [in] GShmPath : shm mapping file path
	 * @param [in] GShmSize : shm size
	 * @return true/false
	 * @note 
	 */		
    GShm(const GInt8* shmPath, const GUint64 shmSize);
    ~GShm();

	/**
	 * @brief set shm mapping file path
	 * @param [in] shmPath : shm mapping file path     
	 * @note 
	 */	
    void setPath(const GInt8* shmPath);

	/**
	 * @brief set shm mapping file size
	 * @param [in] shmSize : shm size  
	 * @note 
	 */		
    void setSize(const GUint64 shmSize);
   
	/**
	 * @brief sync the shm
	 * @return G_YES/G_NO  
	 * @note 
	 */	
    GResult sync();
   
	/**
	 * @brief write data to shm
	 * @param [in] offset : offset of shm
	 * @param [in] data : input data
	 * @param [in] size : write size     
	 * @return G_YES/G_NO  
	 * @note 
	 */		
    GResult write(const GUint32 offset, const GInt8* data, const GUint32 length);

	/**
	 * @brief read data from shm
	 * @param [in] offset : offset of shm
	 * @param [in] buffer : output buffer
	 * @param [in] size : read size    
	 * @return G_YES/G_NO  
	 * @note 
	 */	
    GResult read(const GUint32 offset, GInt8* buffer, const GUint32 size);

private:
	/**
	 * @brief init the shm
	 * @return G_YES/G_NO  
	 * @note 
	 */	    	
    GResult init();
   
	/**
	 * @brief uninit the shm
	 * @return G_YES/G_NO  
	 * @note 
	 */	 	
	GResult uninit();

private:
	GInt8		    m_shmPath[G_PATH_MAX];
	GUint64			m_shmSize;	
	void*			m_shmAddr;
	bool            m_initFlags;
};
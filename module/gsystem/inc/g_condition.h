/************************************************************************************
**  
* @copyright (c) 2010-2019, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_GCondition.h
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

#include <sys/time.h>
#include <pthread.h>
#include <g_type.h>

/** 
 * POSIX GCondition wrapper
 */
class GCondition 
{
public:
	GCondition();
	~GCondition();

    /**
     * wake up the waitting thread, only wake up one thread,
     * according to the thread priority and waitting time.
     * @return true/false
     * @note 
     */ 
	bool signal();
	 
    /**
     * wake up all waitting threads
     * @return true/false
     * @note 
     */		
	bool broadcast();
	
    /**
     * let current thread has reached the awaited state, utill be wake up
     * @return true/false
     * @note 
     */		
	bool wait();
	
    /**
     * let current thread has reached the awaited state,
     * utill be wake up or timeout return
     * @param [in] timeout : waitting timeout
     * @return true/false
     * @note 
     */	
	bool wait(const GUint32 timeout);

private: 
    /**
     * for to prevent copying
     * @param
     * @return true/false
     * @note 
     */		
	GCondition(const GCondition&); 
	
    /**
     * prevent copying
     * @note
     */	
	void operator=(const GCondition&);	
    
private:
	pthread_cond_t		m_GCondition;
	pthread_mutex_t		m_mutex;	
};
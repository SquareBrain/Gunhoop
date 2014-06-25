/************************************************************************************
**  
* @copyright (c) 2010-2019, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_condition.h
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

G_NS_GCOMMON_BEG

/** 
 * POSIX condition wrapper
 */
class Condition 
{
public:
	Condition();
	~Condition();

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
	Condition(const Condition&); 
	void operator=(const Condition&);	
    
private:
	pthread_cond_t		m_condition;
	pthread_mutex_t		m_mutex;	
};

G_NS_GCOMMON_END

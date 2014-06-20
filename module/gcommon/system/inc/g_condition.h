/************************************************************************************
**  
* @copyright (c) 2010-2019, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    condition.h
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

G_NS_GCOMMON_BEG

// brief : POSIX condition wrapper
//	
// usage :
//	Condition myCond;
//  
//  void ThreadOne()
//  {
//      if (!myCond.Wait())
//      {
//          return;
//      }
//      
//      // to do
//  }
//
//  void ThreadTwo()
//  {
//      sleep(1);
//      myCond.Signal();
//  }
class Condition 
{
public:
	Condition();
	~Condition();

	// brief : wake up the waitting thread, only wake up one thread,  
	// according to the thread priority and waitting time
	// @para
	// @return true/false
	// note:
	bool Signal();
	
	// brief : wake up all waitting threads
	// @para
	// @return true/false
	// note:
	bool Broadcast();
	
	// brief : let current thread has reached the awaited state, utill be wake up
	// @para
	// @return true/false
	// note:
	bool Wait();
	
	// brief : let current thread has reached the awaited state, 
	// utill be wake up or timeout return
	// @para [in]timeout : waitting timeout
	// @return true/false
	// note:
	bool Wait(const GUint32 timeout);

private:
	// brief : to prevent copying
	// @para 
	// @return 
	// note:
	Condition(const Condition&); 
	void operator=(const Condition&);	
    
private:
	pthread_cond_t		m_condition;
	pthread_mutex_t		m_mutex;	
};

DUYE_POSIX_NS_END

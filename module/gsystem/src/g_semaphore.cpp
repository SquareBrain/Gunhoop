/************************************************************************************
**  
* @copyright (c) 2010-2019, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_semaphore.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-06-20
* @note 
*
*  1. 2014-06-20 duye Created this file
* 
*/
#include <g_semaphore.h>

namespace gsys {
	
Semaphore::Semaphore()
{
	// 2st paramater not equal to zero indicate shared in process
	// otherwise shared in all thread in current process
	// 3st paramater is the initialization value of semaphore
	sem_init(&m_sem, 0, 0);
}

Semaphore::~Semaphore()
{
	sem_destroy(&m_sem);
}

bool Semaphore::wait()
{
	return sem_wait(&m_sem) == 0 ? true : false;
}

bool Semaphore::tryWait()
{
	return sem_trywait(&m_sem) == 0 ? true : false;
}

bool timedWait(const GUint64 timeout)
{
	struct timespec abs_timeout;
	return sem_timedwait(&m_sem, abs_timeout) == 0 ? true : false;	
}

bool Semaphore::post()
{
	return sem_post(&m_sem) == 0 ? true : false;
}

}

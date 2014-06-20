/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    duye_condition.cpp
* @version     
* @brief      
* @author   duye
* @date     2013-12-22
* @note 
*
*  1. 2013-12-22 duye Created this file
* 
*/

#include <duye/posix/thread/inc/duye_condition.h>

DUYE_POSIX_NS_BEG

//----------------------------class Condition-------------------------//
Condition::Condition()       
{ 
    pthread_cond_init(&m_condition, 0);        
    pthread_mutex_init(&m_mutex, 0);       
}       

Condition::~Condition()     
{      
    pthread_cond_destroy(&m_condition);        
    pthread_mutex_destroy(&m_mutex);    
}

bool Condition::Signal()        
{        
    pthread_mutex_lock(&m_mutex);
    D_Int32 ret = pthread_cond_signal(&m_condition);
    pthread_mutex_unlock(&m_mutex);
    
    return (ret == 0 ? true : false);
}    

bool Condition::Broadcast() 
{     
    pthread_mutex_lock(&m_mutex);
    D_Int32 ret = pthread_cond_broadcast(&m_condition);
    pthread_mutex_unlock(&m_mutex);

    return (ret == 0 ? true : false);
}     

bool Condition::Wait()  
{        
    pthread_mutex_lock(&m_mutex);
    D_Int32 ret = pthread_cond_wait(&m_condition, &m_mutex);
    pthread_mutex_unlock(&m_mutex);

    return (ret == 0 ? true : false);
}      

bool Condition::Wait(const D_UInt32 timeout)     
{       
    struct timeval now;    
    struct timespec tmpTimeout;  
    
    gettimeofday(&now, 0);     
    
    tmpTimeout.tv_sec = now.tv_sec + timeout / 1000;       
    tmpTimeout.tv_nsec = now.tv_usec * 1000 + timeout % 1000 * 1000 * 1000;    

    pthread_mutex_lock(&m_mutex);
    D_Int32 ret = pthread_cond_timedwait(&m_condition, &m_mutex, &tmpTimeout);
    pthread_mutex_unlock(&m_mutex);

    return (ret == 0 ? true : false);
}

DUYE_POSIX_NS_END

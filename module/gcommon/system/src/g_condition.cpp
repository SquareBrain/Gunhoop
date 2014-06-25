/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_condition.cpp
* @version     
* @brief      
* @author   duye
* @date     2013-06-20
* @note 
*
*  1. 2013-06-20 duye Created this file
* 
*/

#include <g_condition.h>

G_NS_GCOMMON_BEG

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

bool Condition::signal()        
{        
    pthread_mutex_lock(&m_mutex);
    GInt32 ret = pthread_cond_signal(&m_condition);
    pthread_mutex_unlock(&m_mutex);
    
    return (ret == 0 ? true : false);
}    

bool Condition::broadcast() 
{     
    pthread_mutex_lock(&m_mutex);
    GInt32 ret = pthread_cond_broadcast(&m_condition);
    pthread_mutex_unlock(&m_mutex);

    return (ret == 0 ? true : false);
}     

bool Condition::wait()  
{        
    pthread_mutex_lock(&m_mutex);
    GInt32 ret = pthread_cond_wait(&m_condition, &m_mutex);
    pthread_mutex_unlock(&m_mutex);

    return (ret == 0 ? true : false);
}      

bool Condition::wait(const GUint32 timeout)     
{       
    struct timeval now;    
    struct timespec tmpTimeout;  
    
    gettimeofday(&now, 0);     
    
    tmpTimeout.tv_sec = now.tv_sec + timeout / 1000;       
    tmpTimeout.tv_nsec = now.tv_usec * 1000 + timeout % 1000 * 1000 * 1000;    

    pthread_mutex_lock(&m_mutex);
    GInt32 ret = pthread_cond_timedwait(&m_condition, &m_mutex, &tmpTimeout);
    pthread_mutex_unlock(&m_mutex);

    return (ret == 0 ? true : false);
}

G_NS_GCOMMON_END

/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_GCondition.cpp
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

GCondition::GCondition()       
{ 
    pthread_cond_init(&m_GCondition, 0);        
    pthread_mutex_init(&m_mutex, 0);       
}       

GCondition::~GCondition()     
{      
    pthread_cond_destroy(&m_GCondition);        
    pthread_mutex_destroy(&m_mutex);    
}

bool GCondition::signal()        
{        
    pthread_mutex_lock(&m_mutex);
    GInt32 ret = pthread_cond_signal(&m_GCondition);
    pthread_mutex_unlock(&m_mutex);
    
    return (ret == 0 ? true : false);
}    

bool GCondition::broadcast() 
{     
    pthread_mutex_lock(&m_mutex);
    GInt32 ret = pthread_cond_broadcast(&m_GCondition);
    pthread_mutex_unlock(&m_mutex);

    return (ret == 0 ? true : false);
}     

bool GCondition::wait()  
{        
    pthread_mutex_lock(&m_mutex);
    GInt32 ret = pthread_cond_wait(&m_GCondition, &m_mutex);
    pthread_mutex_unlock(&m_mutex);

    return (ret == 0 ? true : false);
}      

bool GCondition::wait(const GUint32 timeout)     
{       
    struct timeval now;    
    struct timespec tmpTimeout;  
    
    gettimeofday(&now, 0);     
    
    tmpTimeout.tv_sec = now.tv_sec + timeout / 1000;       
    tmpTimeout.tv_nsec = now.tv_usec * 1000 + timeout % 1000 * 1000 * 1000;    

    pthread_mutex_lock(&m_mutex);
    GInt32 ret = pthread_cond_timedwait(&m_GCondition, &m_mutex, &tmpTimeout);
    pthread_mutex_unlock(&m_mutex);

    return (ret == 0 ? true : false);
}

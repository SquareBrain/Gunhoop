/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	     g_sync.h
* @version      
* @brief      
* @author    duye
* @date	     2014-07-18
* @note 
*
*  1. 2014-07-18 duye Created this file
* 
*/
#pragma once

#include <g_system.h>

/** 
 * @brief synchronized code scope
 */
#define synchronized(mtx) for (GInt32 i = 0; i < 1; i ++) for (gsys::AutoLock lock(mtx); i < 1; i ++)

namespace gcom {

/** 
 * @brief secrity object template
 */
template <class T>
class SecrityObj : public T
{
public:
    SecrityObj() {}
    ~SecrityObj() {}
    
    /**
     * @brief auto lock
     * @param [in] auto_lock : auto lock object
     */
    void lock();   
    void unlock();
    gsys::Mutex& mutex() const;
    
private:
    gsys::Mutex	m_mutex;
};

template <class T> 
void SecrityObj::lock()
{
    m_mutex.lock();
}

template <class T>
void SecrityObj::unlock()
{
    m_mutex.unlock();
}

template <class T>
gsys::Mutex Secrity::mutex() const
{
    return m_mutex;
}
}

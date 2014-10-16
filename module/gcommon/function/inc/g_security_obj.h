/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	     g_security_obj.h
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
template <typename T>
class SecurityObj : public T
{
public:
    SecurityObj() {}
    ~SecurityObj() {}
    
    void lock();   
    void unlock();

    /**
     * @brief get this mutex, can use AutoLock(mutex)
     * @return mutex
     */
    gsys::Mutex& mutex();
    
private:
    gsys::Mutex	m_mutex;
};

template <class T> 
void SecurityObj<T>::lock()
{
    m_mutex.lock();
}

template <class T>
void SecurityObj<T>::unlock()
{
    m_mutex.unlock();
}

template <class T>
gsys::Mutex& SecurityObj<T>::mutex()
{
    return m_mutex;
}
}

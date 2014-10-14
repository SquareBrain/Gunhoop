/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_lock.cpp
* @version     
* @brief      
* @author   duye
* @date	    2013-11-26
* @note 
*
* 3. 2014-06-21 duye move to gohoop project 
* 2. 2014-01-04 duye 
*     a. Change GMutex function GTryLock() to GTryLock()
*     b. Add function GTryLock() for GOrgLock class
*     c. Modify GTryLock class implimenting
* 1. 2013-11-26 duye Created this file
*/
#include <g_lock.h>

namespace gsys {

Mutex::Mutex()
{
    init(PTHREAD_MUTEX_RECURSIVE);
}

Mutex::Mutex(const GInt32 kind)
{
    if (kind != PTHREAD_MUTEX_NORMAL && 
	kind != PTHREAD_MUTEX_RECURSIVE &&
	kind != PTHREAD_MUTEX_ERRORCHECK &&
	kind != PTHREAD_MUTEX_DEFAULT)
    {
	init(PTHREAD_MUTEX_DEFAULT);
    }
    else
    {
 	init(kind);
    }
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&m_mutex);
}

bool Mutex::lock() 
{
    return pthread_mutex_lock(&m_mutex) == 0 ? true : false;
}

bool Mutex::tryLock()
{
    return pthread_mutex_trylock(&m_mutex) == 0 ? true : false;
}

bool Mutex::unlock()
{
    return pthread_mutex_unlock(&m_mutex) == 0 ? true : false;
}

void Mutex::init(const GInt32 kind)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, kind);
    pthread_mutex_init(&m_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

TryLock::TryLock(Mutex& mutex, const bool autoUnlock) : m_mutex(mutex), m_autoUnlock(autoUnlock) {}

TryLock::~TryLock()
{
    if (m_autoUnlock)
    {
    	m_mutex.unlock();
    }
}

bool TryLock::lock(const GUint32 timeout)
{
    if (m_mutex.tryLock())
    {
    	return true;
    }

    if (timeout == 0)
    {
        return false;    
    }

    static const GUint32 sleepUnit = 10; 
    GUint32 loops = timeout / sleepUnit + 1;
						
    do
    {
		if (loops == 1)
		{
	  	    usleep(1000 * (timeout % sleepUnit));
		}
		else
		{
		    usleep(1000 * sleepUnit);    
		}

		if (m_mutex.tryLock())
		{
		    return true;
		}
    } while(--loops);

    return false;
}

bool TryLock::unlock()
{
    if (m_autoUnlock)
    {
        return false;
    }

    return m_mutex.unlock();
}

AutoLock::AutoLock(Mutex& mutex) : m_mutex(mutex)
{
    m_mutex.lock();
}

AutoLock::~AutoLock()
{
    m_mutex.unlock();
}

}

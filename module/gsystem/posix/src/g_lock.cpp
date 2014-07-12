/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_lock.cpp
* @version     
* @brief      
* @author   duye
* @date     2013-11-26
* @note 
*
* 3. 2014-06-21 duye move to gohoop project 
* 2. 2014-01-04 duye 
* 	a. Change GMutex function GTryLock() to GTryLock()
* 	b. Add function GTryLock() for GOrgLock class
* 	c. Modify GTryLock class implimenting
* 1. 2013-11-26 duye Created this file
*/
#include <g_lock.h>

GMutex::GMutex()
{
	init(PTHREAD_MUTEX_RECURSIVE);
}

GMutex::GMutex(const GInt32 kind)
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

GMutex::~GMutex()
{
	pthread_mutex_destroy(&m_mutex);
}

bool GMutex::lock() 
{
	return pthread_mutex_lock(&m_mutex) == 0 ? true : false;
}

bool GMutex::GTryLock()
{
	return pthread_mutex_trylock(&m_mutex) == 0 ? true : false;
}

bool GMutex::unlock()
{
	return pthread_mutex_unlock(&m_mutex) == 0 ? true : false;
}

void GMutex::init(const GInt32 kind)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, kind);
	pthread_mutex_init(&m_mutex, &attr);
	pthread_mutexattr_destroy(&attr);
}

GOrgLock::GOrgLock() : m_mutex(NULL)
{
	m_mutex = new GMutex();	
}

GOrgLock::GOrgLock(const GInt32 kind) : m_mutex(NULL)
{
	m_mutex = new GMutex(kind);	
}

GOrgLock::~GOrgLock() 
{
	delete m_mutex;
}

bool GOrgLock::lock()
{
	return m_mutex->lock();
}

bool GOrgLock::GTryLock()
{
	return m_mutex->GTryLock();
}

bool GOrgLock::unlock()
{
	return m_mutex->unlock();
}

GTryLock::GTryLock(GMutex& mutex, const bool autoUnlock) 
	: m_mutex(mutex)
	, m_autoUnlock(autoUnlock)
{
}

GTryLock::~GTryLock() 
{
	if (m_autoUnlock)
	{
		m_mutex.unlock();
	}
}

bool GTryLock::lock(const GUint32 timeout)
{
	if (m_mutex.GTryLock())
	{
		return true;
	}

	if (timeout == 0)
	{
	    //G_LOG_ERROR(G_LOG_PREFIX, "timeout should > 0");
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

		if (m_mutex.GTryLock())
		{
			return true;
		}
	} while(--loops);

    //G_LOG_ERROR(G_LOG_PREFIX, "time out, and lock failed");
    
	return false;
}

bool GTryLock::unlock()
{
	if (m_autoUnlock)
	{
	    //G_LOG_WARN(G_LOG_PREFIX, "this is automation unlock");
		return false;
	}

	return m_mutex.unlock();
}

GAutoLock::GAutoLock(GMutex& mutex) : m_mutex(mutex)
{
	m_mutex.lock();
}

GAutoLock::~GAutoLock()
{
	m_mutex.unlock();
}

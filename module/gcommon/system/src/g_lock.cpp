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
* 	a. Change Mutex function TryLock() to Trylock()
* 	b. Add function Trylock() for OrgLock class
* 	c. Modify TryLock class implimenting
* 1. 2013-11-26 duye Created this file
*/
#include <g_logger.h>
#include <g_lock.h>

static const GInt8* G_LOG_PREFIX = "gohoop.gcommon.system.lock";

G_NS_GCOMMON_BEG

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

bool Mutex::trylock()
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

OrgLock::OrgLock() : m_mutex(NULL)
{
	m_mutex = new Mutex();	
}

OrgLock::OrgLock(const GInt32 kind) : m_mutex(NULL)
{
	m_mutex = new Mutex(kind);	
}

OrgLock::~OrgLock() 
{
	delete m_mutex;
}

bool OrgLock::lock()
{
	return m_mutex->lock();
}

bool OrgLock::trylock()
{
	return m_mutex->trylock();
}

bool OrgLock::unlock()
{
	return m_mutex->unlock();
}

TryLock::TryLock(Mutex& mutex, const bool autoUnlock) 
	: m_mutex(mutex)
	, m_autoUnlock(autoUnlock)
{
}

TryLock::~TryLock() 
{
	if (m_autoUnlock)
	{
		m_mutex.unlock();
	}
}

bool TryLock::lock(const GUint32 timeout)
{
	if (m_mutex.trylock())
	{
		return true;
	}

	if (timeout == 0)
	{
	    G_LOG_ERROR(G_LOG_PREFIX, "timeout should > 0, %s:%d", __FILE__, __LINE__);
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

		if (m_mutex.trylock())
		{
			return true;
		}
	} while(--loops);

    G_LOG_ERROR(G_LOG_PREFIX, "time out, and lock failed, %s:%d", __FILE__, __LINE__);
    
	return false;
}

bool TryLock::unlock()
{
	if (m_autoUnlock)
	{
	    G_LOG_WARN(G_LOG_PREFIX, "this is automation unlock, %s:%d", __FILE__, __LINE__);
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

G_NS_END

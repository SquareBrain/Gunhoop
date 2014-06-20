/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    duye_lock.cpp
* @version     
* @brief      
* @author   duye
* @date     2013-11-26
* @note 
*
* 1. 2013-11-26 duye Created this file
* 2. 2014-01-04 duye 
* 	a. Change Mutex function TryLock() to Trylock()
* 	b. Add function Trylock() for OrgLock class
* 	c. Modify TryLock class implimenting
*/

#include <duye/posix/thread/inc/duye_lock.h>

DUYE_POSIX_NS_BEG

//----------------------------class Mutex-------------------------//
Mutex::Mutex()
{
	Init(PTHREAD_MUTEX_RECURSIVE);
}

Mutex::Mutex(const D_Int32 kind)
{
	if (kind != PTHREAD_MUTEX_NORMAL && 
		kind != PTHREAD_MUTEX_RECURSIVE &&
		kind != PTHREAD_MUTEX_ERRORCHECK &&
		kind != PTHREAD_MUTEX_DEFAULT)
	{
		Init(PTHREAD_MUTEX_DEFAULT);
	}
	else
	{
		Init(kind);
	}
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&m_mutex);
}

bool Mutex::Lock() 
{
	return pthread_mutex_lock(&m_mutex) == 0 ? true : false;
}

bool Mutex::Trylock()
{
	return pthread_mutex_trylock(&m_mutex) == 0 ? true : false;
}

bool Mutex::Unlock()
{
	return pthread_mutex_unlock(&m_mutex) == 0 ? true : false;
}

void Mutex::Init(const D_Int32 kind)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, kind);
	pthread_mutex_init(&m_mutex, &attr);
	pthread_mutexattr_destroy(&attr);
}

//----------------------------class OrgLock-------------------------//
OrgLock::OrgLock() : m_mutex(NULL)
{
	m_mutex = new Mutex();	
}

OrgLock::OrgLock(const D_Int32 kind) : m_mutex(NULL)
{
	m_mutex = new Mutex(kind);	
}

OrgLock::~OrgLock() 
{
	delete m_mutex;
}

bool OrgLock::Lock()
{
	return m_mutex->Lock();
}

bool OrgLock::Trylock()
{
	return m_mutex->Trylock();
}

bool OrgLock::Unlock()
{
	return m_mutex->Unlock();
}

//----------------------------class TryLock-------------------------//
TryLock::TryLock(Mutex& mutex, const bool autoUnlock) 
	: m_mutex(mutex)
	, m_autoUnlock(autoUnlock)
{
}

TryLock::~TryLock() 
{
	if (m_autoUnlock)
	{
		m_mutex.Unlock();
	}
}

bool TryLock::Lock(const D_UInt32 timeout)
{
	if (m_mutex.Trylock())
	{
		return true;
	}

	if (timeout == 0)
	{
		return false;    
	}

    // ∫¡√Î
	const D_UInt32 sleepUnit = 10; 

	unsigned int loops = timeout / sleepUnit + 1;
						
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

		if (m_mutex.Trylock())
		{
			return true;
		}
	} while(--loops);

	return false;
}

bool TryLock::Unlock()
{
	if (m_autoUnlock)
	{
		return false;
	}

	return m_mutex.Unlock();
}

//----------------------------class TryLock-------------------------//
AutoLock::AutoLock(Mutex& mutex) : m_mutex(mutex)
{
	m_mutex.Lock();
}

AutoLock::~AutoLock()
{
	m_mutex.Unlock();
}

DUYE_POSIX_NS_END

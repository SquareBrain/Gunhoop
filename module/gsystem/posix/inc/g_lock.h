/***********************************************************************************
**  
* @copyright (c) 2010-2019, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file	    g_lock.h
* @version     
* @brief      
* @author   duye
* @date     2013-11-26
* @note 
*
* 4. 2014-06-20 duye move to gohoop project
* 3. 2014-01-09 duye Add comments
* 2. 2014-01-04 duye 
* 	a. Modify Mutex function TryLock to Trylock()
* 	b. Add function Trylock() for class OrgLock
* 	c. Modify TryLock class implimenting
*
* 1. 2013-11-26 duye Created this file
*/

#pragma once

#include <pthread.h>
#include <unistd.h>
#include <g_type.h>

/** 
 * POSIX mutex wrapper
 */
class Mutex
{
public:
    /**
     * constructor
     * @note default mutex type is PTHREAD_MUTEX_RECURSIVE
     */	
	Mutex();
	
    /**
     * constructor
     * @param [in] kind : mutex type
     * @note have four mutex type, are PTHREAD_MUTEX_NORMAL、PTHREAD_MUTEX_RECURSIVE
	 * PTHREAD_MUTEX_ERRORCHECK、PTHREAD_MUTEX_DEFAULT
     */		
	explicit Mutex(const GInt32 kind);
	~Mutex();

    /**
     * lock mutex, enter to awaited state
     * @return true/false
     * @note 
     */		
	bool lock();
	
    /**
     * try to lock mutex, failure return immediately
     * @return true/false
     * @note 
     */		
	bool trylock();

    /**
     * release lock
     * @return true/false
     * @note 
     */		
	bool unlock();

private:
    /**
     * to prevent copying
     * @note 
     */			
	Mutex(const Mutex&); 
	void operator=(const Mutex&);	

    /**
     * initialize mutex
     * @param [in] kind : mutex type, reference constructor function
     * @note 
     */		
	void init(const GInt32 kind);
    
private:
	pthread_mutex_t	m_mutex;	
};


/** 
 * original lock wrapper
 */
class OrgLock
{
public:
    /**
     * constructor
     * @note default mutex type is PTHREAD_MUTEX_RECURSIVE
     */		
	OrgLock();
	
    /**
     * constructor
     * @param [in] kind : mutex type
     * @note have four mutex type : PTHREAD_MUTEX_NORMAL、PTHREAD_MUTEX_RECURSIVE
	 * PTHREAD_MUTEX_ERRORCHECK、PTHREAD_MUTEX_DEFAULT
     */		
	explicit OrgLock(const GInt32 kind);
	virtual ~OrgLock();

    /**
     * locked and waitting
     * @return true/false
     * @note
     */			
	bool lock();
	
    /**
     * try lock, if failed return immediately
     * @return true/false
     * @note
     */		
	bool trylock();
	
    /**
     * release lock
     * @return true/false
     * @note
     */		
	bool unlock();

private:
    /**
     * prevent copying
     * @note
     */		
	OrgLock(const OrgLock&); 

    /**
     * prevent copying
     * @note
     */	
	void operator=(const OrgLock&);	

private:
	Mutex*	m_mutex;
};

/** 
 * try lock wrapper
 */
class TryLock
{
public:
    /**
     * constructor
	 * @param [in] mutex : mutex
	 * @param [in] autoUnlock : whether release lock automatic
     * @note 
     */		
	TryLock(Mutex& mutex, const bool autoUnlock = true);
	~TryLock();

    /**
     * try lock
	 * @param [in] timeout : try lock timeout, default is zero, indicate try once, 
	 * then return immediately
	 * @return true/false
     * @note 
     */		
	bool lock(const GUint32 timeout = 0);

    /**
     * release lock
	 * @return true/false
     * @note if construct a release lock, invoke invalid
     */			
	bool unlock();

private:
    /**
     * prevent copying
     * @note
     */	
	TryLock(const TryLock&); 
	
    /**
     * prevent copying
     * @note
     */		
	void operator=(const TryLock&);	
    
private:
	Mutex&	m_mutex;
	bool	m_autoUnlock;
};

/** 
 * auto lock wrapper
 */
class AutoLock
{
public:
    /**
     * constructor
	 * @param [in]mutex : mutex
     * @note 
     */			
	explicit AutoLock(Mutex& mutex);
	~AutoLock();

private:
    /**
     * prevent copying
     * @note
     */		
	AutoLock(const AutoLock&);

    /**
     * prevent copying
     * @note
     */			
	void operator=(const AutoLock&);

private:
	Mutex&      m_mutex;
};
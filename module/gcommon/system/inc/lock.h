/***********************************************************************************
**  
*    @copyright (c) 2010-2019, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
************************************************************************************/
/**
* @file	    duye_lock.h
* @version     
* @brief      
* @author   duye
* @date     2013-11-26
* @note 
*
* 3. 2014-01-09 duye Add comments
* 
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

#include <duye/posix/inc/duye_posix_def.h> 

DUYE_POSIX_NS_BEG

// brief : POSIX mutex wrapper
//	
// usage:
//	Mutex mutex;
//  mutex.Lock();
//  mutex.Trylock();
//  mutex.Unlock();
class Mutex
{
public:
	// brief :
	// @para
	// @return 
	// note : default mutex type is PTHREAD_MUTEX_RECURSIVE
	Mutex();
	
	// brief :
	// @para [in]kind : mutex type
	// @return 
	// note : have four mutex type, are PTHREAD_MUTEX_NORMAL、PTHREAD_MUTEX_RECURSIVE
	// PTHREAD_MUTEX_ERRORCHECK、PTHREAD_MUTEX_DEFAULT
	explicit Mutex(const D_Int32 kind);
	~Mutex();

	// brief : lock mutex, enter to awaited state
	// @para 
	// @return true/false
	// note:
	bool Lock();

	// brief : try to lock mutex, failure return immediately
	// @para 
	// @return true/false
	// note:	
	bool Trylock();

	// brief : release lock
	// @para 
	// @return true/false
	// note:	
	bool Unlock();

private:
	// brief : to prevent copying
	// @para 
	// @return 
	// note:
	Mutex(const Mutex&); 
	void operator=(const Mutex&);	

	// brief : initialize mutex
	// @para [in]kink : mutex type, reference constructor function
	// @return
	// note:	
	void Init(const D_Int32 kind);
    
private:
	pthread_mutex_t	m_mutex;	
};

// brief : original lock wrapper
//	
// usage:
//	OrgLock orgLock;
//  orgLock.Lock();
//  orgLock.Trylock();
//  orgLock.Unlock();
class OrgLock
{
public:
	// brief :
	// @para
	// @return 
	// note : default mutex type is PTHREAD_MUTEX_RECURSIVE
	OrgLock();
	
	// brief :
	// @para [in]kind : mutex type
	// @return 
	// note : have four mutex type : PTHREAD_MUTEX_NORMAL、PTHREAD_MUTEX_RECURSIVE
	// PTHREAD_MUTEX_ERRORCHECK、PTHREAD_MUTEX_DEFAULT	
	explicit OrgLock(const D_Int32 kind);
	virtual ~OrgLock();

	// brief : locked and waitting
	// @para 
	// @return true/false
	// note:
	bool Lock();

	// brief : try lock, if failed return immediately
	// @para 
	// @return true/false
	// note:	
	bool Trylock();

	// brief : release lock
	// @para  
	// @return true/false
	// note:		
	bool Unlock();

private:
	// brief : prevent copying
	// @para 
	// @return 
	// note:
	OrgLock(const OrgLock&); 
	void operator=(const OrgLock&);	

private:
	Mutex*	m_mutex;
};

// brief : try lock wrapper
//	
// usage:
//  Mutex mutex;
//
//  void MyFun()
//  {
//      // auto release lock
//	    TryLock autoTryLock(mutex);
//      if (!autoTryLock.Lock(1000))
//      {
//          return;
//      }
//      
//      // to do
//
//      // not auto release lock
//      TryLock notAutoTryLock(mutex, false);
//      if (!autoTryLock.Lock(1000))
//      {
//          notAutoTryLock.Unlock();
//          return;
//      }     
//      
//      // to do
//      
//      notAutoTryLock.Unlock();
//  }
class TryLock
{
public:
	// brief :
	// @para [in]mutex : mutex
	// @para [in]autoUnlock : whether release lock automatic
	// @return 
	// note
	TryLock(Mutex& mutex, const bool autoUnlock = true);
	
	~TryLock();

	// brief : try lock 
	// @para [in]timeout : try lock timeout, default is zero, indicate try once, 
	// then return immediately
	// @return true/false
	// note
	bool Lock(const D_UInt32 timeout = 0);

	// brief : release lock
	// @para
	// @return true/false
	// note : if construct a release lock, invoke invalid
	bool Unlock();

private:
	// brief : prevent copying
	// @para 
	// @return 
	// note
	TryLock(const TryLock&); 
	void operator=(const TryLock&);	
    
private:
	Mutex&	m_mutex;
	bool	m_autoUnlock;
};

// brief : auto lock wrapper
//	
// usage :
//  Mutex mutex;
//
//  void MyFun()
//  {
//      // auto release lock
//	    AutoLock autoLock(mutex);
//
//      // to do
//      // auto release lock, when return the function
//  }
class AutoLock
{
public:
	// brief :
	// @para [in]mutex : mutex
	// @return 
	// note:
	explicit AutoLock(Mutex& mutex);
	~AutoLock();

private:
	// brief : prevent copying
	// @para 
	// @return 
	// note
	AutoLock(const AutoLock&);
	void operator=(const AutoLock&);

private:
	Mutex& m_mutex;
};

DUYE_POSIX_NS_END

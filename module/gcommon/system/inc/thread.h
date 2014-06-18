/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    duye_thread.h
* @version     
* @brief      
* @author   duye
* @date     2013-11-26
* @note 
*
* 2. 2014-01-12 duye Add comments   
*
* 1. 2013-11-26 duye Created this file
* 
*/

#pragma once

#include <pthread.h> 

#include <duye/posix/inc/duye_posix_def.h>

DUYE_POSIX_NS_BEG

// brief : thread state
enum ThreadState
{
	// running state
	THR_STATE_RUN = 0,
	// stoped state
	THR_STATE_STOP,
	// exit state
	THR_STATE_EXIT
};

// brief : the pointer of thread enter
typedef void* (*ThreadFunPoint_t)(void*);

// brief : can be inherited ty user
//	
// usage:
//	class MyThread : public Runnable
//  {
//  public:
//      MyThread() {}
//      virtual ~MyThread() {}
//  
//      virtual void Run()
//      {
//          // my thread
//          for (;;) {}
//      }
//  }
class Runnable
{
public:
	virtual ~Runnable() {}
	virtual void Run() = 0;
};

// brief : POSIX thread wrapper
//	
// usage:
//	class MyThread : public Runnable
//  {
//  public:
//      MyThread() {}
//      virtual ~MyThread() {}
//  
//      virtual void Run()
//      {
//          // my thread
//          for (;;) {}
//      }
//  }
//  
//  MyThread myThread;
//  Thread newThread(&myThread);
//  newThread.Start();
class Thread
{
public:
    explicit Thread(Runnable* target, const bool autoRel = true);
    ~Thread();
	
	// brief : startup thread
	// @para
	// return true/false
	bool Start();

	// brief : get thread ID
	// @para
	// return : thread ID
	pthread_t GetThreadId() const;

	// brief : create a new thread
	// @para [in]entry : the entry function for thread 
	// @para [in]argument : thread argument
	// @para [in]autoRel : whether detached with main thread, default is ture, 
	// indicate detached with main thread
	// return : thread ID
	// usage :
	//  void MyEntryFun(void* argument)
	//	{
	//	    for (;;) {}
	//  }
	//		
	//	void* argument;
	//	Thread::CreateThread(MyEntryFun, argument);
	static pthread_t CreateThread(void* entry, void* argument, const bool autoRel = true);

private:
	// brief : prevent copying
	Thread(const Thread&);
	void operator=(const Thread&);
	
	// brief : thread entry
	// @para [in]argument : thread argument
	// return : reutrn description
	static void* EnterPoint(void* argument);

private:
	// thread ID
	pthread_t	m_threadId;
	// indicate whether is detached with main thread£¬default is detached
	bool		m_autoRel;
	// user thread object
	Runnable*	m_runnable;
};

// brief : thread base class, be inherited by user
// usage :
//	class MyThreadTask : public ThreadTask
//	{
//	public:
//		MyThreadTask() { this->Start(); }
//		virtual ~MyThreadTask() {}
//
//	private:
//		virtual Int8_t* Run()
//		{
//			// thread loop
//			for (;;) {}
//			return NULL;	
//		}
//	};
class ThreadTask
{
public:
	// brief : 
	// @para [in]autoRel : whether is detached with main thread, default is detached
	// note
	explicit ThreadTask(const bool autoRel = true);
	virtual ~ThreadTask();

	// brief : startup thread
	// @para
	// return true/false
	bool Start();

	// brief : thread entry function
	// @para
	// note
	virtual void Run() = 0;

private:
	// brief : prevate copying
	ThreadTask(const ThreadTask&);
	void operator=(const ThreadTask&);
	
	// brief : inner used for starting thread
	// @para [in]argument : thread argument
	// return : thread return description
	static void* EnterPoint(void* argument);	

private:
	// thread ID
	pthread_t	m_threadId;	
	// whether is detached with main thread, default is ture, 
	// indicate detached with main thread
	bool		m_autoRel;
};

DUYE_POSIX_NS_END

/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_threadpool.h
* @version     
* @brief      
* @author   duye
* @date     2013-12-10
* @note 
*
*  2. 2014-06-21 duye move to gohoop project 
*  1. 2013-12-10 duye Created this file
* 
*/

#pragma once

#include <list>
#include <g_thread.h>
#include <g_condition.h>

G_NS_GCOMMON_BEG

// default the count of thread pool
static const GUint32 G_DEF_THREAD_COUNT = 20;

class ThreadWorker;
class ThreadJob;

// brief : thread pool implemention
// usage :
//  class MyJob : public ThreadJob
//  {
//  public:
//      MyJob() {}
//      virtual MyJob() {}
//  
//      virtual void Work(void* userData)
//      {
//          // TODO    
//      }
//  }
//  
//  int threadPoolSize = 90;
//  ThreadPool myThreadPool(threadPoolSize);
//
//  MyJob myJob;
//  void* userData = NULL;
//  myThreadPool.DoJob(&myJob, userData))
class ThreadPool
{
public:
    // thread worker queue
	typedef std::list<ThreadWorker*> ThreadWorkerList;

public:
    // brief : 
    // @para [in]threadCount : the size of thread pool
    // note
	explicit ThreadPool(const GUint32 threadCount = G_DEF_THREAD_COUNT);
	~ThreadPool();

    // brief : start to do user job
    // @para [in]threadJob : user job object
    // @para [in]userData : user data
    // return : true/false
    // note
	bool DoJob(ThreadJob* threadJob, void* userData);

	// brief : get thread count
	// @para
	// return : the count of thread pool
	// note
	GUint32 GetThreadCount() const;

private:
    // brief : prevent copying
    ThreadPool(const ThreadPool&);
    void operator=(const ThreadPool&);

    // brief : initialize thread pool
	void InitThreadPool();

	// brief : uninitialize thread pool
	void UninitThreadPool();

	// brief : move idle thread worker from busy queue to idle queue
	// @para [in]workerId : thread worker ID
	// note
	void MoveToIdleList(const GUint32 workerId);

	// brief : move busy thread worker from idle queue to busy queue
	// @para [in]workerId : thread worker ID
	// note
	void MoveToBusyList(const GUint32 workerId);

private:
    // thread pool size
	GUint32				m_threadCount;
	// the idle queue
	ThreadWorkerList	m_idleThreadWorkerList;
	// the busy queue
	ThreadWorkerList	m_busyThreadWorkerList;
};

// brief : thread worker, class ThreadPool depend on 
// usage:
//  void MyTestFun()
//  {
//      ThreadWorker threadWork;
//  
//      class MyJob : public ThreadJob {};
//      ThreadJob* threadJob = new MyJob;
//      void* userData;
//      threadWork.DoWork(threadJob, userData);
//  }
class ThreadWorker : public ThreadTask
{
public:
    // brief : 
    // @para [in]workerId : setting worker ID
    // note
	explicit ThreadWorker(const GUint32 workerId);
	virtual ~ThreadWorker();

    // brief : get thread worker ID
    // return : thread worker ID
	GUint32 GetWorkerId() const;

	// brief : to do work
	// @para [in] threadJob : user thread job
	// @para [in] userData : user data
	// return : true/false
	// note
	bool DoWork(ThreadJob* threadJob, void* userData);		

private:
    // brief : prevent copying
	ThreadWorker(const ThreadWorker&);
	void operator=(const ThreadWorker&);
	
	// brief : thread run fucntion, loop 
	void Run();

private:
    // thread worker ID, setting by external
	GUint32			m_workerId;
	// waitting condition
	Condition		m_condition;
	// current running job
	ThreadJob*		m_threadJob;	
	// current running user data
	void*			m_userData;	
};

// brief : thread job, be inherited by user
// usage:
//  class UserJob : public ThreaJob
//  {
//  public:
//      UserJob() {}
//      virtual UserJob() {}
//      
//      virtual void Work(void* userData)
//      {
//          // do user work
//      }
//  }
class ThreadJob
{
public:
	~ThreadJob();
	virtual void Work(void* userData) = 0;
};

G_NS_GCOMMON_END

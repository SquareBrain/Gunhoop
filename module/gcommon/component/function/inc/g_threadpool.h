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
#include <g_type.h>
#include <g_thread.h>
#include <g_condition.h>

G_NS_GCOMMON_BEG

/**
 * thread job, be inherited by user
 * usage:
 *  class UserJob : public ThreaJob
 *  {
 *  public:
 *      UserJob() {}
 *      virtual UserJob() {}
 *      
 *      virtual void Work(void* userData)
 *      {
 *          // do user work
 *      }
 *  }
 */
interface ThreadJob
{
public:
	~ThreadJob();

    /**
     * do user work
     * @param [in] userData : user data
     */  	
	virtual void work(void* userData) = 0;
};

class ThreadWorker;

/** 
 * thread pool implemention
 */
class ThreadPool
{
public:
    /**
     * thread worker queue
     */      
	typedef std::list<ThreadWorker*> ThreadWorkerList;

public:
    ThreadPool();

    /**
     * constructor
     * @param [in] threadCount : thread pool count
     */    
	explicit ThreadPool(const GUint32 threadCount);
	~ThreadPool();

    /**
     * start to do user job
     * @param [in] threadJob : user job object
     * @param [in] userData : user data
     * @return G_YES/G_NO
     */       
	GResult doJob(ThreadJob* threadJob, void* userData);

    /**
     * get thread count
     * @param [in] threadJob : user job object
     * @param [in] userData : user data
     * @return the count of thread pool
     */ 	
	GUint32 getThreadCount() const;

private:
    /**
     * prevent copying
     */     
    ThreadPool(const ThreadPool&);
    void operator=(const ThreadPool&);

    /**
     * initialize thread pool
     */       
	void initThreadPool();

    /**
     * uninitialize thread pool
     */  	
	void uninitThreadPool();

    /**
     * move idle thread worker from busy queue to idle queue
     * @param [in] workerId : thread worker ID
     */ 	
	void moveToIdleList(const GUint32 workerId);

    /**
     * move busy thread worker from idle queue to busy queue
     * @param [in]workerId : thread worker ID
     */ 	
	void moveToBusyList(const GUint32 workerId);

private:
    // thread pool size
	GUint32				m_threadCount;
	// the idle queue
	ThreadWorkerList	m_idleThreadWorkerList;
	// the busy queue
	ThreadWorkerList	m_busyThreadWorkerList;
};

/** 
 * thread worker, class ThreadPool depend on 
 * @usage:
 *  void MyTestFun()
 *  {
 *      ThreadWorker threadWork;
 *  
 *      class MyJob : public ThreadJob {};
 *      ThreadJob* threadJob = new MyJob;
 *      void* userData;
 *      threadWork.DoWork(threadJob, userData);
 *  }
 */
class ThreadWorker : public ThreadTask
{
public:
    /**
     * constructor
     * @param [in] workerId : worker ID
     */       
	explicit ThreadWorker(const GUint32 workerId);
	virtual ~ThreadWorker();

    /**
     * get thread worker ID
     * @param [in] workerId : worker ID
     * @return thread worker ID
     */     
	GUint32 getWorkerId() const;

    /**
     * to do work
	 * @param [in] threadJob : user thread job
	 * @param [in] userData : user data
	 * @return G_YES/G_NO
     */  	
	GResult doWork(ThreadJob* threadJob, void* userData);		

private:
    /**
     * prevent copying
     */     
	ThreadWorker(const ThreadWorker&);
	void operator=(const ThreadWorker&);
	
    /**
     * thread run fucntion, loop 
     */	
	void run();

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

G_NS_END

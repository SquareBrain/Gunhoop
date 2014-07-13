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
#include <g_system.h>
#include <g_namespace.h>
#include <g_thread.h>
#include <g_condition.h>

G_NS_GCOMMON_BEG

/**
 * @brief thread job, be inherited by user
 */
class ThreadJob
{
public:
	~ThreadJob() {}

    /**
     * @brief do user work
     * @param [in] userData : user data
     */  	
	virtual void work(void* userData) = 0;
};

class ThreadWorker;

/** 
 * @brief thread pool implemention
 */
class ThreadPool
{
public:
    /**
     * @brief thread worker queue
     */      
	typedef std::list<ThreadWorker*> ThreadWorkerList;

public:
    ThreadPool();

    /**
     * @brief constructor
     * @param [in] threadCount : thread pool count
     */    
	explicit ThreadPool(const GUint32 threadCount);
	~ThreadPool();

    /**
     * @brief start to do user job
     * @param [in] threadJob : user job object
     * @param [in] userData : user data
     * @return G_YES/G_NO
     */       
	GResult doJob(ThreadJob* threadJob, void* userData);

    /**
     * @brief get thread count
     * @param [in] threadJob : user job object
     * @param [in] userData : user data
     * @return the count of thread pool
     */ 	
	GUint32 getThreadCount() const;

private:
    /**
     * @brief prevent copying
     */     
    ThreadPool(const ThreadPool&);
    void operator=(const ThreadPool&);

    /**
     * @brief initialize thread pool
     * @return G_YES/G_NO
     */       
	GResult initThreadPool();

    /**
     * @brief uninitialize thread pool
     * @return G_YES/G_NO
     */  	
	GResult uninitThreadPool();

    /**
     * @brief move idle thread worker from busy queue to idle queue
     * @param [in] workerId : thread worker ID
     */ 	
	void moveToIdleList(const GUint32 workerId);

    /**
     * @brief move busy thread worker from idle queue to busy queue
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
 * @brief thread worker, class ThreadPool depend on 
 */
class ThreadWorker : public GThreadTask
{
public:
    /**
     * @brief constructor
     * @param [in] workerId : worker ID
     */       
	explicit ThreadWorker(const GUint32 workerId);
	virtual ~ThreadWorker();

    /**
     * @brief get thread worker ID
     * @param [in] workerId : worker ID
     * @return thread worker ID
     */     
	GUint32 getWorkerId() const;

    /**
     * @brief to do work
	 * @param [in] threadJob : user thread job
	 * @param [in] userData : user data
	 * @return G_YES/G_NO
     */  	
	GResult doWork(ThreadJob* threadJob, void* userData);		

private:
    /**
     * @brief prevent copying
     */     
	ThreadWorker(const ThreadWorker&);
	void operator=(const ThreadWorker&);
	
    /**
     * @brief thread entry function
     * @return G_YES/G_NO
     * @note 
     */	
	GResult run();

private:
    // thread worker ID, setting by external
	GUint32			m_workerId;
	// waitting condition
	GCondition		m_condition;
	// current running job
	ThreadJob*		m_threadJob;	
	// current running user data
	void*			m_userData;	
};

G_NS_END

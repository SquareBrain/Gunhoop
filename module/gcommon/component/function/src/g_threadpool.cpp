/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_threadpool.cpp
* @version     
* @brief      
* @author   duye
* @date     2013-12-10
* @note 
*
* 2. 2014-01-12 duye Modify implemention    
* 1. 2013-12-10 duye Created this file
* 
*/

#include <g_threadpool.h> 

static const GInt8* LOG_PREFIX = "gohoop.gcommon.component.threadpool";

// default the count of thread pool
static const GUint32 G_DEF_THREAD_COUNT = 20;

G_NS_GCOMMON_BEG

ThreadPool::ThreadPool() : m_threadCount(G_DEF_THREAD_COUNT)
{
    InitThreadPool();    
}

ThreadPool::ThreadPool(const GUint32 threadCount) : m_threadCount(threadCount)
{
    InitThreadPool();    
}

ThreadPool::~ThreadPool()
{
}

GResult ThreadPool::doJob(ThreadJob* threadJob, void* userData)
{
	return G_YES;
}

GUint32 ThreadPool::getThreadCount() const
{
	return m_threadCount;
}

GResult ThreadPool::initThreadPool()
{
	for (GUint32 i = 0; i < m_threadCount; i++)
	{
		m_idleThreadWorkerList.push_back(new ThreadWorker(i));		
	}

	return G_YES;
}

GResult ThreadPool::uninitThreadPool()
{
    return G_YES;    
}

ThreadWorker::ThreadWorker(const GUint32 workerId) 
	: m_workerId(workerId)
	, m_threadJob(NULL)
{
	this->start();
}

ThreadWorker::~ThreadWorker()
{
}

GUint32 ThreadWorker::getWorkerId() const
{
	return m_workerId;
}

GResult ThreadWorker::doWork(ThreadJob* threadJob, void* userData)
{
	m_threadJob = threadJob;
	m_userData = userData;

	return G_YES;
}

void ThreadWorker::run()
{
	for (;;)
	{
		m_condition.Wait();
		if (NULL == m_threadJob)	
		{
			continue;
		}

		m_threadJob->Work(m_userData);
		m_threadJob = NULL;
		m_userData = NULL;
	}
}

G_NS_END

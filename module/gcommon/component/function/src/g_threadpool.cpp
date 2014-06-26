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

G_NS_GCOMMON_BEG

ThreadPool::ThreadPool(const GUint32 threadCount) : m_threadCount(threadCount)
{
    InitThreadPool();    
}

ThreadPool::~ThreadPool()
{
    
}

bool ThreadPool::DoJob(ThreadJob* threadJob, void* userData)
{
	return true;
}

GUint32 ThreadPool::GetThreadCount() const
{
	return m_threadCount;
}

void ThreadPool::InitThreadPool()
{
	for (GUint32 i = 0; i < m_threadCount; i++)
	{
		m_idleThreadWorkerList.push_back(new ThreadWorker(i));		
	}
}

void ThreadPool::UninitThreadPool()
{
    
}

ThreadWorker::ThreadWorker(const GUint32 workerId) 
	: m_workerId(workerId)
	, m_threadJob(NULL)
{
	this->Start();
}

ThreadWorker::~ThreadWorker()
{
}

GUint32 ThreadWorker::GetWorkerId() const
{
	return m_workerId;
}

bool ThreadWorker::DoWork(ThreadJob* threadJob, void* userData)
{
	m_threadJob = threadJob;
	m_userData = userData;

	return true;
}

void ThreadWorker::Run()
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

/************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    duye_thread_pool.cpp
* @version     
* @brief      
* @author   duye
* @date     2013-12-10
* @note 
*
* 2. 2014-01-12 duye Modify implemention    
*
* 1. 2013-12-10 duye Created this file
* 
*/

#include <duye/posix/thread/inc/duye_thread_pool.h> 

DUYE_POSIX_NS_BEG

//---------------------------class ThreadPool----------------------//
ThreadPool::ThreadPool(const D_UInt32 threadCount) : m_threadCount(threadCount)
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

D_UInt32 ThreadPool::GetThreadCount() const
{
	return m_threadCount;
}

void ThreadPool::InitThreadPool()
{
	for (D_UInt32 i = 0; i < m_threadCount; i++)
	{
		m_idleThreadWorkerList.push_back(new ThreadWorker(i));		
	}
}

void ThreadPool::UninitThreadPool()
{
    
}

/*---------------------------ThreadWorker class----------------------*/
ThreadWorker::ThreadWorker(const D_UInt32 workerId) 
	: m_workerId(workerId)
	, m_threadJob(NULL)
{
	this->Start();
}

ThreadWorker::~ThreadWorker()
{
}

D_UInt32 ThreadWorker::GetWorkerId() const
{
	return m_workerId;
}

D_Bool ThreadWorker::DoWork(ThreadJob* threadJob, void* userData)
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

DUYE_POSIX_NS_END

/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_thread.cpp
* @version     
* @brief      
* @author   duye
* @date     2013-11-26
* @note 
*
*  1. 2013-11-26 duye Created this file
* 
*/

#include <g_thread.h> 

static const GInt8* LOG_PREFIX = "gohoop.gcommon.system.thread";

G_NS_GCOMMON_BEG

Thread::Thread(Runnable* runnable, const bool autoRel) 
	: m_threadId(-1)
	, m_autoRel(autoRel)
	, m_runnable(runnable)
{
}

Thread::~Thread()
{
}

bool Thread::start()
{
	pthread_attr_t* attributes = NULL;
	
	GInt32 ret = pthread_create(&m_threadId, attributes, EnterPoint, m_runnable);

	if (ret != 0)
	{
		return false;
	}

	if (m_autoRel)
	{
		pthread_detach(m_threadId);
	}

	return true;
}

GUint32 Thread::getThreadId() const
{
	return (GUint32)m_threadId;
}

void* Thread::enterPoint(void* argument)
{
	Runnable* runnable = static_cast<Runnable*>(argument);

    runnable->Run();

	return NULL;
}

ThreadTask::ThreadTask(const bool autoRel) : m_threadId(-1), m_autoRel(autoRel)
{
}

ThreadTask::~ThreadTask()
{
}

bool ThreadTask::start()
{
	pthread_attr_t* attributes = NULL;
	
	GInt32 ret = pthread_create(&m_threadId, attributes, EnterPoint, this);

	if (ret != 0)
	{
		return false;
	}

	if (m_autoRel)
	{
		pthread_detach(m_threadId);
	}

	return true;
}

void* ThreadTask::enterPoint(void* argument)
{
	ThreadTask* threadTask = static_cast<ThreadTask*>(argument);

    threadTask->Run();

	return NULL;
}

GInt32 ThreadUtil::createThread(void* entry, void* argument, const bool autoRel)
{
	pthread_attr_t* attributes = NULL;

	pthread_t threadId = -1;

	GInt32 ret = pthread_create(&threadId, attributes, (ThreadFunPoint_t)entry, argument);
	if (ret != 0)
	{
		return (GInt32)threadId;
	}

	if (autoRel)
	{
		pthread_detach(threadId);
	}

	return (GInt32)threadId;
}

G_NS_END

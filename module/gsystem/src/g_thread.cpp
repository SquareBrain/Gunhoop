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
* @date	    2013-11-26
* @note 
*
*  1. 2013-11-26 duye Created this file
* 
*/
#include <g_thread.h> 

using namespace gsys;

Thread::Thread(Runnable* runnable, const bool autoRel) 
    : m_threadId(-1)
    , m_autoRel(autoRel)
    , m_runnable(runnable)
{
}

Thread::~Thread()
{
}

GResult Thread::start()
{
    pthread_attr_t* attributes = NULL;
    GResult ret = pthread_create(&m_threadId, attributes, enterPoint, m_runnable);
    if (ret != 0)
    {
    	//G_LOG_ERROR(G_LOG_PREFIX, "call pGThread_create() failed");
    	return G_NO;
    }

    if (m_autoRel)
    {
    	pthread_detach(m_threadId);
    }

    return G_YES;
}

GUint32 Thread::getGThreadId() const
{
    return (GUint32)m_threadId;
}

void* Thread::enterPoint(void* argument)
{
    Runnable* runnable = static_cast<Runnable*>(argument);
    runnable->run();
    return NULL;
}

ThreadTask::ThreadTask(const bool autoRel) : m_threadId(-1), m_autoRel(autoRel) {}

ThreadTask::~ThreadTask() {}

GResult ThreadTask::startTask()
{
    pthread_attr_t* attributes = NULL;
    GInt32 ret = pthread_create(&m_threadId, attributes, enterPoint, this);
    if (ret != 0)
    {
    	//G_LOG_ERROR(G_LOG_PREFIX, "call pGThread_create() failed");
    	return G_NO;
    }

    if (m_autoRel)
    {
    	pthread_detach(m_threadId);
    }

    return G_YES;
}

void* ThreadTask::enterPoint(void* argument)
{
    ThreadTask* threadTask = static_cast<ThreadTask*>(argument);
    threadTask->run();
    return NULL;
}

GInt32 ThreadUtil::createThread(void* entry, void* argument, const bool autoRel)
{
    pthread_attr_t* attributes = NULL;
    pthread_t threadId = -1;

    GInt32 ret = pthread_create(&threadId, attributes, (GThreadFunPoint_t)entry, argument);
    if (ret != 0)
    {
    	//G_LOG_ERROR(G_LOG_PREFIX, "call pGThread_create() failed");
    	return (GInt32)threadId;
    }

    if (autoRel)
    {
    	pthread_detach(threadId);
    }

    return (GInt32)threadId;
}
}

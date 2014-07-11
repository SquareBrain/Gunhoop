/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_GThread.cpp
* @version     
* @brief      
* @author   duye
* @date     2013-11-26
* @note 
*
*  1. 2013-11-26 duye Created this file
* 
*/
#include <g_GThread.h> 

GThread::GThread(Runnable* runnable, const bool autoRel) 
	: m_GThreadId(-1)
	, m_autoRel(autoRel)
	, m_runnable(runnable)
{
}

GThread::~GThread()
{
}

GResult GThread::start()
{
	pGThread_attr_t* attributes = NULL;
	GResult ret = pGThread_create(&m_GThreadId, attributes, enterPoint, m_runnable);
	if (ret != 0)
	{
	    //G_LOG_ERROR(G_LOG_PREFIX, "call pGThread_create() failed");
		return G_NO;
	}

	if (m_autoRel)
	{
		pGThread_detach(m_GThreadId);
	}

	return G_YES;
}

GUint32 GThread::getGThreadId() const
{
	return (GUint32)m_GThreadId;
}

void* GThread::enterPoint(void* argument)
{
	Runnable* runnable = static_cast<Runnable*>(argument);
    runnable->run();
	return NULL;
}

GThreadTask::GThreadTask(const bool autoRel) : m_GThreadId(-1), m_autoRel(autoRel)
{
}

GThreadTask::~GThreadTask()
{
}

GResult GThreadTask::start()
{
	pGThread_attr_t* attributes = NULL;
	GInt32 ret = pGThread_create(&m_GThreadId, attributes, enterPoint, this);
	if (ret != 0)
	{
	    //G_LOG_ERROR(G_LOG_PREFIX, "call pGThread_create() failed");
		return G_NO;
	}

	if (m_autoRel)
	{
		pGThread_detach(m_GThreadId);
	}

	return G_YES;
}

void* GThreadTask::enterPoint(void* argument)
{
	GThreadTask* GThreadTask = static_cast<GThreadTask*>(argument);
    GThreadTask->run();
	return NULL;
}

GInt32 GThreadUtil::createGThread(void* entry, void* argument, const bool autoRel)
{
	pGThread_attr_t* attributes = NULL;
	pGThread_t GThreadId = -1;

	GInt32 ret = pGThread_create(&GThreadId, attributes, (GThreadFunPoint_t)entry, argument);
	if (ret != 0)
	{
	    //G_LOG_ERROR(G_LOG_PREFIX, "call pGThread_create() failed");
		return (GInt32)GThreadId;
	}

	if (autoRel)
	{
		pGThread_detach(GThreadId);
	}

	return (GInt32)GThreadId;
}

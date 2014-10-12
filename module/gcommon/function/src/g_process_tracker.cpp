/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_process_tracker.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-10-11
* @note 
*
*  1. 2014-10-11 duye Created this file
* 
*/
#include <signal.h>
#include <g_process_tracker.h>

namespace gcom {

ProcessTracker::ProcessTracker()
{
    m_processSysCallback.registObserver(this);
}

ProcessTracker::~ProcessTracker()
{
    m_observerList.clear();
}

void ProcessTracker::addObserver(ProcessObserver* observer)
{
    IS_NULL_R(observer);
    gsys::AutoLock auto_lock(m_observerList.mutex()); 
    ObserverList::iterator iter = m_observerList.begin();
    for (; iter != m_observerList.end(); ++iter)
    {
        if (*iter == observer)
        {
            return;
        }
    }   
    
    m_observerList.push_back(observer);
}

void ProcessTracker::removeObserver(ProcessObserver* observer)
{
    IS_NULL_R(observer);
    gsys::AutoLock auto_lock(m_observerList.mutex());
    m_observerList.remove(observer);
}

void ProcessTracker::wait(const GUint32 timeout)
{
    m_exitCondition.wait(timeout);
}

void ProcessTracker::wakeup()
{
    m_exitCondition.broadcast();
}
    
void ProcessTracker::signalHandler(const GInt32 sig)
{
    gsys::AutoLock autoLock(m_observerList.mutex());
    ObserverList::iterator iter = m_observerList.begin();
    for (; iter != m_observerList.end(); ++iter)
    {
    	switch (sig)
        {
    	    case gsys::G_SIGSEGV:
    	    	(*iter)->onSegmentationFault();
    	    	break;
    	    case gsys::G_SIGINT:
    	    	(*iter)->onCtrlC();
    	     	break;
    	    default:
    	    	(*iter)->onOther(sig);
    	    	break;
        }
    }    
}

}

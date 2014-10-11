/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_process.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-09-28
* @note 
*
*  1. 2014-09-28 duye Created this file
* 
*/
#include <signal.h>
#include <g_process.h>

namespace gsys {

ProcessSysCallback::ProcessSysCallback()
{
    // segmentation fault
    signal(SIGSEGV, signalHandlerCallback);
	
    // ctrl + c
    signal(SIGINT, signalHandlerCallback);
}

ProcessMonitor* ProcessSysCallback::m_processMonitor = nullptr;

ProcessSysCallback::~ProcessSysCallback() {}

void ProcessSysCallback::registProcessMonitor(ProcessMonitor* process_monitor)
{
    m_processMonitor = process_monitor;
}

void ProcessSysCallback::signalHandlerCallback(const GInt32 sig)
{
	IS_NULL_R(m_processMonitor);
    m_processMonitor->signalHandler(sig);
}

ProcessMonitor::ProcessMonitor()
{
    m_processSysCallback.registProcessMonitor(this);
}

ProcessMonitor::~ProcessMonitor()
{
    m_observerList.clear();
}

ProcessMonitor& ProcessMonitor::getInstance()
{
    static ProcessMonitor process_monitor;
    return process_monitor;
}

void ProcessMonitor::addObserver(ProcessMonitorObserver* observer)
{
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

void ProcessMonitor::removeObserver(ProcessMonitorObserver* observer)
{
    gsys::AutoLock autoLock(m_observerList.mutex());
    ObserverList::iterator iter = m_observerList.begin();
    for (; iter != m_observerList.end(); ++iter)
    {
        if (*iter == observer)
        {
            m_observerList.erase(iter);
            break;
        }
    }    
}

void ProcessMonitor::signalHandler(const GInt32 sig)
{
    gsys::AutoLock autoLock(m_observerList.mutex());
    ObserverList::iterator iter = m_observerList.begin();
    for (; iter != m_observerList.end(); ++iter)
    {
        (*iter)->onSegmentationFault(sig);
        (*iter)->onCtrlC(sig);
    }    
}

}

/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_process.cpp
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
	if (m_processMonitor != nullptr)
	{
		m_processMonitor->signalHandler(sig);
	}
}

ProcessMonitor::ProcessMonitor()
{
	m_processSysCallback.registProcessMonitor(this);
}

ProcessMonitor::~ProcessMonitor()
{
    m_processMonitorList.clear();
}

void ProcessMonitor::addMonitor(ProcessMonitorInterface* monitor)
{
    m_processMonitorList.push_back(monitor);
}

void ProcessMonitor::removeMonitor(ProcessMonitorInterface* monitor)
{
    ProcessMonitorList::iterator iter = m_processMonitorList.begin();
    for (; iter != m_processMonitorList.end(); ++iter)
    {
        if (monitor == *iter)
        {
            m_processMonitorList.erase(iter);
            break;
        }
    }    
}

void ProcessMonitor::signalHandler(const GInt32 sig)
{
    ProcessMonitorList::iterator iter = m_processMonitorList.begin();
    for (; iter != m_processMonitorList.end(); ++iter)
    {
        (*iter)->onSegmentationFault(sig);
        (*iter)->onCtrlC(sig);
    }    
}

}

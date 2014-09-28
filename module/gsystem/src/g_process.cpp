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

ProcessMonitor::ProcessMonitor()
{
    // segmentation fault
    signal(SIGSEGV, signalHandler);
    // Ctrl + c
    signal(SIGINT, signalHandler);
}

ProcessMonitor::~ProcessMonitor()
{
    m_monitorList.clear();
}

void ProcessMonitor::addMonitor(ProcessMonitorInterface* monitor)
{
    m_monitorList.push_back(monitor);
}

void ProcessMonitor::signalHandler(const GInt32 sig)
{
    MonitorList::iterator iter = m_monitorList.begin();
    for (; iter != m_monitorList.end(); ++iter)
    {
        (*iter)->onSegmentationFault(sig);
        (*iter)->onCtrlC(sig);
    }    
}

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

Process::Process() : m_id(-1) {}
~Process::Process() {}

void Process::setId(const GInt32 id)
{
	m_id = id;
}

GInt32 Process::id() 
{
	return m_id;
}

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

}

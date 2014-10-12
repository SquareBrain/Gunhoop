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

ProcessSysCallback::ProcessSysCallback() : m_observer(nullptr)
{
    // segmentation fault
    signal(SIGSEGV, signalHandlerCallback);
	
    // ctrl + c
    signal(SIGINT, signalHandlerCallback);
}

ProcessSysCallback::~ProcessSysCallback() {}

void ProcessSysCallback::registProcessMonitor(ProcessMonitor* observer)
{
    m_observer = observer;
}

void ProcessSysCallback::signalHandlerCallback(const GInt32 sig)
{
    IS_NULL_R(m_observer);
    m_observer->onSignalHandler(sig);
}

}

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

ProcessSysCallbackObserver* ProcessSysCallback::m_observer = nullptr;

ProcessSysCallback::ProcessSysCallback()
{
    // segmentation fault
    signal(SIGSEGV, ProcessSysCallback::signalHandler);
	
    // ctrl + c
    signal(SIGINT, ProcessSysCallback::signalHandler);
}

ProcessSysCallback::~ProcessSysCallback() {}

void ProcessSysCallback::registObserver(ProcessSysCallbackObserver* observer)
{
    m_observer = observer;
}

void ProcessSysCallback::signalHandler(const GInt32 sig)
{
    IS_NULL_R(m_observer);
    m_observer->onSignalHandler(sig);
}

}

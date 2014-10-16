/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_sys_process.cpp
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
#include <unistd.h>
#include <sys/resource.h>
#include <g_sys_process.h>

namespace gsys {

static const GUint32 G_DEF_LIMIT_FILE = 1024;

ProcessSysCallbackObserver* ProcessSysCallback::m_observer = nullptr;

SysProcess::SysProcess()
{
    setLimitFile(G_DEF_LIMIT_FILE);   	
}

SysProcess::~SysProcess() {}

GResult SysProcess::setLimitFile(const GUint32 limit_file)
{
    m_limitFile = limit_file;
	
    struct rlimit rt;
    rt.rlim_max = rt.rlim_cur = m_limitFile;
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1) 
    {
        return G_NO;
    }    	
    
    return G_YES;
}

GUint32 SysProcess::limitFile() const
{
    return m_limitFile;	
}

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

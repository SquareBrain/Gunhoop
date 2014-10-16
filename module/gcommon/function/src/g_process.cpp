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
* @date     2014-10-11
* @note 
*
*  1. 2014-10-11 duye Created this file
* 
*/
#include <g_process.h>

namespace gcom {

Process::Process()
{
    m_processSysCallback.registObserver(this);
}

Process::~Process()
{
    m_observerList.clear();
}

void Process::addObserver(ProcessObserver* observer)
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

void Process::removeObserver(ProcessObserver* observer)
{
    IS_NULL_R(observer);
    gsys::AutoLock auto_lock(m_observerList.mutex());
    m_observerList.remove(observer);
}

void Process::setLimitFile(const GUint32 num)
{
    m_sysProcess.setLimitFile(num);   
}

GUint32 Process::limitFile() const
{
    return m_sysProcess.limitFile();
}

void Process::wait(const GUint32 timeout)
{
    m_exitCondition.wait(timeout);
}

void Process::wakeup()
{
    m_exitCondition.broadcast();
}
    
void Process::onSignalHandler(const GInt32 sig)
{
    gsys::AutoLock autoLock(m_observerList.mutex());
    ObserverList::iterator iter = m_observerList.begin();
    for (; iter != m_observerList.end(); ++iter)
    {
    	switch (sig)
        {
    	    case gsys::G_SIGSEGV:
    	    	(*iter)->onSegFault();
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

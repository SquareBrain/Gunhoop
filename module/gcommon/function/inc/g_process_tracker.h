/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_process_tracker.h
* @version     
* @brief      
* @author   duye
* @date	    2014-10-11
* @note 
*
*  1. 2014-10-11 duye Created this file
* 
*/
#pragma once

#include <list>
#include <g_system.h>

namespace gcom {

/**
 * @brief process monitor interface
 */
class ProcessObserver
{
public:
    virtual ~ProcessObserver() {}
    virtual void onSegFault() = 0;
    virtual void onCtrlC() = 0;
    virtual void onOther(const GInt32 sig) = 0;
};

/**
 * @biref process tracker
 */
class ProcessTracker : public gsys::ProcessSysCallbackObserver, public Singleton<ProcessTracker>
{
public:
    typedef SecrityObj<std::list<ProcessObserver*>> ObserverList;
    
public:    
    ProcessTracker();
    ~ProcessTracker();
    
    /**
     * @brife addition process  
     * @param [in] observer : process observer
     */
    void addObserver(ProcessObserver* observer);

    /**
     * @brief remove process 
     * @param [in] observer : process observer
     */
    void removeObserver(ProcessObserver* observer);
    
    /**
     * @brief wait process exit
     * @param [in] timeout : wait time, default is 10 seconds
     */
    void wait(const GUint32 timeout = 10);
    
    /**
     * @brief wakeup all waiter thread
     */
    void wakeup();
    
private:
    // inherit from base class gsys::ProcessSysCallbackObserver 
    // system signal handler
    void onSignalHandler(const GInt32 sig);
    
private:    
    gsys::ProcessSysCallback    m_processSysCallback;
    ObserverList                m_observerList;
    Condition                   m_exitCondition;
};

}

/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_process.h
* @version     
* @brief      
* @author   duye
* @date	    2014-09-28
* @note 
*
*  1. 2014-09-28 duye Created this file
* 
*/

#pragma once

#include <list>
#include <g_type.h>
#include <g_condition.h>
#include <g_lock.h>

namespace gsys {

/**
 * @brief process monitor interface
 */
class ProcessMonitorObserver
{
public:
    virtual ~ProcessMonitorObserver() {}
    virtual void onSegmentationFault(const GInt32 sig) {}
    virtual void onCtrlC(const GInt32 sig) {}
};

class ProcessMonitor;

/**
 * @biref process monitor for system API callback
 */
class ProcessSysCallback
{
public:
    ProcessSysCallback();
    ~ProcessSysCallback();
    
    /**
     * @brief regist ProcessMonitor
     * @param [in] process_monitor : ProcessMonitor
     */
    void registProcessMonitor(ProcessMonitor* process_monitor);
    
    /**
     * @brief system signal callback
     * @param [in] sig : signal
     */
    static void signalHandlerCallback(const GInt32 sig);

private:
    static ProcessMonitor*  m_processMonitor;
};

/**
 * @biref process monitor
 */
class ProcessMonitor
{
public:
    friend class ProcessSysCallback;
    typedef std::list<ProcessMonitorObserver*> ProcessObserverList;
    
public:    
    ProcessMonitor();
    ~ProcessMonitor();
    
    static ProcessMonitor& instance();
    
    /**
     * @brife addition process monitor interface 
     * @param [in] observer : process observer
     */
    void addObserver(ProcessMonitorObserver* observer);

    /**
     * @brief remove process monitor interface
     * @param [in] observer : process observer
     */
    void removeObserver(ProcessMonitorObserver* observer);
    
    /**
     * @brief wait process exit
     * @param [in] timeout : wait time, default is 2 seconds
     */
    void waitExit(const guint32 timeout = 2);
    
    /**
     * @brief wakeup all waiter thread
     */
    void wakeupExit();
    
private:
    // system signal handler
    void signalHandler(const GInt32 sig);
    
private:    
    ProcessSysCallback  m_processSysCallback;
    ProcessObserverList m_processMonitorList;
    Mutex               m_monitorListMutex;
    Condition           m_exitCondition;
};
}

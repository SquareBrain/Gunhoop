/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_process_tracker.h
* @version     
* @brief      
* @author	duye
* @date		2014-10-11
* @note 
*
*  1. 2014-10-11 duye Created this file
* 
*/
#pragma once

#include <g_system.h>

namespace gcom {

/**
 * @brief process monitor interface
 */
class ProcessTrackerObserver
{
public:
    virtual ProcessTrackerObserver() {}
    virtual void onSegmentationFault(const GInt32 sig) {}
    virtual void onCtrlC(const GInt32 sig) {}
};

/**
 * @biref process tracker
 */
class ProcessTracker
{
public:
    typedef gsys::SecrityObj<std::list<ProcessTrackerObserver*>> ObserverList;
    
public:    
    ProcessTracker();
    ~ProcessTracker();
    
    static ProcessTracker& instance();
    
    /**
     * @brife addition process monitor interface 
     * @param [in] observer : process observer
     */
    void addObserver(ProcessTrackerObserver* observer);

    /**
     * @brief remove process monitor interface
     * @param [in] observer : process observer
     */
    void removeObserver(ProcessTrackerObserver* observer);
    
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
    ObserverList        m_observerList;
    Condition           m_exitCondition;
};


}

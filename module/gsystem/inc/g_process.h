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

namespace gsys {

/**
 * @brief process monitor interface
 */
class ProcessMonitorInterface
{
public:
    virtual ~ProcessMonitorInterface() {}
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
    typedef std::list<ProcessMonitorInterface*> ProcessMonitorList;
    
public:    
    ProcessMonitor();
    ~ProcessMonitor();
    
    static ProcessMonitor* getInstance();
    
    /**
     * @brife addition process monitor interface 
     * @param [in] monitor_interface : process monitor interface
     */
    void addMonitor(ProcessMonitorInterface* monitor_interface);

    /**
     * @brief remove process monitor interface
     * @param [in] monitor_interface : process monitor interface
    */
    void removeMonitor(ProcessMonitorInterface* monitor_interface);
    
private:
    // system signal handler
    void signalHandler(const GInt32 sig);
    
private:    
    ProcessSysCallback  m_processSysCallback;
    ProcessMonitorList  m_processMonitorList;
};

}

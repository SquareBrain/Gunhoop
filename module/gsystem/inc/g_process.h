/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_process.h
* @version     
* @brief      
* @author   duye
* @date     2014-09-28
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
 * @brief process monitor
 */
class ProcessMonitorInterface
{
public:
    virtual ~ProcessMonitorInterface() {}
    
    virtual void onSegmentationFault(const GInt32 sig) {}
    virtual void onCtrlC(const GInt32 sig) {}
};

/**
 * @biref process monitor
 */
class ProcessMonitor
{
public:
    typedef std::list<ProcessMonitorInterface*> MonitorList;
    
public:
    ProcessMonitor();
    ~ProcessMonitor();
    
    void addMonitor(ProcessMonitorInterface* monitor);
    
private:
    static void signalHandler(const GInt32 sig);
    
private:
    MonitorList     m_monitorList;
};
}

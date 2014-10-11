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
 * @brief system callback observer
 */
class ProcessSysCallbackObserver
{
public:
    virtual ~ProcessSysCallbackObserver() {}

    /**
     * @brief system signal handler
     * @param [in] sig : signal
     */    
    void onSignalHandler(const GInt32 sig);
};

/**
 * @biref process monitor for system API callback
 */
class ProcessSysCallback
{
public:
    ProcessSysCallback();
    ~ProcessSysCallback();
    
    /**
     * @brief regist process system callback observer
     * @param [in] observer : observer
     */
    void registObserver(ProcessSysCallbackObserver* observer);
    
    /**
     * @brief system signal callback
     * @param [in] sig : signal
     */
    static void signalHandlerCallback(const GInt32 sig);

private:
    ProcessSysCallbackObserver*  m_observer;
};

}

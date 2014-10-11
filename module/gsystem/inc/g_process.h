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

#include <g_type.h>

namespace gsys {

/**
 * @brief process monitor interface
 */
class ProcessObserver
{
public:
    virtual ~ProcessObserver() {}
    virtual void onSegFault(const GInt32 sig) = 0;
    virtual void onCtrlC(const GInt32 sig) = 0;
    virtual void onOther(const GInt32 sig) = 0;
};

/**
 * @brief process
 */
class Process
{
public:
    /**
     * @brief process signal type
     */
    typedef enum
    {
        G_SIGSEGV = 0,
        G_SIGINT    
    } ProcessSignal;
        
public:
    Process();
    ~Process();

    /**
     * @brief set/get process id
     * @return process id
     */
    void setId(const GInt32 id);
    GInt32 id();

private:
    GInt32 m_id;
};

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
    virtual void onSignalHandler(const GInt32 sig) = 0;
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

/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_host_server.h
* @version     
* @brief      
* @author   duye
* @date     2014-10-09
* @note 
*
*  1. 2014-10-09 duye Created this file
* 
*/
#pragma once

#include <g_system.h>

namespace gcom {
    
/**
 * @brief server statue
 */
typedef enum 
{
    G_HSERVER_INIT = 0,
    G_HSERVER_WORK,
    G_HSERVER_STOP,
    G_HSERVER_FAULT
} HostServerState;
  
/**
 * @brief host server base class
 */
class HostServer : public gsys::ThreadTask
{
public:
    HostServer();
    virtual ~HostServer();
    
    /**
     * @brief startup service
     * @return G_YES/G_NO
     */       
    virtual GResult start() = 0;
	
    /**
     * @brief stop service
     * @return G_YES/G_NO
     */
    virtual GResult stop() = 0;
	
    /**
     * @brief message loop handle
     * @return G_YES/G_NO
     */
    virtual GResult routine() = 0;       
	
    /**
     * @brief set/get host server state	
     */
    void setState(const HostServerState& state);
    const HostServerState& state() const;
    
private:
    // inherit from base class gsys::ThreadTask
    // to run new service thread
    GResult run();
	
private: 
    HostServerState     m_state;	 
};
}

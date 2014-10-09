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

namespace gcom {
    
/**
 * @brief server statue
 */
typedef enum 
{
    HOST_SERVER_INIT = 0,
    HOST_SERVER_WORK,
    HOST_SERVER_STOP,
    HOST_SERVER_FAULT
} HostServerState;
  
/**
 * @brief host server base class
 */
class HostServer
{
public:
    HostServer() {}
    virtual ~HostServer() {}
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
	 */
	virtual void routine() = 0;       
	
	/**
	 * @brief set/get host server state
	 */
	void setServerState(const HostServerState& state) { m_serverState = state; }
	const HostServerState& getServerState() const { return m_serverState; }
	
private:
	HostServerState     m_serverState;
};

}

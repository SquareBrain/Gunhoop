/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_network_server.h
* @version     
* @brief      
* @author   duye
* @date     2014-10-29
* @note 
*
*  1. 2014-10-29 duye Created this file
* 
*/
#pragma once

#include <string>
#include <list>
#include <g_system.h>
#include <g_security_obj.h>
#include <g_network_def.h>

namespace gcom {

/**
 * @brief network server running state
 */
typedef enum
{
    G_SERVER_INIT,
    G_SERVER_WORK,
    G_SERVER_STOP,
    G_SERVER_FAULT
} ServerState;

/**
 * @brief server user interface
 */
class NetworkServerObserver
{
public:
    virtual ~NetworkServerObserver() {}
    
    /**
     * @brief message handler
     * @param [in] data : user data
     * @param [in] len : data length
     * @return G_YES/G_NO
     */
     virtual GResult onMessage(void* data, const GUint64 len);
};

/**
 * @brief network server
 */
class NetworkServer : public gsys::ThreadTask
{
public:
    typedef SecurityObj<std::list<NetworkServerObserver*>> ObserverList;
  
public:
    NetworkServer();
    
    /**
     * @brief constructor
     * @param [in] server_addr : ftp server address
     * @param [in] net_card : network card for communication, defualt is eth0
     */    
    explicit NetworkServer(const IPPortPair& server_addr, const std::string& net_card = "eth0");
    virtual ~NetworkServer();
    
    /**
     * @brief startup service
     * @return G_YES/G_NO
     */
    virtual GResult start() = 0;
    
    /**
     * @brief startup service
     * @param [in] server_addr : ftp server address
     * @param [in] net_card : network card for communication, defualt is eth0
     * @return G_YES/G_NO
     */
    virtual GResult start(const IPPortPair& server_addr, const std::string& net_card = "eth0") = 0;
    
    /**
     * @brief restart service
     * @return G_YES/NO
     */
    virtual GResult restart() = 0;
    
    /**
     * @brief stop service
     * @return G_YES/G_NO
     */
    virtual GResult stop() = 0;
     
    /**
     * @brief message loop handle, new thread
     * @note derive class implemention
     * @return G_YES/G_NO
     */
    virtual GResult routine() = 0; 

    /**
     * @brief to keep server work
     * @return G_YES/G_NO
     */
    GResult keepWork();    
    
    /**
     * @brief addition observer
     * @param [in] observer : user observer
     * @return G_YES/G_NO
     */
    GResult addObserver(NetworkServerObserver* observer);
    
    /**
     * @brief remove observer
     * @param [in] observer : user observer
     * @return G_YES/G_NO
     */
    GResult removeObserver(NetworkServerObserver* observer);    
      
    /**
     * @brief get server address
     * @return server address
     */
    void setServerAddr(const IPPortPair& server_addr);
    const IPPortPair& serverAddr() const;
    
    /**
     * @brief get net card
     * @return net card
     */
    void setNetCard(const std::string& net_card);
    const std::string& netCard() const;    
    
    /**
     * @brief set/get server state
     * @return server state
     */
    void setState(const ServerState state);
    const ServerState& state() const;
    
    /**
     * @brief get observer list
     * @return observer list
     */
    const ObserverList& observerList() const;    
	
private:	
    // inherit from base class gsys::ThreadTask
    // to run new service thread
    GResult run();

private:
    IPPortPair		m_serverAddr;
    std::string 	m_netCard;
    ServerState 	m_state;
    ObserverList 	m_observerList;
};
}

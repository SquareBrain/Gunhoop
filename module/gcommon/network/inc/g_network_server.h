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
#include <g_network_def.h>

namespace gcom {

/**
 * @brief network server running state
 */
typedef enum
{
    SERVER_INIT,
    SERVER_WORK,
    SERVER_STOP,
    SERVER_FAULT
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
     * @param [in] user_data : user data
     * @return G_YES/G_NO
     */
     virtual GResult onMessage(void* user_data);
};

/**
 * @brief network server
 */
class NetworkServer : public gsys::ThreadTask
{
public:
    typedef gsys::SecrityObj<std::list<NetworkServerInterface*>> ObserverList;
  
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
    const IPPortPair& serverAddr() const;
    
    /**
     * @brief get net card
     * @return net card
     */
    const std::string netCard() const;    
    
    /**
     * @brief get server state
     * @return server state
     */
    const ServerState& state() const;
    
    /**
     * @brief get observer list
     * @return observer list
     */
    ObserverList& observerList() const;
	
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

/**
 * @brief network server monitor
 */
class NetworkServerTracker
{
public:
    NetworkServerTracker();
    ~NetworkServerTracker();
    
    /**
     * @brief to keep server work
     * @return G_YES/G_NO
     */
    static GResult keepWork(NetworkServer* server);
};

}

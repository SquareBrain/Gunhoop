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
 * @brief network server running status
 */
typedef enum
{
    // uninit status
    G_SERVER_INIT,
    // working status
    G_SERVER_WORK,
    // stopped status
    G_SERVER_STOP,
    // server happened fault
    G_SERVER_FAULT
} ServerState;
    
/**
 * @brief base message
 */
class BaseMessage
{
public:
    BaseMessage();
    virtual ~BaseMessage();
    
    /**
     * @brief set data 
     * @param [in] data : original network data
     * @param [in] len : original network data length
     */
    void setData(GInt8* data, const GUint64 len);
    
    /**
     * @brief get data
     * @return data pointer
     */
    GInt8* data() const;
    
    /**
     * @brief get length
     * @return data length
     */
    GUint64 length() const;
    
private:
    // free data
    void clear();
    
private:
    // data length
    GUint64 m_dataLen;
    // original network data
    GInt8*  m_orgData;
};

/**
 * @brief server user interface
 */
class NetworkServerObserver
{
public:
    virtual ~NetworkServerObserver() {}
    
    /**
     * @brief message handler
     * @param [in] message : message
     * @return G_YES/G_NO
     */
     virtual GResult onMessage(BaseMessage* message);
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
     * @brief verify server address
     * @return G_YES/G_NO
     */
    GResult isValidServerAddr() const;
    
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
    
/**
 * @brief client agent
 */
class ClientAgent
{
public:
    ClientAgent();
    explicit ClientAgent(const GInt32 sockfd, const NetAddr& clientAddr);
    ~ClientAgent();
  
    /**
     * @brief set/get sock fd
     * @param [in] sockfd : socket fd
     * @return sock fd
     */
    void setSockfd(const GInt32 sockfd);
    GInt32 sockfd() const;
    
    /**
     * @brief set/get client address
     * @param [in] clientAddr : client address
     * @return client address
     */
    void setClientAddr(const NetAddr& clientAddr);
    const NetAddr& clientAddr() const;

private:
    GInt32    m_sockfd;
    NetAddr   m_netAddr;
};
    
}

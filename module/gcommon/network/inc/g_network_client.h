/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_network_client.h
* @version     
* @brief      
* @author   duye
* @date	    2014-09-30
* @note 
*
*  1. 2014-09-30 duye Created this file
* 
*/
#pragma once

#include <string>
#include <g_system.h>
#include <g_network_def.h>

namespace gcom {
    
/**
 * @brief network client connect state
 */
typedef enum
{
    CLIENT_INIT,
    CLIENT_ON,
    CLIENT_OFF
} ClientState;

/**
 * @biref network client observer
 */
class NetworkClientObserver
{
public:
    virtual ~NetworkClientObserver() {}
    
    /**
     * @brief handle server response message, user need implement
     * @param [in] msg : message data
     * @return G_YES/G_NO
     */
    virtual GResult onServerResponse(const GInt8* msg) = 0; 
};

/**
 * @brief network client component base class
 */
class NetworkClient : public gsys::ThreadTask
{
public:
    typedef gsys::SecrityObj<std::list<NetworkClientObserver*>> ObserverList;
    
public:
    NetworkClient();
    
    /**
     * @brief constructor
     * @param [in] server_addr : ftp server address
     * @param [in] net_card : network card for communication, defualt is eth0
     */    
    explicit NetworkClient(const IPPortPair& server_addr, const std::string& net_card = "eth0");
    virtual ~NetworkClient();
    
    /**
     * @brief to connect ftp server
     * @return G_YES/G_NO
     * @note derive class implemention
     */
    virtual GResult connect() = 0;
    
    /**
     * @brief to connect ftp server
     * @param [in] server_addr : server address
     * @param [in] server_addr : ftp server address
     * @param [in] net_card : network card for communication, defualt is eth0     
     * @return G_YES/G_NO
     * @note derive class implemention
     */
    virtual GResult connect(const IPPortPair& server_addr, const std::string& net_card = "eth0") = 0;
    
    /**
     * @brief send message
     * @param [in] data : by sent data
     * @param [in] len : data length
     * @return have sent size, -1 failed
     * @note derive class implemention
     */
    virtual GInt64 sendMsg(const GInt8* data, const GUint64 len) = 0;
    
    /**
     * @brief message loop handle, new thread
     * @note derive class implemention
     * @return G_YES/G_NO
     */
    virtual GResult msgLoop() = 0;
    
    /**
     * @brief get server address
     * @return server address
     */
    const IPPortPair& serverAddr() const;

    /**
     * @brief get net card
     * @return net card
     */
    const std::string& netCard() const;
    
    /**
     * @brief set/get connect state
     */
    void setState(const ClientState& state);
    const ClientState& state() const;    
    
    /**
     * @brief add/remove observer
     * @brief [in] observer : observer
     * @return G_YES/G_NO
     */
    GResult addObserver(NetworkClientObserver* observer);
    GResult removeObserver(NetworkClientObserver* observer);    

private:
    GResult run();
    GResult findObserver(NetworkClientInterface* observer);
    
protected:
    IPPortPair          m_serverAddr;
    std::string         m_netCard;
    ClientState  	    m_state;
    ObserverList        m_observerList;
};

}

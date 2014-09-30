/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_network_client.h
* @version     
* @brief      
* @author	duye
* @date		2014-09-30
* @note 
*
*  1. 2014-09-30 duye Created this file
* 
*/

#include <g_system.h>
#include <g_network_def.h>

namespace gcom {
    
/**
 * @brief connect state
 */
typedef enum
{
    CONNECT_INIT,
    CONNECT_ON,
    CONNECT_OFF
} ClientConnectState;

/**
 * @biref network client interface
 */
class NetworkClientInterface
{
public:
    virtual ~NetworkClientInterface() {}
    
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
    typedef std::list<NetworkClientInterface*> ObserverList;
    
public:
    NetworkClient();
    explicit NetworkClient(const IPPortPair& server_addr);
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
     * @return G_YES/G_NO
     * @note derive class implemention
     */
    virtual GResult connect(const IPPortPair& server_addr) = 0;
    
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
     */
    virtual void msgLoop() = 0;
    
    /**
     * @brief get server address
     * @param [in] server_addr : server address
     */
    const IPPortPair& getServerAddr() const;
    
    /**
     * @brief get connect state
     */
    const ClientConnectState& getConnectState() const;
    
    /**
     * @brief add/remove observer
     * @brief [in] observer : observer
     * @return G_YES/G_NO
     */
    GResult addObserver(NetworkClientInterface* observer);
    GResult removeObserver(NetworkClientInterface* observer);    

private:
    GResult run();
    GResult findObserver(NetworkClientInterface* observer);
    
protected:
    IPPortPair          m_serverAddr;
    ClientConnectState  m_connectState;
    ObserverList        m_observerList;
};

}

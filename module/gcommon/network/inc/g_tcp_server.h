/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_tcp_server.h
* @version     
* @brief      
* @author   duye
* @date     2014-08-29
* @note 
*
*  1. 2014-08-29 duye Created this file
* 
*/

#pragma once

#include <g_system.h>
#include <g_network.h>

namespace gcom {
	
/**
 * @brief connect state
 */
typedef enum
{
	// don't connect state
	TCP_CON_STATE_OFF,
	// connected state
	TCP_CON_STATE_CON,
	// idle state
	TCP_CON_STATE_IDLE,
	// closed state
	TCP_CON_STATE_CLOSED
} TcpConnectState;

/**
 * @brief tcp server user interface
 */
class TcpServiceInterface
{
public:
	virtual ~TcpServiceInterface() {}
	virtual GResult accepted(const SocketAddr& clientAddr) = 0;
};

/**
 * @brief tcp server
 */
class TcpService
{
public:
	TcpService();
	explicit TcpService(const IPPortPair& serverAddr, TcpServerInterface* interface = nullptr);
	virtual ~TcpService();
	
	/**
	 * @brief start
	 * @return G_YES/G_NO
	 */
	GResult start();
	
	/**
	 * @brief stop
	 * @return G_YES/G_NO
	 */
	GResult stop();
	
	/**
	 * @brief setting server address
	 */
	void setServerAddr(const IPPortPair& serverAddr);
	
	/**
	 * @brief send data
	 * @param [in] data : send data
	 * @param [in] len : data length
	 * @return -1:failed, > 0:send size
	 */
	GInt64 send(const GInt8* data, const GUing64 len);
	
	/**
	 * @brief receive data
	 * @param [in] buffer : data buffer
	 * @param [in] size : buffer size
	 * @return -1:failed, > 0:received size
	 */
	GInt64 recv(const GInt8* buffer, const GUing64 size);
	
private:
	IPPortPair			m_serverAddr;
	TcpServerInterface*	m_tcpServerInterface;
};
}

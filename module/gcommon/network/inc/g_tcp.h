/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_tcp.h
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
	
class TcpInterface
{
public:
	virtual ~TcpInterface() {}
	virtual GResult state(const TcpConnectState& state) = 0;
	virtual GResult received(GInt8* data, const GUint64 len) = 0;
};

/**
 * @brief tcp socket class
 */
class Tcp : public gsys::ThreadTask
{
public:
	Tcp(const IPPortPair& ipPortPair, const TcpInterface* interface/*nullptr*/);
	~Tcp();
	
	/**
	 * @brief connect
	 * @return G_YES/G_NO
	 */
	GResult connect();
	
	/**
	 * @brief close
	 * @return G_YES/G_NO
	 */
	GResult close();
	
	/**
	 * @brief send data
	 * @param [in] data : send data
	 * @param [in] len : data length
	 * @return -1:failed, > 0:send size
	 */
	GInt64 send(const GInt8* data, const GUing64 len);
	
private:
	virtual GResult run();
	
private:
	SocketAddr		m_socketAddr;
	TcpInterface*	m_tcpInterface;
};

/**
 * @brief tcp component
 */
class TcpClient : public TcpInterface
{
public:
	TcpClient();
	virtual ~TcpClient();
	
	/**
	 * @brief setting server address
	 */
	void setServerAddr(const IPPortPair& serverAddr);
	
	/**
	 * @brief start
	 */
	GResult start();
	
	/**
	 * @brief stop
	 */
	GResult stop();

protected:
	virtual GResult state(const TcpConnectState& state) = 0;
	virtual GResult received(GInt8* data, const GUint64 len) = 0;
	
private:
	Tcp		m_tcp;
};

}

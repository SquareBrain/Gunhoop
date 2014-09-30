/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_ftp_client.h
* @version     
* @brief      
* @author   duye
* @date     2014-09-28
* @note 
*
*  1. 2014-09-28 duye Created this file
* 
*/
#pragma once

#include <g_network_client.h>
#include <g_tcp_client.h>
#include <g_udp_client.h>

namespace gcom {

class FtpClient : public NetworkClient
{
public:
	FtpClient();
	explicit FtpClient(const IPPortPair& server_addr);
	virtual ~FtpClient();
	
	/**
	 * @brief inherit from base class NetworkClient, to connect server
	 */
	GResult connect();
	GResult connect(const IPPortPair& server_addr);
	
	/**
	 * @brief inherit from base class NetworkClient, to handle server response, implement by user
	 */
	virtual GResult response(const GInt8* msg) = 0;
	
private:
	TcpClient	m_tcpClient;
	UdpClinet	m_udpClient;
};

}

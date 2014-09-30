/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_rpc_client.h
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
#include <g_http_client.h>

namespace gcom {

/**
 * @brief rpc client
 */
class RpcClient : public NetworkClient
{
public:
	RpcClient();
	/**
	 * @brief constructor
	 * @param [in] server_addr : ftp server address
	 * @param [in] interface : network card name, defualt is eth0
	 */
	explicit RpcClient(const IPPortPair& server_addr, const GInt8* interface = "eth0");
	virtual ~RpcClient();
	
	/**
	 * @brief to connect ftp server
	 * @return G_YES/G_NO
	 * @note derive class implemention
	 */
	GResult connect();
    
	/**
	 * @brief to connect ftp server
	 * @param [in] server_addr : server address
	 * @return G_YES/G_NO
	 * @note derive class implemention
	 */
	GResult connect(const IPPortPair& server_addr);
	
	/**
	 * @brief send message
	 * @param [in] data : by sent data
	 * @param [in] len : data length
	 * @return have sent size, -1 failed
	 * @note derive class implemention
	 */
	GInt64 sendMsg(const GInt8* data, const GUint64 len);
	
	/**
	 * @brief message loop handle, new thread
	 * @note derive class implemention
	 */
	void msgLoop();	
	
private:
	TcpClient	m_tcpClient;
	UdpClinet	m_udpClient;
};

}

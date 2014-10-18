/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_udp_client.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-09-30
* @note 
*
*  1. 2014-09-30 duye Created this file
* 
*/
#include <g_udp_client.h>

namespace gcom {

UdpClient::UdpClient() {}
UdpClient::UdpClient(const IPPortPair& server_addr, const std::string& net_card) 
	: NetworkClient(server_addr, net_card) {}

UdpClient::~UdpClient() {}

GResult UdpClient::connect()
{
	if (serverAddr().ipAddr().ipStr().empty())
	{
		return G_NO;
	}
	
	return G_YES;
}

GResult UdpClient::connect(const IPPortPair& server_addr, const std::string& net_card)
{
	setServerAddr(server_addr);
	setNetCard(net_card);
	return connect();
}

GInt64 UdpClient::sendMsg(const GInt8* data, const GUint64 len)
{
	GInt64 size = -1;
	return size;
}

GResult UdpClient::routine()
{
	for (;;)
	{
		gsys::System::sleep(2); 
	}
	
	return G_YES;
}
}

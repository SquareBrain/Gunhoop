/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_rpc_client.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-08-30
* @note 
*
*  1. 2014-08-30 duye Created this file
* 
*/
#include <g_rpc_client.h>

namespace gcom {

RpcClient::RpcClient() {}
RpcClient::RpcClient(const IPPortPair& server_addr, const std::string& net_card) 
	: NetworkClient(server_addr, net_card) {}

RpcClient::~RpcClient() {}

GResult RpcClient::connect()
{
	if (serverAddr().ipAddr().ipStr().empty())
	{
		return G_NO;
	}
	
	return G_YES;
}

GResult RpcClient::connect(const IPPortPair& server_addr, const std::string& net_card)
{
	setServerAddr(server_addr);
	setNetCard(net_card);
	return connect();
}

GInt64 RpcClient::sendMsg(const GInt8* data, const GUint64 len)
{
	GInt64 size = -1;
	return size;
}

GResult RpcClient::routine()
{
	for (;;)
	{
		gsys::System::sleep(2); 
	}
	
	return G_YES;
}
}

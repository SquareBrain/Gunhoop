/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_rpc_server.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-08-30
* @note 
*
*  1. 2014-08-30 duye Created this file
* 
*/
#include <g_rpc_server.h>

namespace gcom {

RpcServer::RpcServer() {}
RpcServer::RpcServer(const IPPortPair& server_addr, const std::string& net_card)
	: NetworkServer(server_addr, net_card) {}
RpcServer::~RpcServer() {}
	  
GResult RpcServer::start()
{
	return G_YES;
}

GResult RpcServer::start(const IPPortPair& server_addr, const std::string& net_card)
{
	setServerAddr(server_addr);
	setNetCard(net_card);
	return start();
}

GResult RpcServer::restart()
{
	if (state() != G_SERVER_WORK)
	{
		return start();
	}
	
	return G_YES;
}

GResult RpcServer::stop()
{
	return G_YES;
}

GResult RpcServer::routine()
{
	for (;;)
	{
		gsys::System::sleep(2);
	}

	return G_YES;
}
}

/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_tcp_server.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-09-30
* @note 
*
*  1. 2014-09-30 duye Created this file
* 
*/
#include <g_tcp_server.h>
	
namespace gcom {

TcpServer::TcpServer() {}
TcpServer::TcpServer(const IPPortPair& server_addr, const std::string& net_card)
	: NetworkServer(server_addr, net_card) {}
TcpServer::~TcpServer() {}
	  
GResult TcpServer::start()
{
	return G_YES;
}

GResult TcpServer::start(const IPPortPair& server_addr, const std::string& net_card)
{
	setServerAddr(server_addr);
	setNetCard(net_card);
	return start();
}

GResult TcpServer::restart()
{
	if (state() != G_SERVER_WORK)
	{
		return start();
	}
	
	return G_YES;
}

GResult TcpServer::stop()
{
	return G_YES;
}

GResult TcpServer::routine()
{
	for (;;)
	{
		gsys::System::sleep(2);
	}

	return G_YES;
}
}

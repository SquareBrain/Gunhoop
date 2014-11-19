/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_cli_server.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-11-19
* @note 
*
*  1. 2014-11-19 duye Created this file
* 
*/
#include <g_cli_server.h>
	
namespace gcom {

CliServer::CliServer() {}
CliServer::CliServer(const IPPortPair& server_addr, const std::string& net_card)
	: NetworkServer(server_addr, net_card) {}
CliServer::~CliServer() {}
	  
GResult CliServer::start()
{
	return G_YES;
}

GResult CliServer::start(const IPPortPair& server_addr, const std::string& net_card)
{
	setServerAddr(server_addr);
	setNetCard(net_card);
	return start();
}

GResult CliServer::restart()
{
	if (state() != G_SERVER_WORK)
	{
		return start();
	}
	
	return G_YES;
}

GResult CliServer::stop()
{
	return G_YES;
}

GResult CliServer::routine()
{
	for (;;)
	{
		gsys::System::sleep(2);
	}

	return G_YES;
}
}

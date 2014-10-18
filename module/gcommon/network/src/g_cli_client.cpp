/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_cli_client.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-10-17
* @note 
*
*  1. 2014-10-17 duye Created this file
* 
*/
#include <g_cli_client.h>

namespace gcom {

CliClient::CliClient() {}
CliClient::CliClient(const IPPortPair& server_addr, const std::string& net_card) 
	: NetworkClient(server_addr, net_card) {}

CliClient::~CliClient() {}

GResult CliClient::connect()
{
    if (serverAddr().ipAddr().ipStr().empty())
    {
    	return G_NO;
    }
	
    return G_YES;
}

GResult CliClient::connect(const IPPortPair& server_addr, const std::string& net_card)
{
    setServerAddr(server_addr);
	setNetCard(net_card);
	return connect();
}

GInt64 CliClient::sendMsg(const GInt8* data, const GUint64 len)
{
    GInt64 size = -1;
    return size;
}

GResult CliClient::routine()
{
    for (;;)
    {
    	gsys::System::sleep(2);	
    }
    
    return G_YES;
}
}

/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_http_client.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-08-29
* @note 
*
*  1. 2014-08-29 duye Created this file
* 
*/
#include <g_http_client.h>

namespace gcom {

HttpClient::HttpClient() {}
HttpClient::HttpClient(const IPPortPair& server_addr, const std::string& net_card) 
	: NetworkClient(server_addr, net_card) {}

HttpClient::~HttpClient() {}

GResult HttpClient::connect()
{
    if (serverAddr().ipAddr().ipStr().empty())
    {
    	return G_NO;
    }
	
    return G_YES;
}

GResult HttpClient::connect(const IPPortPair& server_addr, const std::string& net_card)
{
    setServerAddr(server_addr);
	setNetCard(net_card);
	return connect();
}

GInt64 HttpClient::sendMsg(const GInt8* data, const GUint64 len)
{
    GInt64 size = -1;
    return size;
}

GResult HttpClient::routine()
{
    for (;;)
    {
    	gsys::System::sleep(2);	
    }
    
    return G_YES;
}
}

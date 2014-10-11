/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_ftp_client.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-09-28
* @note 
*
*  1. 2014-09-28 duye Created this file
* 
*/
#include <g_tcp_client.h>
#include <g_udp_client.h>
#include <g_ftp_client.h>

namespace gcom {

FtpClient::FtpClient() {}
FtpClient::FtpClient(const IPPortPair& server_addr, const std::string& net_card) : NetworkClient(server_addr, net_card) {}
FtpClient::~FtpClient();

GResult FtpClient::connect()
{
    if (getServerAddr().getIPAddr().getIPStr().empty())
    {
    	return G_NO;
    }
	
    return G_YES;
}

GResult FtpClient::connect(const IPPortPair& server_addr, const std::string& net_card)
{
    if (getServerAddr().getIPAddr().getIPStr().empty())
    {
    	return G_NO;
    }

    return G_YES;
}

GInt64 FtpClient::sendMsg(const GInt8* data, const GUint64 len)
{
    GInt64 size = -1;
    return size;
}

GResult FtpClient::routine()
{
    for (;;)
    {
    	gsys::System::sleep(2);	
    }
    
    return G_YES;
}

}

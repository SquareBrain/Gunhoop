/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_client_factory.cpp
* @version     
* @brief      
* @author   duye
* @date	    2014-10-12
* @note 
*
*  1. 2014-10-12 duye Created this file
* 
*/
#include <g_client_factory.h>

namespace gcom {

ClientFactory::ClientFactory() {}
ClientFactory::~ClientFactory() {}

NetworkClient* ClientFactory::create(const ClientType& client_type)
{
    NetworkClient* client = nullptr;

    switch (client_type)
    {
        case G_CLIENT_FTP:
        {
            client = new FtpClient;
            break;
        }
        case G_CLIENT_HTTP:
        {
            client = new HttpClient;
            break;
        }
        case G_CLIENT_RPC:
        {
            client = new RpcClient;
            break;
        }
        case G_CLIENT_TCP:
        {
            client = new TcpClient;
            break;
        }
        case G_CLIENT_UDP:
        {
            client = new UdpClient;
            break;
        }
        case G_CLIENT_CLI:
        {
            client = new CliClient;
            break;
        }
        default:
            break;
    }
    
    return client;
}

void ClientFactory::destroy(const ClientType& client_type, NetworkClient* client)
{
    switch (client_type)
    {
        case G_CLIENT_FTP:
        {
            FtpClient* del_client = dynamic_cast<FtpClient*>client;
            delete del_client;
            break;
        }
        case G_CLIENT_HTTP:
        {
            HttpClient* del_client = dynamic_cast<HttpClient*>client;
            delete del_client;
            break;
        }
        case G_CLIENT_RPC:
        {
            RpcClient* del_client = dynamic_cast<RpcClient*>client;
            delete del_client;
            break;
        }
        case G_CLIENT_TCP:
        {
            TcpClient* del_client = dynamic_cast<TcpClient*>client;
            delete del_client;
            break;
        }
        case G_CLIENT_UDP:
        {
            UdpClient* del_client = dynamic_cast<UdpClient*>client;
            delete del_client;
            break;
        }
        case G_CLIENT_CLI:
        {
            CliClient* del_client = dynamic_cast<CliClient*>client;
            delete del_client;
            break;
        }
        default:
            break;
    }
}
}

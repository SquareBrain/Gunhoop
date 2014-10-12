/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_server_factory.cpp
* @version     
* @brief      
* @author   duye
* @date	    2014-10-12
* @note 
*
*  1. 2014-10-12 duye Created this file
* 
*/
#include <g_server_factory.h>

ServerFactory::ServerFactory() {}
ServerFactory::~ServerFactory() {}

NetworkServer* ServerFactory::createServer(const ServerType& server_type)
{
    NetworkServer* server = nullptr;
    
    switch (server_type)
    {
        case SERVER_FTP:
            server = new FtpServer;
            break;
        case SERVER_HTTP:
            server = new HttpServer;
            break;
        case SERVER_RPC:
            server = new RpcServer;
            break;
        case SERVER_TCP:
            server = new TcpServer;
            break;
        case SERVER_UDP:
            server = new UdpServer;
            break;
        case SERVER_CLI:
            server = new CliServer;
            break;
        default:
            break;
    }
    
    return server;
}

void ServerFactory::destroyServer(const ServerType& server_type, NetworkServer* server)
{
    switch (server_type)
    {
        case SERVER_FTP:
        {    
            FtpServer* del_server = dynamic_cast<FtpServer*>server;
            delete del_server;
            break;
        }
        case SERVER_HTTP:
        {    
            HttpServer* del_server = dynamic_cast<HttpServer*>server;
            delete del_server;
            break;
        }
        case SERVER_RPC:
        {    
            RpcServer* del_server = dynamic_cast<RpcServer*>server;
            delete del_server;
            break;
        }
        case SERVER_TCP:
        {    
            TcpServer* del_server = dynamic_cast<TcpServer*>server;
            delete del_server;
            break;
        }
        case SERVER_UDP:
        {    
            UdpServer* del_server = dynamic_cast<UdpServer*>server;
            delete del_server;
            break;
        }
        case SERVER_CLI:
        {    
            CliServer* del_server = dynamic_cast<CliServer*>server;
            delete del_server;
            break;
        }
        default:
            break;
    }    
}

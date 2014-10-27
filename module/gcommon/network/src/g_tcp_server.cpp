/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_tcp_server.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-09-30
* @note 
*
*  1. 2014-09-30 duye Created this file
* 
*/
#include <g_socket.h>
#include <g_tcp_server.h>
	
namespace gcom {

static const GInt8* LOG_PREFIX = "gohoop.gcom.network.tcpserver";

TcpServer::TcpServer() : m_serverSocket(nullptr){}
TcpServer::TcpServer(const IPPortPair& server_addr, const std::string& net_card)
    : NetworkServer(server_addr, net_card)
    , m_serverSocket(nullptr) {}
TcpServer::~TcpServer() 
{
    delete m_serverSocket;
    m_serverSocket = nullptr;
}
	  
GResult TcpServer::start()
{
    if (server_addr.ipAddr().ipStr().empty())
    {
    	G_LOG_ERROR(LOG_PREFIX, "server address no setting");
    	return G_NO;
    }
    
    SocketInfo socket_info;
    socket_info.setProtocol(G_IPPROTO_TCP);
    socket_info.setServerIP(serverAddr().ipAddr().ip());
    socket_info.setServerPort(serverAddr().port());
    socket_info.setLocalIfName(netCard());
    
    m_serverSocket = new gsys::ServerSocket();
    if (IS_NO(m_serverSocket->bind(socket_info)))
    {
    	G_LOG_ERROR(LOG_PREFIX, "bind server address failed");
    	return G_NO;
    }
    
    if (IS_NO(m_serverSocket->listen()))
    {
    	G_LOG_ERROR(LOG_PREFIX, "listen server address failed");
    	return G_NO;    	
    }
    
    startTask();
    
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
    	SockAddr& client_addr;
    	if (IS_YES(m_serverSocket->accept(client_addr, )))
    	{
    	    G_LOG_INFO(LOG_PREFIX, "accept client address %s:%d", );
    	}
    	
    	gsys::System::sleep(2);
    }

    return G_YES;
}
}

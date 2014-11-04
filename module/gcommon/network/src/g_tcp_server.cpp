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
#include <g_tcp_server.h>
	
namespace gcom {

static const GInt8* LOG_PREFIX = "gohoop.gcom.network.tcpserver";

TcpServer::TcpServer() : m_socket(nullptr){}
TcpServer::TcpServer(const IPPortPair& server_addr, const std::string& net_card)
    : NetworkServer(server_addr, net_card) {}
TcpServer::~TcpServer() 
{
    close();
    
    gsys::AutoLock auto_lock(m_clientMap.mutex());
    ClientAgentMap::const_iterator iter = m_clientMap.begin();
    for (; iter != m_clientMap.end(); ++iter)
    {
        delete iter->second;
    }
    m_clientMap.clean();
}
	  
GResult TcpServer::start()
{
    if (state() == G_SERVER_WORK)
    {
        return G_YES;
    }
    
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
    
    if (IS_NO(m_socket.open(socket_info)))
    {
    	G_LOG_ERROR(LOG_PREFIX, "%s", m_serverSocket.error());
    	return G_NO;
    }
    
    // init epoll
    if (IS_NO(m_epoll.open()))
    {
    	G_LOG_ERROR(LOG_PREFIX, "%s", m_epoll.error());
    	return G_NO;    	
    }
    
    startTask(false);
    
    setState(G_SERVER_WORK);
    
    return G_YES;
}

GResult TcpServer::start(const IPPortPair& server_addr, const std::string& net_card)
{
    if (state() == G_SERVER_WORK)
    {
    	stop();
    }
    
    setServerAddr(server_addr);
    setNetCard(net_card);
    return start();
}

GResult TcpServer::restart()
{
    stop();
    return start();
}

GResult TcpServer::stop()
{
    if (state() == G_SERVER_STOP)
    {
        return G_YES;	
    }
    
    m_socket.close();
    m_epoll.close();
  
    setState(G_SERVER_STOP);
    
    // wait thread exit
    join();
    
    return G_YES;
}

GResult TcpServer::routine()
{
    // add server listen sockfd to epoll
    m_epoll.addfd(m_serverSocket.socket().sockfd());
    
    for (;;)
    {
    	if (state() == G_SERVER_STOP)
    	{
    	    gsys::System::usleep(10);
            break;
    	}
    	else if (state() == G_SERVER_FAULT)
    	{
       	    gsys::System::sleep(1);
    	    continue; 	    
    	}
    	
    	gsys::Epoll::EventList event_list;
    	if (IS_NO(m_epoll.wait(event_list, 100)))
    	{
    	    gsys::System::usleep(100);
    	    continue;
    	}
    	
    	// handle epoll event
    	gsys::Epoll::EventList::const_iterator iter = event_list.begin();
    	for (; iter != event_list.end(); ++iter)
    	{
    	    if (iter->fd() == m_serverSocket.socket().sockfd())   	
    	    {
    	    	SockAddr client_addr;
    	    	GInt32 client_sockfd = -1;
    	        if (IS_NO(m_serverSocket.accept(client_addr, client_sockfd)))
    	        {
    	            continue;
    	        }
    	        
    	        G_LOG_INFO(LOG_PREFIX, "accept client address %s:%d", client_addr.ipStr(), client_addr.port());
    	        
    	        ClientAgent* client_agent = new ClientAgent(client_socket, client_addr);
    	        gsys::AutoLock auto_lock(m_clientMap.mutex());
    	        m_clientMap.insert(std::make_pair(client_sockfd, client_agent));
    	        
    	        // add to epoll
    	        m_epoll.addfd(client_sockfd);
    	    }
    	    else if (iter->isRecv())
    	    {
    	    	
    	    }
    	    else if (iter->isSend())
    	    {
    	    	
    	    }
    	}
    }
    
    return G_YES;
}

ClientAgent::ClientAgent() {}
ClientAgent::ClientAgent(const GInt32 sockfd, const SockAddr& client_addr) : m_sockfd(-1) {}
ClientAgent::~ClientAgent() {}

void ClientAgent::setSockfd(const GInt32 sockfd)
{
    m_sockfd = sockfd;	
}

GInt32 ClientAgent::sockfd() const
{
    return m_sockfd; 
}

void ClientAgent::setClientAddr(const SockAddr& client_addr)
{
    m_clientAddr = client_addr;	
}

SockAddr& ClientAgent::clientAddr()
{
    return m_clientAddr;
}

}

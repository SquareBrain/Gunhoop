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
#include <g_logger.h>
#include <g_tcp_server.h>
	
namespace gcom {

static const GInt8* G_LOG_PREFIX = "gohoop.gcom.network.tcpserver";

TcpServer::TcpServer() {}
TcpServer::TcpServer(const IPPortPair& server_addr, const std::string& net_card)
    : NetworkServer(server_addr, net_card) {}
TcpServer::~TcpServer() 
{
    stop();
    
    gsys::AutoLock auto_lock(m_clientMap.mutex());
    for (auto iter = m_clientMap.begin(); iter != m_clientMap.end(); ++iter)
    {
        delete iter->second;
    }
    m_clientMap.clear();
}
	  
GResult TcpServer::start()
{
    if (state() == G_SERVER_WORK)
    {
        return G_YES;
    }
    
    if (IS_NO(isValidServerAddr()))
    {
    	G_LOG_ERROR(G_LOG_PREFIX, "server address is inval");
    	return G_NO;
    }
    
    gsys::SocketInfo socket_info;
    socket_info.setProtocol(gsys::G_IPPROTO_TCP);
    socket_info.setServerIp(serverAddr().ipAddr().ip());
    socket_info.setServerPort(serverAddr().port());
    socket_info.setLocalIfName(netCard());
    
    if (IS_NO(m_socket.open(socket_info)))
    {
    	G_LOG_ERROR(G_LOG_PREFIX, "%s", m_socket.error());
    	return G_NO;
    }
    
    // init epoll
    if (IS_NO(m_epoll.open()))
    {
    	G_LOG_ERROR(G_LOG_PREFIX, "%s", m_epoll.error());
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
    m_epoll.addfd(m_socket.socket().sockfd());
    
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
    	
    	gsys::Epoll::EventList eventList;
    	if (IS_NO(m_epoll.wait(eventList, 100)))
    	{
    	    gsys::System::usleep(100);
    	    continue;
    	}
    	
    	// handle epoll event
    	for (auto iter = eventList.begin(); iter != eventList.end(); ++iter)
    	{
    	    if (iter->fd() == m_socket.socket().sockfd())   	
    	    {
    	    	gsys::SockAddr clientAddr;
    	    	GInt32 clientSockfd = -1;
    	        if (IS_NO(m_socket.accept(clientAddr, clientSockfd)))
    	        {
    	            continue;
    	        }
    	        
    	        G_LOG_DEBUG(G_LOG_PREFIX, "accept client address %s:%d", clientAddr.ipStr(), clientAddr.port());
    	        
                NetAddr addr;
                addr.setIPAddr(IPAddr(clientAddr.ip()));
                addr.setPort(clientAddr.port());
    	        ClientAgent* clientAgent = new ClientAgent(clientSockfd, addr);
    	        gsys::AutoLock autoLock(m_clientMap.mutex());
    	        m_clientMap.insert(std::make_pair(clientSockfd, clientAgent));
    	        
    	        // add to epoll
    	        m_epoll.addfd(clientSockfd);
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

}

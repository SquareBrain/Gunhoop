/*************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_socket.cpp
* @version     
* @brief      
* @author   duye
* @date	    2014-2-16
* @note 
*
*  2. 2014-06-21 duye move to gohoop 
*  1. 2014-02-16 duye Created this file
* 
*/
#include <g_sys.h>
#include <net/if.h>
#include <g_socket.h>

namespace gsys {

static const GInt8* LOG_PREFIX = "gohoop.gsys.socket";

const struct in6_addr IN6ADDR_ANY = IN6ADDR_ANY_INIT;

SockAddr::SockAddr() : SockAddr(INADDR_ANY, 0) {}
SockAddr::SockAddr(const GUint32 ip, const GUint16 port)
{
    m_addrLen = sizeof(sockaddr_in);
    bzero(&m_addr, m_addrLen);
    m_addr.sin_family = AF_INET; 
    m_addr.sin_port = htons(port);
    m_addr.sin_addr.s_addr = htonl(ip); 
}

SockAddr::~SockAddr() {}

void SockAddr::setIP(const GUint32 ip)
{
    m_addr.sin_addr.s_addr = htonl(ip);	
}

GUint32 SockAddr::ip()
{
    return ntohl(m_addr.sin_addr.s_addr);
}

GUint8* SockAddr::ipStr()
{
    return (GUint8*)inet_ntoa(m_addr.sin_addr);
}

void SockAddr::setPort(const GUint16 port)
{
    m_addr.sin_port = htons(port);
}

GUint16 SockAddr::port()
{
    return ntohs(m_addr.sin_port);
}

sockaddr_in& SockAddr::addr()
{
    return m_addr;
}

GUint16 SockAddr::addrLen() const
{
    return m_addrLen;
}

IPv6Addr::IPv6Addr()
{
    m_addrLen = sizeof(sockaddr_in6);
    bzero(&m_sockAddr, m_addrLen);
    m_sockAddr.sin6_family = AF_INET6; 
    m_sockAddr.sin6_port = 0; 
    m_sockAddr.sin6_addr = IN6ADDR_ANY;
}

IPv6Addr::IPv6Addr(const GUint8 ip[16], const GUint16 port)
{
    bzero(&m_sockAddr, sizeof(m_sockAddr));
    m_sockAddr.sin6_family = AF_INET6; 
    m_sockAddr.sin6_port = htons(port);
    memcpy(m_sockAddr.sin6_addr.s6_addr, ip, 16);
}

IPv6Addr::~IPv6Addr() {}

GUint8* IPv6Addr::ip()
{
    return m_sockAddr.sin6_addr.s6_addr;
}

GUint16 IPv6Addr::port()
{
    return ntohs(m_sockAddr.sin6_port);
}

sockaddr_in6& IPv6Addr::addr()
{
    return m_sockAddr;
}

GUint16 IPv6Addr::addrLen() const
{
    return m_addrLen;
}

Socket::Socket() : m_sockfd(-1), m_isInit(false) {}
Socket::~Socket() { close(); }

GResult Socket::open(const NetProtocol& protocol, const std::string& if_name)
{
    GInt32 domain = AF_INET;
    GInt32 sock_type = -1;
    GInt32 sock_protocol = -1;
    switch (protocol)
    {
    	case G_IPPROTO_TCP:
    	{
            sock_protocol = IPPROTO_TCP;
            sock_type = SOCK_STREAM;
            break;
    	}
    	case G_IPPROTO_UDP:
    	{
    	    sock_protocol = IPPROTO_UDP;
    	    sock_type = SOCK_DGRAM;
    	    break;
    	}
    	case G_IPPROTO_SCTP:
    	{
    	    sock_protocol = IPPROTO_SCTP;
    	    setError("[warn]%s:argument protocol(%d) not support (%s:%d)\n", __FUNCTION__, protocol, __FILE__, __LINE__);
    	    return G_NO; // not support
    	}
    	case G_IPPROTO_TIPC:
    	{
    	    //sock_protocol = IPPROTO_TIPC;
    	    setError("[warn]%s:argument protocol(%d) not support (%s:%d)\n", __FUNCTION__, protocol, __FILE__, __LINE__);
    	    return G_NO; // not support
    	}
    	default:
        {
    	    setError("[error]%s:argument protocol(%d) invalid (%s:%d)\n", __FUNCTION__, protocol, __FILE__, __LINE__);
    	    return G_NO;
        }
    }	
	
    m_sockfd = ::socket(domain, sock_type, sock_protocol);
    if (m_sockfd < 0)
    {
    	setError("[error]%s: socket(%d, %d, %d) ret=%d invalid (%s:%d)\n", 
    	    __FUNCTION__, domain, sock_type, sock_protocol, m_sockfd, __FILE__, __LINE__);
        return G_NO;
    }

	// set non-blocking
	//fcntl(m_sockfd, F_SETFL, O_NONBLOCK)

    // init socket option
    initOption(if_name);
    m_isInit = true;
    return G_YES;
}

GResult Socket::close(const GInt32 how)
{
    // SHUT_RD how = 0 : stop receive data
    // SHUT_WR how = 1 : stop send data
    // SHUT_RDWR how = 2 : both above way
    if (!m_isInit)
    {
    	return G_YES;
    }

    //return (shutdown(m_sockfd, how) == 0 ? G_YES : G_NO);
    return ::close(m_sockfd) == 0 ? G_YES : G_NO;
}

GInt32 Socket::sockfd() const
{
    return m_sockfd;	
}

GInt8* Socket::getError()
{
    return m_error;
}

GResult Socket::initOption(const std::string& if_name)
{
    GResult ret = G_YES;
    
    // setting specified interface
    struct ifreq interface;
    strncpy(interface.ifr_ifrn.ifrn_name, if_name.c_str(), if_name.length());
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_BINDTODEVICE, (char*)&interface, sizeof(interface)) == -1) 
    {
    	setError("[warn]%s:setsockopt() failed (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
    	ret = G_NO;        
    }    
    
    // set address reuse, address reuse flag, 1 reuse
    GInt32 reuse = 1;    
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(GInt32)) == -1)
    {
    	setError("[warn]%s:setsockopt() failed (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
    	ret = G_NO;
    }

    // send time limit, unit ms
    int send_time = 2000;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&send_time, sizeof(int)) == -1)
    {
    	 ret = false;
    }

    // receive time limit, unit ms
    int recv_time = 2000;    
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&recv_time, sizeof(int)) == -1)
    {
    	 ret = false;
    }

    // set send data buffer size 
    GInt32 send_buf_size = 0xFFFF;    
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDBUF, (const char*)&send_buf_size, sizeof(GInt32)) == -1)
    {
    	setError("[warn]%s:setsockopt() failed (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
    	ret = false;
    }

    // set receive data buffer size
    GInt32 recv_buf_size = 0xFFFF;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, (const char*)&recv_buf_size, sizeof(GInt32)) == -1)
    {
    	setError("[warn]%s:setsockopt() failed (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
    	ret = false;
    }

    // don't copy data from system buffer to socket buffer when send data
    /*
    GInt32 is_copy = 0;    
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDBUF, (const char*)&is_copy, sizeof(GInt32)) == -1)
    {
    	setError("[warn]%s:setsockopt() failed (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
    	ret = false;
    }

    // don't copy data from system buffer to GSocket buffer when receive data
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, (const char*)&is_copy, sizeof(GInt32)) == -1)
    {
    	setError("[warn]%s:setsockopt() failed (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
    	ret = false;
    }
    */

    // let data send completly after execute close GSocket
    /*
    struct STR_Linger
    {
    	GInt16 l_onoff;
    	GInt16 l_linger;
    };

    STR_Linger linger;
    linger.l_onoff = 1;		// 1. allow wait; 0. force close
    linger.l_linger = 1;	// the time of waiting unit s
    if (setsockopt(m_sockfd, SOL_GSocket, SO_LINGER, (const char*)&linger, sizeof(STR_Linger)) == -1)
    {
        ret = false;
    }
    */

    return ret;
}

void Socket::setError(const GInt8* args, ...)
{
    System::pformat(m_error, G_ERROR_BUF_SIZE, args);
}

GInt64 Transfer::send(Socket& socket, const GUint8* data, const GUint64 len, const GInt32 flags)
{
    return ::send(socket.sockfd(), data, len, flags);
}

GInt64 Transfer::sendmsg(Socket& socket, const struct msghdr* msg, const GInt32 flags)
{
    return ::sendmsg(socket.sockfd(), msg, flags);	
}

GInt64 Transfer::sendto(Socket& socket, SockAddr& dst_addr, const GUint8* data, const GUint64 len, const GInt32 flags)
{
    return ::sendto(socket.sockfd(), data, len, flags, (const struct sockaddr*)&dst_addr.addr(), dst_addr.addrLen());	
}

GInt64 Transfer::recv(Socket& socket, GUint8* buffer, const GUint64 size, const GInt32 flags)
{
    return ::recv(socket.sockfd(), buffer, size, flags);
}

GInt64 Transfer::recvmsg(Socket& socket, struct msghdr* msg, const GInt32 flags)
{
    return ::recvmsg(socket.sockfd(), msg, flags);	
}

GInt64 Transfer::recvfrom(Socket& socket, SockAddr& src_addr, GUint8* buffer, const GUint64 size, const GInt32 flags)
{
    GUint32 addr_len = src_addr.addrLen();
    return ::recvfrom(socket.sockfd(), buffer, size, flags, (struct sockaddr*)&src_addr.addr(), &addr_len);	
}

SocketServer::SocketServer() {}
SocketServer::~SocketServer() 
{
    close();
}

GResult SocketServer::bind(const SocketInfo& socket_info)
{
	if (IS_NO(m_socket.open(socket_info.protocol(), socket_info.localIfName())))
	{
		setError("[error]%s:Socket not init (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
		return G_NO;
	}

	m_socketInfo = socket_info;
    m_addr.setIP(m_socketInfo.serverIP());
    m_addr.setPort(m_socketInfo.serverPort());	
	
	return ::bind(m_socket.sockfd(), (const struct sockaddr*)&m_addr.addr(), m_addr.addrLen()) < 0 ? G_NO : G_YES;
}

GResult SocketServer::listen(const GUint32 max_connect_num)
{
    return ::listen(m_socket.sockfd(), max_connect_num) == 0 ? G_YES : G_NO;
}

GInt32 SocketServer::accept(SockAddr& client_addr, const RecvMode& mode)
{
    socklen_t addr_len = client_addr.addrLen();
    if (mode == G_RECV_BLOCK)
    {
        return ::accept(m_socket.sockfd(), (struct sockaddr*)&client_addr.addr(), &addr_len);	
    }
    else if (mode == G_RECV_NONBLOCK)
    {
    	return ::accept4(m_socket.sockfd(), (struct sockaddr*)&client_addr.addr(), &addr_len, SOCK_NONBLOCK);	
    }
    
    return -1;
}

GInt64 SocketServer::recvfrom(SockAddr& client_addr, GUint8* buffer, const GUint64 size, const RecvMode& mode)
{
    if (mode == G_RECV_BLOCK)
    {
    	return Transfer::recvfrom(m_socket, client_addr, buffer, size);
    }
    else if (mode == G_RECV_NONBLOCK)
    {
    	return Transfer::recvfrom(m_socket, client_addr, buffer, size, MSG_DONTWAIT);
    }
    
    return -1;
}

GResult SocketServer::close()
{
    return m_socket.close();    	
}

GInt8* SocketServer::getError()
{
    return m_error;
}

void SocketServer::setError(const GInt8* args, ...)
{
    System::pformat(m_error, G_ERROR_BUF_SIZE, args);
}
	
SocketClient::SocketClient() {}
SocketClient::~SocketClient() 
{
    close();
}

GResult SocketClient::connect(const SocketInfo& socket_info)
{
    if (IS_NO(m_socket.open(socket_info.protocol(), socket_info.localIfName())))
    {
    	setError("[error]%s:Socket not init (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
    	return G_NO;
    }

    m_socketInfo = socket_info;
    m_addr.setIP(m_socketInfo.serverIP());
    m_addr.setPort(m_socketInfo.serverPort()); 	
    
    return ::connect(m_socket.sockfd(), (const struct sockaddr*)&m_addr.addr(), m_addr.addrLen()) < 0 ? G_NO : G_YES;
}

GInt64 SocketClient::send(const GUint8* data, const GUint64 len, const GInt32 flags)
{
    return Transfer::send(m_socket, data, len, flags);	
}

GInt64 SocketClient::recv(GUint8* buffer, const GUint64 size, const RecvMode& mode)
{
    if (mode == G_RECV_BLOCK)
    {
    	return Transfer::recv(m_socket, buffer, size);
    }
    else if (mode == G_RECV_NONBLOCK)
    {
    	return Transfer::recv(m_socket, buffer, size, MSG_DONTWAIT);
    }
    
    return -1;	
}

GResult SocketClient::close()
{
    return m_socket.close();
}

GInt8* SocketClient::getError()
{
    return m_error;
}

void SocketClient::setError(const GInt8* args, ...)
{
    System::pformat(m_error, G_ERROR_BUF_SIZE, args);
}

Epoll::Epoll(const GUint32 max_event) : m_epollfd(-1), m_maxEvents(max_event)
{
    create();  
}

Epoll::~Epoll() 
{
    if (m_epollfd != -1)
    {
    	close(m_epollfd);
    }
}
	 
GResult Epoll::addfd(const GInt32 fd, const GUint32 events)
{
    IS_YES_RR((m_epollfd == -1, G_NO);
    
    struct epoll_event epoll_event;
    bzero(&epoll_event, sizeof(struct epoll_event));
    epoll_event.data.fd = fd;
    epoll_event.events = events;
    epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &epoll_event);
    
    return G_YES;
}

GResult Epoll::modfd(const GInt32 fd, const GUint32 events)
{
    IS_YES_RR((m_epollfd == -1, G_NO);
    
    struct epoll_event epoll_event;
    bzero(&epoll_event, sizeof(struct epoll_event));
    epoll_event.data.fd = fd;
    epoll_event.events = events;
    epoll_ctl(m_epollfd, EPOLL_CTL_MOD, fd, &epoll_event);
    
    return G_YES;
}
	 
GResult Epoll::delfd(const GInt32 fd)
{
    IS_YES_RR((m_epollfd == -1, G_NO);
    epoll_ctl(m_epollfd, EPOLL_CTL_DEL, fd, nullptr);
    return G_YES;
}

GResult Epoll::wait(std::list<Event>& event_list, const GUint32 timeout)
{
    IS_YES_RR(m_epollfd == -1, G_NO);
    struct epoll_event* events = calloc(m_maxEvents, sizeof(epoll_event));
    GInt ret = epoll_wait(m_epollfd, events, m_maxEvents, timeout);
    if (ret > 0)
    {
    	GUint32 event_num = ret;
    	for (GUint32 i = 0; i < evnet_num; i++)
    	{
            if ((events[i].events & EPOLLERR) ||  
                (events[i].events & EPOLLHUP))
            {
            	setError("epoll error, close fd");
            	
            	continue;
            }
            
    	}
    }
    
    return ret == -1 ? G_NO : G_YES;
}

GResult Epoll::create()
{
    m_epollfd = epoll_create(m_maxEvents);
    if (m_epollfd == -1)
    {
    	G_LOG_ERROR(LOG_PREFIX, "epoll_create failed");
    	return G_NO;
    }   
    
    return G_YES;
}
}

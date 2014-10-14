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
#include <g_socket.h>

namespace gsys {
	
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

GUint8* SockAddr::ipstr()
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
    m_sockAddr.sin6_family = AF_INET6; // IPv6
    m_sockAddr.sin6_port = 0; // port
    m_sockAddr.sin6_addr = IN6ADDR_ANY; // addr
}

IPv6Addr::IPv6Addr(const GUint8 ip[16], const GUint16 port/*0*/)
{
    bzero(&m_sockAddr, sizeof(m_sockAddr));
    m_sockAddr.sin6_family = AF_INET6; // IPv4
    m_sockAddr.sin6_port = htons(port); // port
    memcpy(m_sockAddr.sin6_addr.s6_addr, ip, 16); // addr 
}

IPv6Addr::~IPv6Addr() {}

GUint8* IPv6Addr::getIPStr()
{
    return m_sockAddr.sin6_addr.s6_addr;
}

GUint16 IPv6Addr::getPort()
{
    return ntohs(m_sockAddr.sin6_port);
}

sockaddr_in6& IPv6Addr::getSockAddr()
{
    return m_sockAddr;
}

GUint16 IPv6Addr::getAddrLen() const
{
    return m_addrLen;
}

Socket::Socket() : m_sockfd(-1), m_isInit(false) {}
Socket::~Socket() { uninit(); }

GResult Socket::init(const SockType& type, const NetProtocol& protocol, const std::string& if_name)
{
    GInt32 domain = AF_INET;
    
    /*
    switch (m_family)
    {
    	case G_AF_IPV4:
    	    domain = AF_INET;
    	    break;
    	case G_AF_IPV6:
    	    domain = AF_INET6;
    	    break;	
    	case G_AF_LOCAL:
    	    domain = AF_LOCAL;
    	    break;	
    	case G_AF_UNIX:
    	    domain = AF_UNIX;
    	    break;	
    	case G_AF_ROUTE:
    	    domain = AF_ROUTE;
    	    break;	
    	case G_AF_PACKET:
    	    domain = AF_PACKET;
    	    break;	
    	default:
    	    setError("[error]%s:argument family(%d) invalid (%s:%d)\n", __FUNCTION__, m_family, __FILE__, __LINE__);
    	    return G_NO;
    }
    */
	
    GInt32 sock_type = -1;
    switch (type)
    {
    	case G_SOCK_STREAM:
    	    sock_type = SOCK_STREAM;
    	    break;
    	case G_SOCK_DGRAM:
    	    sock_type = SOCK_DGRAM;
    	    break;	
    	case G_SOCK_SEQPACKET:
    	    sock_type = SOCK_SEQPACKET;
    	    break;	
    	case G_SOCK_RAW:
    	    sock_type = SOCK_RAW;
    	    break;
    	case G_SOCK_RDM:
    	    sock_type = SOCK_RDM;
    	    break;
    	case G_SOCK_PACKET:
    	    sock_type = SOCK_SEQPACKET;
    	    break;	
    	case G_SOCK_NONBLOCK:
    	    sock_type = SOCK_NONBLOCK;
    	    break;	
    	case G_SOCK_CLOEXEC:
    	    sock_type = SOCK_CLOEXEC;
    	    break;	
    	default:
    	    setError("[error]%s:argument type(%d) invalid (%s:%d)\n", __FUNCTION__, type, __FILE__, __LINE__);
    	    return G_NO;
    }
	
    GInt32 sock_protocol = -1;
    switch (protocol)
    {
    	case G_IPPROTO_TCP:
    	{
    	    if (type != G_SOCK_STREAM) 
            {
            	return G_NO;
            }
            sock_protocol = IPPROTO_TCP;
            break;
    	}
    	case G_IPPROTO_UDP:
    	{
    	    if (type != SOCK_DGRAM)
    	    {
    	    	return G_NO;
    	    }		
    	    sock_protocol = IPPROTO_TCP;
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

    // init socket option
    initOption(if_name);
    m_isInit = true;
    return G_YES;
}

GResult Socket::uninit(const GInt32 how)
{
    // how = 0 : stop receive data
    // how = 1 : stop send data
    // how = 2 : both above way
    if (!m_isInit)
    {
    	return G_YES;
    }

    return (shutdown(m_sockfd, how) == 0 ? G_YES : G_NO);
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

    // address reuse flag, 1 reuse
    GInt32 reuse = 1;
    // send time limit, unit ms
    //int stime = 1000;
    // receive time limit, unit ms
    //int rtime = 1000;
    // receive and send data buffer size
    GInt32 bufsize = 0xFFFF;
    // don't copy data from system buffer to GSocket buffer
    GInt32 nosize = 0;
    
    struct ifreq interface;
    strncpy(interface.ifr_ifrn.ifrn_name, if_name.c_str(), if_name.length());
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_BINDTODEVICE, (char *)&interface, sizeof(interface)) == -1) 
    {
    	setError("[warn]%s:setsockopt() failed (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
    	ret = false;        
    }    
    
    // set address reuse
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(GInt32)) == -1)
    {
    	setError("[warn]%s:setsockopt() failed (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
    	ret = false;
    }

    // set send data time limit
    //if (setsockopt(m_sockfd, SOL_GSocket, SO_SNDTIMEO, (const char*)&stime, sizeof(int)) == -1)
    //{
    //	 ret = false;
    //}

    // set receive data time limit
    //if (setsockopt(m_sockfd, SOL_GSocket, SO_RCVTIMEO, (const char*)&rtime, sizeof(int)) == -1)
    //{
    //	 ret = false;
    //}

    // set send data buffer size 
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDBUF, (const char*)&bufsize, sizeof(GInt32)) == -1)
    {
    	setError("[warn]%s:setsockopt() failed (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
    	ret = false;
    }

    // set receive data buffer size
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, (const char*)&bufsize, sizeof(GInt32)) == -1)
    {
    	setError("[warn]%s:setsockopt() failed (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
    	ret = false;
    }

    // don't copy data from system buffer to GSocket buffer when send data
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDBUF, (const char*)&nosize, sizeof(GInt32)) == -1)
    {
    	setError("[warn]%s:setsockopt() failed (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
    	ret = false;
    }

    // don't copy data from system buffer to GSocket buffer when receive data
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, (const char*)&nosize, sizeof(GInt32)) == -1)
    {
    	setError("[warn]%s:setsockopt() failed (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
    	ret = false;
    }

    // let data send completly after execute close GSocket
    struct STR_Linger
    {
    	GInt16 l_onoff;
    	GInt16 l_linger;
    };

    /*
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

void Transfer::setError(const GInt8* args, ...)
{
    System::pformat(m_error, G_ERROR_BUF_SIZE, args);
}

GInt64 Transfer::send(Socket& socket, const GUint8* data, const GUint64 len, const GInt32 flags/*MSG_NOSIGNAL*/)
{
    return ::send(socket.sockfd(), data, len, flags);
}

GInt64 Transfer::sendmsg(Socket& socket, const struct msghdr* msg, const GInt32 flags/*MSG_NOSIGNAL*/)
{
    return ::sendmsg(m_sockfd, msg, flags);	
}

GInt64 Transfer::sendto(Socket& socket, const SockAddr& dst_addr, const GUint8* data, const GUint64 len, const GInt32 flags)
{
    return ::sendto(socket.sockfd(), data, len, flags, (const struct sockaddr*)&dst_addr.addr(), dst_addr.addrLen());	
}

GInt64 Transfer::recv(Socket& socket, GUint8* buffer, const GUint64 size, const GInt32 flags)
{
    return ::recv(socket.sockfd(), buffer, size, flags);
}

GInt64 Transfer::recvmsg(Socket& socket, struct msghdr* msg, const GInt32 flags/*0*/)
{
    return ::recvmsg(socket.sockfd(), msg, flags);	
}

GInt64 Transfer::recvfrom(Socket& socket, SockAddr& src_addr, GUint8* buffer, const GUint64 size, const GInt32 flags/*0*/)
{
    GUint32 addr_len = 0;
    return ::recvfrom(socket.sockfd(), buffer, size, flags, (struct sockaddr*)&src_addr, &addr_len);	
}

SocketServer::SocketServer(const GUint32 server_ip, const GUint16 server_port) {}
SocketServer::~SocketServer() {}

GResult SocketServer::bind()
{
    if (!m_isInit)
    {
    	setError("[error]%s:Socket not init (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
    	return G_NO;
    }

    return ::bind(m_sockfd, (const struct sockaddr*)&m_addr.addr(), m_addr.getAddrLen()) < 0 ? G_NO : G_YES;
}

GResult SocketServer::listen(const GUint32 max_connect_num)
{
    if (!m_isInit)
    {
    	etError("[error]%s:Socket not init (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
    	return G_NO;
    }
	
    return ::listen(m_sockfd, max_connect_num) == 0 ? G_YES : G_NO;
}

GResult SocketServer::accept(SockAddr& client_addr)
{
    if (!m_isInit)
    {
    	setError("[error]%s:Socket not init (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
    	return G_NO;
    }

    GUint32 addr_len = 0;
    return ::accept(m_sockfd, (struct sockaddr*)&client_addr.addr(), &addr_len) < 0 ? G_NO : G_YES;
}

GInt64 SocketServer::recvfrom(IPv4Addr& client_addr, GUint8* buffer, const GUint64 size, const GInt32 flags/*0*/)
{
    return m_socket.recvfrom(client_addr.getSockAddr(), buffer, size, flags);	
}

GInt8* SocketServer::getError()
{
    return m_error;
}

void SocketServer::setError(const GInt8* args, ...)
{
    System::pformat(m_error, G_ERROR_BUF_SIZE, args);
}
	
SocketClient::SocketClient(const GUint32 server_ip, const GUint16 server_port) {}
SocketClient::~SocketClient() {}

const Socket& SocketClient::getSocket() const
{
    return m_socket;
}

GResult SocketClient::connect()
{
    if (!m_isInit)
    {
    	setError("[error]%s:Socket not init (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
        return G_NO;
    }

    return ::connect(m_sockfd, (const struct sockaddr*)&m_ipv4Addr.getSockAddr(), m_ipv4Addr.getAddrLen()) < 0 ? G_NO : G_YES;
}

GInt64 SocketClient::send(const GUint8* data, const GUint64 len, const GInt32 flags/*MSG_NOSIGNAL*/)
{
    return m_socket.send(data, len, flags);	
}

GInt64 SocketClient::recv(GUint8* buffer, const GUint64 size, const GInt32 flags/*0*/)
{
    return m_socket.recv(buffer, size, flags);	
}

GInt8* SocketClient::getError()
{
    return m_error;
}

void SocketClient::setError(const GInt8* args, ...)
{
    System::pformat(m_error, G_ERROR_BUF_SIZE, args);
}

}

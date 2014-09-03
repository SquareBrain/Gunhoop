/*************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_socket.cpp
* @version     
* @brief      
* @author	duye
* @date		2014-2-16
* @note 
*
*  2. 2014-06-21 duye move to gohoop 
*  1. 2014-02-16 duye Created this file
* 
*/
#include <g_socket.h>

namespace gsys {
	
const struct in6_addr IN6ADDR_ANY = IN6ADDR_ANY_INIT;

SockAddr::SockAddr() : m_addrLen(0) {}

SockEntity::~SockEntity() {}

GUint32 IPv4Entity::getAddrLen() const
{
	return m_addrLen;
}

const sockaddr_in* getSockAddr() const
{
	return nullptr;
}

const sockaddr_in6* getSockAddr() const
{
	return nullptr;
}
	
GUint32 SockEntity::getIP() const
{
	return 0;
}

GUint8* SockEntity::getIP() const
{
	return nullptr;
}

IPv4Addr::IPv4Addr() : IPv4Addr(INADDR_ANY, 0) {}
IPv4Addr::IPv4Addr(const GUint32 ip, const GUint16 port/*0*/)
{
	m_addrLen = sizeof(struct sockaddr_in);	
	bzero(&m_sockAddr, m_addrLen);
	
	m_sockAddr.sin_family = AF_INET;		// IPv4
	m_sockAddr.sin_port = htons(port);		// port
	m_sockAddr.sin_addr.s_addr = htonl(ip);		// IP
}

IPv4Addr::~IPv4Addr() {}

GUint32 IPv4Addr::getIP() const
{
	return ntohl(m_sockAddr.sin_addr.s_addr);
}

GUint8* IPv4Addr::getIP() const
{
	return inet_ntoa(m_sockAddr);
}

GUint16 IPv4Addr::getPort() const
{
	return ntohs(m_sockAddr.sin_port);
}

const sockaddr_in* IPv4Addr::getSockAddr() const
{
	return &m_sockAddr;
}

IPv6Addr::IPv6Addr()
{
	m_addrLen = sizeof(struct sockaddr_in6);
	bzero(&m_sockAddr, m_addrLen);
	
	m_sockAddr.sin6_family = AF_INET6;	// IPv6
	m_sockAddr.sin6_port = 0;			// port
	m_sockAddr.sin6_addr = IN6ADDR_ANY; // addr
}

IPv6Addr::IPv6Addr(const GUint8 ip[16], const GUint16 port/*0*/)
{
	m_addrLen = sizeof(struct sockaddr_in6);
	bzero(&m_sockAddr, m_addrLen);
	
	m_sockAddr.sin6_family = AF_INET6;		// IPv4
	m_sockAddr.sin6_port = htons(port);		// port
	memcpy(m_sockAddr.sin6_addr.s6_addr, ip, 16); // addr 
}

IPv6Addr::~IPv6Addr() {}

GUint8* IPv6Addr::getIP() const
{
	return m_sockAddr.sin6_addr.s6_addr;
}

GUint16 IPv6Addr::getPort() const
{
	return ntohs(m_sockAddr.sin6_port);
}

const sockaddr_in6* IPv6Addr::getSockAddr() const
{
	return &m_sockAddr;
}

Socket::Socket(const std::shared_ptr<SockAddr>& sockAddr) 
	: m_sockfd(-1), m_sockAddr(sockAddr), m_isInit(false) {}

Socket::~Socket() 
{
	delete m_sockAddr;
	m_sockAddr = nullptr;
}

GResult Socket::init(const AddrFamily& family, const SockType& type, const NetProtocol& protocol)
{
	GInt32 domain = -1;
	switch (family)
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
		return G_NO;
	}
	
	GInt32 sockType = -1;
	switch (type)
	{
	case G_SOCK_STREAM:
		sockType = SOCK_STREAM;
		break;
	case G_SOCK_DGRAM:
		sockType = SOCK_STREAM;
		break;	
	case G_SOCK_SEQPACKET:
		sockType = SOCK_STREAM;
		break;	
	case G_SOCK_RAW:
		sockType = SOCK_STREAM;
		break;
	case G_SOCK_RDM:
		sockType = SOCK_STREAM;
		break;
	case G_SOCK_PACKET:
		sockType = SOCK_STREAM;
		break;	
	case G_SOCK_NONBLOCK:
		sockType = SOCK_STREAM;
		break;	
	case G_SOCK_CLOEXEC:
		sockType = SOCK_STREAM;
		break;	
	default:
		return G_NO;
	}
	
	GInt32 sockProtocol = -1;
	switch (protocol)
	{
	case G_IPPROTO_TCP:
		{
			if (type != G_SOCK_STREAM)
			{
				return G_NO;
			}
			
			sockProtocol = IPPROTO_TCP;
		}
		break;
	case G_IPPROTO_UDP:
		{
			if (type != SOCK_DGRAM)
			{
				return G_NO;
			}		
			
			sockProtocol = IPPROTO_TCP;
		}
		break;		
	case G_IPPROTO_SCTP:
		sockProtocol = IPPROTO_SCTP;
		return G_NO; // not support
	case G_IPPROTO_TIPC:
		sockProtocol = IPPROTO_TIPC;
		return G_NO; // not support
	default:
		return G_NO;
	}	
	
	GInt32 m_sockfd = ::socket(domain, sockType, sockProtocol)
	if (m_sockfd < 0)
	{
		return G_NO;
	}

	// init socket option
	initOption();
	
	m_isInit = true;

	return G_YES;
}

GResult Socket::uninit(const GInt32 how/*0*/)
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

const std::shared_ptr<SockAddr>& Socket::getSockAddr() const
{
	return m_sockAddr;
}

GResult Socket::bind()
{
	if (!m_isInit)
	{
	    return -1;
	}
	
	return ::bind(m_sockfd, &m_sockAddr->getSockAddr() , m_sockAddr->getAddrLen()) < 0 ? G_NO : G_YES;
}

GResult Socket::listen(const GUint32 maxConnectNum/*20*/)
{
	if (!m_isInit)
	{
	    return -1;
	}
	
	return ::listen(m_sockfd, maxConnectNum) == 0 ? G_YES : G_NO;
}

GResult Socket::accept(sockaddr_in& clientAddr)
{
	if (!m_isInit)
	{
	    return -1;
	}
	
	return ::accept(m_sockfd, &clientAddr, sizeof(sockaddr_in)) < 0 ? G_NO : G_YES;
}

GResult Socket::accept(sockaddr_in6& clientAddr)
{
	if (!m_isInit)
	{
	    return -1;
	}
	
	return ::accept(m_sockfd, &clientAddr, sizeof(sockaddr_in6)) < 0 ? G_NO : G_YES;
}

GResult Socket::connect()
{
	
}

GInt64 Socket::send(const GUint8* data, const GUint64 len, const GInt32 flags/*MSG_NOSIGNAL*/)
{
	if (!m_isInit)
	{
	    return -1;
	}
	
	return ::send(m_sockfd, data, len, flags);
}

GInt64 Socket::sendmsg(const struct msghdr* msg, const GInt32 flags/*MSG_NOSIGNAL*/)
{
	if (!m_isInit)
	{
	    return -1;
	}
	
	return ::sendmsg(m_sockfd, msg, flags);	
}

GInt64 Socket::sendto(const sockaddr_in& dstAddr, const GUint8* data, const GUint64 len, const GInt32 flags/*MSG_NOSIGNAL*/)
{
	return ::send(m_sockfd, data, len, flags, &dstAddr, sizeof(sockaddr_in));	
}

GInt64 Socket::sendto(const sockaddr_in6& dstAddr, const GUint8* data, const GUint64 len, const GInt32 flags/*MSG_NOSIGNAL*/)
{
	return ::send(m_sockfd, data, len, flags, &dstAddr, sizeof(sockaddr_in6));	
}

GInt64 Socket::recv(GUint8* buffer, const GUint64 size, const GInt32 flags/*0*/)
{
	if (!m_isInit)
	{
	    return -1;
	}
	
	return ::recv(m_sockfd, buffer, size, flags);
}

GInt64 Socket::recvmsg(struct msghdr* msg, const GInt32 flags/*0*/)
{
	if (!m_isInit)
	{
	    return -1;
	}
	
	return ::recvmsg(m_sockfd, msg, flags);	
}

GInt64 Socket::recvfrom(sockaddr_in& srcAddr, GUint8* buffer, const GUint64 size, const GInt32 flags/*0*/)
{
	if (!m_isInit)
	{
	    return -1;
	}
	
	return ::recv(m_sockfd, buffer, size, flags, &srcAddr, sizeof(sockaddr_in));	
}

GResult Socket::initOption()
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

	// set address reuse
	if (setsockopt(m_sockEntity.getSockfd(), SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(GInt32)) == -1)
	{
	    //G_LOG_WARN(G_LOG_PREFIX, "set address reuse failed");
		ret = false;	
	}

	// set send data time limit
	//if (setsockopt(m_sockfd, SOL_GSocket, SO_SNDTIMEO, (const char*)&stime, sizeof(int)) == -1)
	//{
	//	ret = false;
	//}

	// set receive data time limit
	//if (setsockopt(m_sockfd, SOL_GSocket, SO_RCVTIMEO, (const char*)&rtime, sizeof(int)) == -1)
	//{
	//	ret = false;
	//}

	// set send data buffer size
	if (setsockopt(m_sockEntity.getSockfd(), SOL_SOCKET, SO_SNDBUF, (const char*)&bufsize, sizeof(GInt32)) == -1)
	{
		ret = false;
	}

	// set receive data buffer size
	if (setsockopt(m_sockEntity.getSockfd(), SOL_SOCKET, SO_RCVBUF, (const char*)&bufsize, sizeof(GInt32)) == -1)
	{
		ret = false;
	}

	// don't copy data from system buffer to GSocket buffer when send data
	if (setsockopt(m_sockEntity.getSockfd(), SOL_SOCKET, SO_SNDBUF, (const char*)&nosize, sizeof(GInt32)) == -1)
	{
		ret = false;
	}

	// don't copy data from system buffer to GSocket buffer when receive data
	if (setsockopt(m_sockEntity.getSockfd(), SOL_SOCKET, SO_RCVBUF, (const char*)&nosize, sizeof(GInt32)) == -1)
	{
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

ServerSocket::ServerSocket(const std::shared_ptr<Socket>& socket) : m_socket(socket) {}
ServerSocket::~ServerSocket() 
{
	delete m_socket;
	m_socket = nullptr;
}

const std::shared_ptr<Socket>& ServerSocket::getSocket() const
{
	return m_socket;
}

GResult ServerSocket::bind()
{
	if (m_socket == nullptr)
	{
		return G_NO;
	}
	
	return m_socket->bind();
}

GResult ServerSocket::listen(const GUint32 maxConnectNum/*20*/)
{
	if (m_socket == nullptr)
	{
		return G_NO;
	}
	
	return m_socket->listen(maxConnectNum);
}

GResult ServerSocket::accept(std::shared_ptr<SockAddr>& cliAddr)
{
	if (cliAddr->get() == nullptr)
	{
		return G_NO;	
	}
	
	if (m_socket == nullptr)
	{
		return G_NO;
	}
	
	return m_socket->accept(*cliAddr->getSockAddr());
}

GInt64 ServerSocket::recvfrom(std::shared_ptr<SockAddr>& cliAddr, GUint8* buffer, const GUint64 size, const GInt32 flags/*0*/)
{
	if (cliAddr->get() == nullptr)
	{
		return G_NO;	
	}
	
	if (m_socket == nullptr)
	{
		return G_NO;
	}
	
	return m_socket->recvfrom(*cliAddr->getSockAddr(), buffer, size, flags);	
}
	
ClientSocket::ClientSocket(const std::shared_ptr<Socket>& socket) : m_socket(socket) {}
ClientSocket::~ClientSocket() 
{
	delete m_socket;
	m_socket = nullptr;
}

const std::shared_ptr<Socket>& ClientSocket::getSocket() const
{
	return m_socket;
}

GResult ClientSocket::connect()
{
	if (m_socket == nullptr)
	{
		return G_NO;
	}
	
	return m_socket->connect();
}

GInt64 ClientSocket::send(const GUint8* data, const GUint64 len, const GInt32 flags/*MSG_NOSIGNAL*/)
{
	if (m_socket == nullptr)
	{
		return G_NO;
	}
	
	return m_socket->send(data, len, flags);	
}

GInt64 ClientSocket::recv(GUint8* buffer, const GUint64 size, const GInt32 flags/*0*/)
{
	if (m_socket == nullptr)
	{
		return G_NO;
	}
	
	return m_socket->recv(buffer, size, flags);	
}
}

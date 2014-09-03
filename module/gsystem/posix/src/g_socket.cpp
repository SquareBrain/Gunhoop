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

IPv4Entity::IPv4Entity() : IPv4Entity(INADDR_ANY, 0) {}

IPv4Entity::IPv4Entity(const GUint32 ip, const GUint16 port/*0*/) : m_sockfd(-1), m_addrLen(0)
{
	m_addrLen = sizeof(struct sockaddr_in);
	bzero(&m_sockAddr, m_addrLen);
	
	m_sockAddr.sin_family = AF_INET;		// IPv4
	m_sockAddr.sin_port = htons(port);		// port
	m_sockAddr.sin_addr.s_addr = htonl(ip);		// IP
}

IPv4Entity::~IPv4Entity()
{
}

GUint32 IPv4Entity::getIP() const
{
	return ntohl(m_sockAddr.sin_addr.s_addr);
}

GUint16 IPv4Entity::getPort() const
{
	return ntohs(m_sockAddr.sin_port);
}

void IPv4Entity::setSockfd(const GInt32 sockfd) const
{
	m_sockfd = sockfd;
}

GInt32 IPv4Entity::getSockfd() const
{
	return m_sockfd;
}

const sockaddr_in& IPv4Entity::getSockAddr() const
{
	return m_sockAddr;
}

socklen_t& IPv4Entity::getAddrLen() const
{
	return m_addrLen;
}

IPv6Entity::IPv6Entity() : m_sockfd(-1), m_addrLen(0)
{
	m_addrLen = sizeof(struct sockaddr_in6);
	bzero(&m_sockAddr, m_addrLen);
	
	m_sockAddr.sin6_family = AF_INET6;	// IPv6
	m_sockAddr.sin6_port = 0;			// port
	m_sockAddr.sin6_addr = IN6ADDR_ANY; // addr
}

IPv6Entity::IPv6Entity(const GUint8* ip, const GUint16 port/*0*/) : m_sockfd(-1), m_addrLen(0)
{
	m_addrLen = sizeof(struct sockaddr_in6);
	bzero(&m_sockAddr, m_addrLen);
	
	m_sockAddr.sin6_family = AF_INET6;		// IPv4
	m_sockAddr.sin6_port = htons(port);		// port
	memcpy(m_sockAddr.sin6_addr.s6_addr, ip, 16); // addr 
}

IPv6Entity::~IPv6Entity()
{
}

GUint8* IPv6Entity::getIP() const
{
	return m_sockAddr.sin6_addr.s6_addr;
}

GUint16 IPv6Entity::getPort() const
{
	return ntohs(m_sockAddr.sin6_port);
}

void IPv6Entity::setSockfd(const GInt32 sockfd) const
{
	m_sockfd = sockfd;
}

GInt32 IPv6Entity::getSockfd() const
{
	return m_sockfd;
}

const sockaddr_in6& IPv6Entity::getSockAddr() const
{
	return m_sockAddr;
}

socklen_t& IPv6Entity::getAddrLen() const
{
	return m_addrLen;
}

Socket::Socket() : m_isInit(false) {}
Socket::Socket(const GUint32 ip, const GUint16 port/*0*/) : m_sockEntity(ip, port), m_isInit(false) {}
Socket::~Socket() {}

GUint32 Socket::getIP() const
{
	return m_sockEntity.getIP();
}

GUint32 Socket::getPort() const
{
	m_sockEntity.getPort();
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
	
	GInt32 sockfd = ::socket(domain, sockType, sockProtocol)
	if (sockfd < 0)
	{
		return G_NO;
	}
	
	m_sockEntify.setSockfd(sockfd);

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

	return (shutdown(m_sockEntity.getSockfd(), how) == 0 ? G_YES : G_NO);
}

GInt64 Socket::send(const GUint8* data, const GUint64 len, const GInt32 flags)
{
	if (!m_isInit)
	{
	    return -1;
	}
	
	return ::send(m_sockEntity.getSockfd(), data, len, flags);
}

GInt64 Socket::recv(GUint8* buffer, const GUint64 size, const GInt32 flags)
{
	if (!m_isInit)
	{
	    return -1;
	}
	
	return ::recv(m_sockEntity.getSockfd(), buffer, size, flags);
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

ServerSocket::ServerSocket() : Socket() {}
ServerSocket::ServerSocket(const GUint32 ip, const GUint16 port/*0*/) : Socket(ip, port) {}
ServerSocket::~ServerSocket() {}

GResult ServerSocket::bind(m_sockEntity.getSockfd())
{
	if (!m_isInit)
	{
		return G_NO;
	}
	
	return ::bind(m_sockEntity.getSockfd(), &m_sockEntity.getSockAddr(), m_sockEntity.getAddrLen()) < 0 ? G_NO : G_YES;
}

GResult ServerSocket::listen(const GUint32 maxConnectNum/*20*/)
{
	if (!m_isInit)
	{
		return G_NO;
	}
	
	return ::listen(m_sockEntity.getSockfd(), maxConnectNum) == 0 ? G_YES : G_NO;
}

GResult ServerSocket::accept(sockaddr_in& clientSockAddr)
{
	if (!m_isInit)
	{
		return G_NO;
	}
	
	return ::accept(m_sockEntity.getSockfd(), &clientSockAddr, m_sockEntity.getAddrLen()) < 0 ? G_NO : G_YES;
}

ClientSocket::ClientSocket() : Socket() {}
ClientSocket::ClientSocket(const GUint32 ip, const GUint16 port/*0*/) : Socket(ip, port) {} 
ClientSocket::~ClientSocket() {}

GResult ClientSocket::connect()
{
	if (!m_isInit)
	{
		return G_NO;
	}
	
	return ::connect(m_sockEntity.getSockfd(), m_sockEntity.getSockAddr(), m_sockEntity.getAddrLen()) < 0 ? G_NO : G_YES;
}
}

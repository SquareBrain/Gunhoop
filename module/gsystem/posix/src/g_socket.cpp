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

SockEntity::SockEntity() : SockEntity(INADDR_ANY, 0) {}

SockEntity::SockEntity(const GUint32 ip, const GUint16 port/*0*/) : m_sockfd(-1), m_addrLen(0)
{
	bzero(m_sockAddr, sizeof(sockaddr_in));	
	m_sockAddr.sin_family = AF_INET;		// RF_INET
	m_sockAddr.sin_port = htons(port);		// port
	m_sockAddr.sin_addr.s_addr = ip;		// IP
	memset(&(m_sockAddr.sin_zero), 0, 8);		// set 0
	m_addrLen = sizeof(struct sockaddr);
}

SockEntity::~SockEntity()
{
}

GUint32 SockEntity::getIP() const
{
	return ntohl(m_addr.sin_addr.s_addr);
}

GUint16 SockEntity::getPort() const
{
	return ntohs(m_addr.sin_port);
}

void SockEntity::setSockfd(const GInt32 sockfd) const
{
	m_sockfd = sockfd;
}

GInt32 SockEntity::getSockfd() const
{
	return m_sockfd;
}

const sockaddr_in& SockEntity::getSockAddr() const
{
	return m_sockAddr;
}

socklen_t& SockEntity::getAddrLen() const
{
	return m_addrLen;
}

Socket::Socket() : m_sockEntity(), m_isInit(false) {}
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

GResult Socket::init(const GInt32 domain/*AF_INET*/, const GInt32 type/*SOCK_STREAM*/, const GInt32 protocol/*0*/)
{
	switch (domain)
	{
	case AF_INET:
	case AF_INET6:
	case AF_LOCAL:
	case AF_UNIX:
	case AF_ROUTE:
		break;
	default:
		return G_NO;
	}
	
	switch (type)
	{
	case SOCK_STREAM: // TCP
	case SOCK_DGRAM: // UDP
	case SOCK_RAW:
	case SOCK_PACKET:
	case SOCK_SEQPACKET:
		break;
	default:
		return G_NO;
	}
	
	if (protocol != 0)
	{
		switch (protocol)
		{
		case IPPROTO_TCP:
		case IPPROTO_UDP:
		case IPPROTO_SCTP:
		case IPPROTO_TIPC:
			break;
		default:
			return G_NO;
		}
	}
	
	if (domain == SOCK_STREAM && protocol != IPPROTO_TCP)
	{
		return G_NO;
	}

	if (domain == SOCK_DGRAM && protocol != IPPROTO_UDP)
	{
		return G_NO;
	}	
	
	GInt32 sockfd = ::socket(domain, type, protocol)
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

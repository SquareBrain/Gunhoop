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

IPv4Addr::IPv4Addr() : IPv4Addr(INADDR_ANY, 0) {}
IPv4Addr::IPv4Addr(const GUint32 ip, const GUint16 port/*0*/)
{
	m_addrLen = sizeof(sockaddr_in);
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

GUint8* IPv4Addr::getIPStr() const
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

GUint16 IPv4Addr::getAddrLen() const
{
	return m_addrLen;
}

IPv6Addr::IPv6Addr()
{
	m_addrLen = sizeof(sockaddr_in6);
	bzero(&m_sockAddr, m_addrLen);
	m_sockAddr.sin6_family = AF_INET6;	// IPv6
	m_sockAddr.sin6_port = 0;			// port
	m_sockAddr.sin6_addr = IN6ADDR_ANY; // addr
}

IPv6Addr::IPv6Addr(const GUint8 ip[16], const GUint16 port/*0*/)
{
	bzero(&m_sockAddr, sizeof(m_sockAddr));
	m_sockAddr.sin6_family = AF_INET6;		// IPv4
	m_sockAddr.sin6_port = htons(port);		// port
	memcpy(m_sockAddr.sin6_addr.s6_addr, ip, 16); // addr 
}

IPv6Addr::~IPv6Addr() {}

GUint8* IPv6Addr::getIPStr() const
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

GUint16 IPv6Addr::getAddrLen() const
{
	return m_addrLen;
}

Socket::Socket(const std::shared_ptr<IPv4Addr>& sock_addr) 
	: m_sockfd(-1), m_ipv4Addr(sock_addr), m_isInit(false), m_family(G_AF_IPV4) {}

Socket::Socket(const std::shared_ptr<IPv6Addr>& sock_addr) 
	: m_sockfd(-1), m_ipv6Addr(sock_addr), m_isInit(false), m_family(G_AF_IPV6) {}

Socket::~Socket() 
{
	delete m_ipv4Addr;
	m_ipv4Addr = nullptr;

	delete m_ipv6Addr;
	m_ipv6Addr = nullptr;
}

GResult Socket::init(const SockType& type, const NetProtocol& protocol)
{
	GInt32 domain = -1;
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
		setError("[error]%s:argument family(%d) invalid (%s:%d)\n", __FUNCTION__, family, __FILE__, __LINE__);
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
		setError("[error]%s:argument type(%d) invalid (%s:%d)\n", __FUNCTION__, type, __FILE__, __LINE__);
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
		setError("[warn]%s:argument protocol(%d) not support (%s:%d)\n", __FUNCTION__, protocol, __FILE__, __LINE__);
		return G_NO; // not support
	case G_IPPROTO_TIPC:
		sockProtocol = IPPROTO_TIPC;
		setError("[warn]%s:argument protocol(%d) not support (%s:%d)\n", __FUNCTION__, protocol, __FILE__, __LINE__);
		return G_NO; // not support
	default:
		setError("[error]%s:argument protocol(%d) invalid (%s:%d)\n", __FUNCTION__, protocol, __FILE__, __LINE__);
		return G_NO;
	}	
	
	GInt32 m_sockfd = ::socket(domain, sockType, sockProtocol)
	if (m_sockfd < 0)
	{
		setError("[error]%s: socket(%d, %d, %d) ret=%d invalid (%s:%d)\n", 
			__FUNCTION__, domain, sockType, sockProtocol, m_sockfd, __FILE__, __LINE__);
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

const std::shared_ptr<IPv4Addr>& Socket::getIPv4Addr() const
{
	return m_ipv4Addr;
}

const std::shared_ptr<IPv6Addr>& Socket::getIPv6Addr() const
{
	return m_ipv6Addr;
}

GResult Socket::bind()
{
	if (!m_isInit)
	{
		setError("[error]%s:Socket not init (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
	    return -1;
	}

	switch (m_family)
	{
	case G_AF_IPV4:
		return ::bind(m_sockfd, m_ipv4Addr->getSockAddr(), m_ipv4Addr->getAddrLen()) < 0 ? G_NO : G_YES;
		break;
	case G_AF_IPV6:
		return ::bind(m_sockfd, (sockaddr_in*)(m_ipv6Addr->getSockAddr()), m_ipv6Addr->getAddrLen()) < 0 ? G_NO : G_YES;
		break;
	default:
		break;
	}

	return G_NO;
}

GResult Socket::listen(const GUint32 max_connect_num/*20*/)
{
	if (!m_isInit)
	{
		setError("[error]%s:Socket not init (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
	    return -1;
	}
	
	return ::listen(m_sockfd, max_connect_num) == 0 ? G_YES : G_NO;
}

GResult Socket::accept(sockaddr_in& client_addr)
{
	if (!m_isInit)
	{
		setError("[error]%s:Socket not init (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
	    return -1;
	}
	
	return ::accept(m_sockfd, &client_addr, sizeof(sockaddr_in)) < 0 ? G_NO : G_YES;
}

GResult Socket::accept(sockaddr_in6& client_addr)
{
	if (!m_isInit)
	{
		setError("[error]%s:Socket not init (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
	    return -1;
	}
	
	return ::accept(m_sockfd, &client_addr, sizeof(sockaddr_in6)) < 0 ? G_NO : G_YES;
}

GResult Socket::connect()
{
	if (!m_isInit)
	{
		setError("[error]%s:Socket not init (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
	    return -1;
	}
	
	return ::connect(m_sockfd, m_sockAddr->getSockAddr() , m_sockAddr->getAddrLen()) < 0 ? G_NO : G_YES;	
}

GInt64 Socket::send(const GUint8* data, const GUint64 len, const GInt32 flags/*MSG_NOSIGNAL*/)
{
	if (!m_isInit)
	{
		setError("[error]%s:Socket not init (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
	    return -1;
	}
	
	return ::send(m_sockfd, data, len, flags);
}

GInt64 Socket::sendmsg(const struct msghdr* msg, const GInt32 flags/*MSG_NOSIGNAL*/)
{
	if (!m_isInit)
	{
		setError("[error]%s:Socket not init (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
	    return -1;
	}
	
	return ::sendmsg(m_sockfd, msg, flags);	
}

GInt64 Socket::sendto(const sockaddr_in& dst_addr, const GUint8* data, const GUint64 len, const GInt32 flags/*MSG_NOSIGNAL*/)
{
	if (!m_isInit)
	{
		setError("[error]%s:Socket not init (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
	    return -1;
	}
	
	return ::send(m_sockfd, data, len, flags, &dst_addr, sizeof(sockaddr_in));	
}

GInt64 Socket::sendto(const sockaddr_in6& dst_addr, const GUint8* data, const GUint64 len, const GInt32 flags/*MSG_NOSIGNAL*/)
{
	if (!m_isInit)
	{
		setError("[error]%s:Socket not init (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
	    return -1;
	}
	
	return ::send(m_sockfd, data, len, flags, &dst_addr, sizeof(sockaddr_in6));	
}

GInt64 Socket::recv(GUint8* buffer, const GUint64 size, const GInt32 flags/*0*/)
{
	if (!m_isInit)
	{
		setError("[error]%s:Socket not init (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
	    return -1;
	}
	
	return ::recv(m_sockfd, buffer, size, flags);
}

GInt64 Socket::recvmsg(struct msghdr* msg, const GInt32 flags/*0*/)
{
	if (!m_isInit)
	{
		setError("[error]%s:Socket not init (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
	    return -1;
	}
	
	return ::recvmsg(m_sockfd, msg, flags);	
}

GInt64 Socket::recvfrom(sockaddr_in& src_addr, GUint8* buffer, const GUint64 size, const GInt32 flags/*0*/)
{
	if (!m_isInit)
	{
		setError("[error]%s:Socket not init (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
	    return -1;
	}
	
	return ::recv(m_sockfd, buffer, size, flags, &src_addr, sizeof(sockaddr_in));	
}

GInt8* Socket::getError()
{
	return m_error;
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
	    setError("[warn]%s:setsockopt() failed (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
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
		setError("[warn]%s:setsockopt() failed (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
		ret = false;
	}

	// set receive data buffer size
	if (setsockopt(m_sockEntity.getSockfd(), SOL_SOCKET, SO_RCVBUF, (const char*)&bufsize, sizeof(GInt32)) == -1)
	{
		setError("[warn]%s:setsockopt() failed (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
		ret = false;
	}

	// don't copy data from system buffer to GSocket buffer when send data
	if (setsockopt(m_sockEntity.getSockfd(), SOL_SOCKET, SO_SNDBUF, (const char*)&nosize, sizeof(GInt32)) == -1)
	{
		setError("[warn]%s:setsockopt() failed (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
		ret = false;
	}

	// don't copy data from system buffer to GSocket buffer when receive data
	if (setsockopt(m_sockEntity.getSockfd(), SOL_SOCKET, SO_RCVBUF, (const char*)&nosize, sizeof(GInt32)) == -1)
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

void Socket::setError(const GInt8* args, ...)
{
	System::pformat(m_error, G_ERROR_BUF_SIZE, args);
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
		setError("[error]%s:m_socket == nullptr (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
		return G_NO;
	}
	
	return m_socket->bind();
}

GResult ServerSocket::listen(const GUint32 max_connect_num/*20*/)
{
	if (m_socket == nullptr)
	{
		setError("[error]%s:m_socket == nullptr (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
		return G_NO;
	}
	
	return m_socket->listen(max_connect_num);
}

GResult ServerSocket::accept(std::shared_ptr<IPv4Addr>& client_addr)
{
	if (client_addr->get() == nullptr)
	{
		return G_NO;	
	}
	
	if (m_socket == nullptr)
	{
		setError("[error]%s:m_socket == nullptr (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
		return G_NO;
	}
	
	return m_socket->accept(client_addr->getSockAddr());
}

GInt64 ServerSocket::recvfrom(std::shared_ptr<IPv4Addr>& client_addr, GUint8* buffer, const GUint64 size, const GInt32 flags/*0*/)
{
	if (cliAddr->get() == nullptr)
	{
		setError("[error]%s:cliAddr->get() == nullptr (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
		return G_NO;	
	}
	
	if (m_socket == nullptr)
	{
		setError("[error]%s:m_socket == nullptr (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
		return G_NO;
	}
	
	return m_socket->recvfrom(client_addr->getSockAddr(), buffer, size, flags);	
}

GInt8* ServerSocket::getError()
{
	return m_error;
}

void ServerSocket::setError(const GInt8* args, ...)
{
	System::pformat(m_error, G_ERROR_BUF_SIZE, args);
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
		setError("[error]%s:m_socket == nullptr (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
		return G_NO;
	}
	
	return m_socket->connect();
}

GInt64 ClientSocket::send(const GUint8* data, const GUint64 len, const GInt32 flags/*MSG_NOSIGNAL*/)
{
	if (m_socket == nullptr)
	{
		setError("[error]%s:m_socket == nullptr (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
		return G_NO;
	}
	
	return m_socket->send(data, len, flags);	
}

GInt64 ClientSocket::recv(GUint8* buffer, const GUint64 size, const GInt32 flags/*0*/)
{
	if (m_socket == nullptr)
	{
		setError("[error]%s:m_socket == nullptr (%s:%d)\n", __FUNCTION__, __FILE__, __LINE__);
		return G_NO;
	}
	
	return m_socket->recv(buffer, size, flags);	
}

GInt8* ClientSocket::getError()
{
	return m_error;
}

void ClientSocket::setError(const GInt8* args, ...)
{
	System::pformat(m_error, G_ERROR_BUF_SIZE, args);
}
}

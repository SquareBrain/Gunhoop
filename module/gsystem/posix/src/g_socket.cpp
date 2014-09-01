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

SockEntity::SockEntity() : m_ip(0), m_port(0), m_sockfd(-1), m_sockLen(0)
{
	bzero(m_sockAddr, sizeof(sockaddr_in));	
}

SockEntity::~SockEntity()
{
}

void SockEntity::setIP(const GUint32 ip)
{
	m_ip = ip;
}

GUint32 SockEntity::getIP() const
{
	return m_ip;
}

void SockEntity::setPort(const GUint32 port) const
{
	m_port = port;	
}

GUint32 SockEntity::getPort() const
{
	return m_port;
}


void SockEntity::setSockfd(const GInt32 sockfd) const
{
	m_sockfd = sockfd;
}

GInt32 SockEntity::getSockfd() const
{
	return m_sockfd;
}

void SockEntity::setSockAddr(const sockaddr_in sockaddr) const
{
	m_sockAddr = sockAddr;
}

sockaddr_in SockEntity::getSockAddr() const
{
	return m_sockAddr;
}

void SockEntity::setSockLen(const socklen_t sockLen) const
{
	m_sockLen = sockLen;
}

socklen_t SockEntity::getSockLen() const
{
	return m_sockLen;
}

SockTransfer::SockTransfer() {}

SockTransfer~SockTransfer() {}

GInt64 SockTransfer::send(const SockEntity& sockEntity, 
	const GUint8* data, 
	const GUint64 len, 
	const GInt32 flags)
{
	return ::send(sockEntity.getSockfd(), data, len, flags);
}

GInt64 SockTransfer::recv(const SockEntity& sockEntity, 
	GUint8* buffer, 
	const GUint64 size, 
	const GInt32 flags)
{
	return ::recv(sockEntity.getSockfd(), buffer, size, flags);
}


ServerSocket::ServerSocket() : ServerSocket(0, 0) {}
ServerSocket::ServerSocket(const GUint32 ip, const GUint16 port)
{
	m_sockEntity.setIP(ip);
	m_sockEntity.setPort(ip);
}

ServerSocket::~ServerSocket() {}

void ServerSocket::setIP(const GUint32 ip)
{
	m_sockEntity.setIP(ip);
}

GUint32 ServerSocket::getIP() const
{
	return m_sockEntity.getIP();
}

void ServerSocket::setPort(const GUint32 port) const
{
	m_sockEntity.setPort(port);	
}

GUint32 ServerSocket::getPort() const
{
	m_sockEntity.getPort();
}
	
GResult ServerSocket::init(const GInt32 domain/*AF_INET*/, const GInt32 type/*SOCK_STREAM*/)
{
	return G_YES;
}

GResult ServerSocket::uninit(const GInt32 how/*0*/)
{
	return G_YES;
}

GResult ServerSocket::bind()
{
	return G_YES;
}

GResult ServerSocket::listen()
{
	return G_YES;
}

GResult ServerSocket::accept()
{
	return G_YES;
}

ClientSocket::ClientSocket() : ClientSocket(0, 0) {}
ClientSocket::ClientSocket(const GUint32 ip, const GUint16 port) 
{
	m_sockEntity.setIP(ip);
	m_sockEntity.setPort(ip);	
}

ClientSocket::~ClientSocket() {}

void ClientSocket::setIP(const GUint32 ip)
{
	m_sockEntity.setIP(ip);
}

GUint32 ClientSocket::getIP() const
{
	return m_sockEntity.getIP();
}

void ClientSocket::setPort(const GUint32 port) const
{
	m_sockEntity.setPort(port);	
}

GUint32 ClientSocket::getPort() const
{
	m_sockEntity.getPort();
}

GResult ClientSocket::init(const GInt32 domain/*AF_INET*/, const GInt32 type/*SOCK_STREAM*/)
{
	GInt31 sockfd = socket(domain, type, 0)
	if (sockfd < 0)
	{
		return G_NO;
	}
	
	m_sockEntify.setSockfd(sockfd);

	// init socket option
	initOption();

	return G_YES;
}

GResult ClientSocket::uninit(const GInt32 how/*0*/)
{
	// how = 0 : stop receive data
	// how = 1 : stop send data
	// how = 2 : both above way
	if (m_sockEntity.getSockfd() < -1)
	{
	    return G_YES;
	}

	return (shutdown(m_sockfd, how) == 0 ? G_YES : G_NO);
}

GResult ClientSocket::connect()
{
	return G_YES;
}
	
GInt64 ClientSocket::send(const GUint8* data, const GUint64 len, const GInt32 flags/*MSG_NOSIGNAL*/)
{
	return SockTransfer::send(m_sockEntify.getSockfd(), data, len, flags);
}

GInt64 ClientSocket::recv(GUint8* buffer, const GUint64 size, const GInt32 flags/*0*/)
{
	return SockTransfer::recv(m_sockEntify.getSockfd(), buffer, size, flags);
}

Socket::Socket() : m_sockfd(-1), m_addrLen(0)
{
}

Socket::Socket(const GUint32 ip, const GUint16 port) 
    : m_sockfd(-1), m_addrLen(0)
{
	setAddr(ip, port);        
}

Socket::Socket(const Socket& Socket)
{
	m_sockfd = Socket.m_sockfd;
	m_addr = Socket.m_addr;
}

Socket::~Socket()
{
	closeSocket(m_sockfd);
}

GResult Socket::openSocket(const GInt32 domain, const GInt32 type)
{
	if ((m_sockfd = socket(domain, type, 0)) == -1)
	{
		//G_LOG_ERROR(G_LOG_PREFIX, "open GSocket() failed");
		return G_NO;
	}

	// init GSocket option
	if (!initOption())
	{
	    //G_LOG_ERROR(G_LOG_PREFIX, "init GSocket option failed");
	}

	return G_YES;
}

GInt64 Socket::sendData(const GUint8* data, const GUint64 length, const GInt32 flags)
{
	return send(m_sockfd, data, length, flags);
}

GInt64 Socket::recvData(GUint8* buffer, const GUint64 size, const GInt32 flags)
{
	return recv(m_sockfd, buffer, size, flags);
}

GResult Socket::closeSocket(const GInt32 how)
{
	// how = 0 : stop receive data
	// how = 1 : stop send data
	// how = 2 : both above way
	if (m_sockfd == -1)
	{
	    //G_LOG_ERROR(G_LOG_PREFIX, "GSocket hasn't open");
	    return G_YES;
	}

	return (shutdown(m_sockfd, how) == 0 ? G_YES : G_NO);
}

void Socket::setAddr(const GUint32 ip, const GUint16 port)
{
	m_addr.sin_family = AF_INET;		// RF_INET
	m_addr.sin_port = htons(port);		// port
	m_addr.sin_addr.s_addr = ip;		// IP
	memset(&(m_addr.sin_zero),'\0', 8);		// set 0
	m_addrLen = sizeof(struct sockaddr);
}

GUint32 Socket::getIP() const
{
	return ntohl(m_addr.sin_addr.s_addr);
}

GUint16 Socket::getPort() const
{
	return ntohs(m_addr.sin_port);
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
	if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(GInt32)) == -1)
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
	if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDBUF, (const char*)&bufsize, sizeof(GInt32)) == -1)
	{
		ret = false;
	}

	// set receive data buffer size
	if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, (const char*)&bufsize, sizeof(GInt32)) == -1)
	{
		ret = false;
	}

	// don't copy data from system buffer to GSocket buffer when send data
	if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDBUF, (const char*)&nosize, sizeof(GInt32)) == -1)
	{
		ret = false;
	}

	// don't copy data from system buffer to GSocket buffer when receive data
	if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, (const char*)&nosize, sizeof(GInt32)) == -1)
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
}

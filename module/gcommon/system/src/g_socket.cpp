/*************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_socket.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-2-16
* @note 
*
*  2. 2014-06-21 duye move to gohoop 
*  1. 2014-02-16 duye Created this file
* 
*/

#include <socket.h>

static const GInt8* LOG_PREFIX = "gohoop.gcommon.system.socket";

G_NS_GCOMMON_BEG 

Socket::Socket() : m_sockfd(0), m_addrLen(0)
{
}

Socket::Socket(const GUint32 ip, const GUint16 port) : m_sockfd(0), m_addrLen(0)
{
    SetAddr(ip, port);        
}

Socket::Socket(const GUint8* ip, const GUint16 port) : m_sockfd(0), m_addrLen(0)
{
}

Socket::Socket(const Socket& socket)
{
    m_sockfd = socket.m_sockfd;
    m_addr = socket.m_addr;
    
	//SetSockfd(socket.GetSockfd());
	//SetAddr(socket.GetAddr());
}

Socket::~Socket()
{
	Shutdown(m_sockfd);
}

bool Socket::InitSocket(const GInt32 domain, const GInt32 type)
{
	m_sockfd = socket(domain, type, 0);

	if (m_sockfd != -1)
	{
		// init socket option
		if (!InitOption())
		{
			POSIX_WARN(LOG_PREFIX, "CWSocket : Init socket option failed \n");
		}
	}

	return m_sockfd != -1;
}

GInt32 Socket::Send(const GUint8* msg, const GUint32 msgLen, const GInt32 flags)
{
	return send(m_sockfd, msg, msgLen, flags);
}

GInt32 Socket::Recv(GUint8** buf, const GUint32 bufLen, const GInt32 flags)
{
	return recv(m_sockfd, *buf, bufLen, flags);
}

bool Socket::Shutdown(const GInt32 how)
{
	// how = 0 : stop receive data
	// how = 1 : stop send data
	// how = 2 : both above way
	return shutdown(m_sockfd, how) == 0;
}

void Socket::SetAddr(const GUint32 ip, const GUint16 port)
{
	m_addr.sin_family = AF_INET;		// RF_INET
	m_addr.sin_port = htons(port);		// port
	m_addr.sin_addr.s_addr = ip;		// IP
	memset(&(m_addr.sin_zero),'\0', 8);		// set 0
	m_addrLen = sizeof(struct sockaddr);
}

void Socket::SetAddr(const GUint8* ip, const GUint16 port)
{
	m_addr.sin_family = AF_INET;		// RF_INET
	m_addr.sin_port = htons(port);		// port
	m_addr.sin_addr.s_addr = inet_addr((const char*)ip);		// IP
	bzero(&(m_addr.sin_zero), 8);		// set 0
	m_addrLen = sizeof(struct sockaddr);
}

void Socket::SetAddr(sockaddr_in addr)
{
	m_addr.sin_family = addr.sin_family;				// RF_INET
	m_addr.sin_port = addr.sin_port;					// port
	m_addr.sin_addr.s_addr = addr.sin_addr.s_addr;		// IP
	bzero(&(m_addr.sin_zero), 8);						// set 0
	m_addrLen = sizeof(struct sockaddr);
}

sockaddr_in Socket::GetAddr() const
{
	return m_addr;
}

GUint32 Socket::GetIP() const
{
	return ntohl(m_addr.sin_addr.s_addr);
}

GUint16 Socket::GetPort() const
{
	return ntohs(m_addr.sin_port);
}

bool Socket::InitOption()
{
	bool ret = true;

	// address reuse flag, 1 reuse
	GInt32 reuse = 1;
	// send time limit, unit ms
	//int stime = 1000;
	// receive time limit, unit ms
	//int rtime = 1000;
	// receive and send data buffer size
	GInt32 bufsize = 0xFFFF;
	// don't copy data from system buffer to socket buffer
	GInt32 nosize = 0;

	// set address reuse
	if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(GInt32)) == -1)
	{
		ret = false;	
	}
	
	// set send data time limit
	//if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&stime, sizeof(int)) == -1)
	//{
	//	ret = false;
	//}

	// set receive data time limit
	//if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&rtime, sizeof(int)) == -1)
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
	
	// don't copy data from system buffer to socket buffer when send data
	if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDBUF, (const char*)&nosize, sizeof(GInt32)) == -1)
	{
		ret = false;
	}

	// don't copy data from system buffer to socket buffer when receive data
	if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, (const char*)&nosize, sizeof(GInt32)) == -1)
	{
		ret = false;
	}

	// let data send completly after execute close socket
	struct STR_Linger
	{
		GInt16 l_onoff;
		GInt16	l_linger;
	};

	/*
	STR_Linger linger;
	linger.l_onoff = 1;		// 1. allow wait; 0. force close
	linger.l_linger = 1;	// the time of waiting unit s
	if (setsockopt(m_sockfd, SOL_SOCKET, SO_LINGER, (const char*)&linger, sizeof(STR_Linger)) == -1)
	{
		ret = false;
	}
	*/

	return ret;
}

G_NS_GCOMMON_END

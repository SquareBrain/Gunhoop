/*************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     duye_socket.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-2-16
* @note 
*
*  1. 2014-2-16 duye Created this file
* 
*/

#include <duye/posix/socket/inc/duye_socket.h>

static const D_Int8* LOG_PREFIX = "posix.socket";

DUYE_POSIX_NS_BEG 

Socket::Socket() : m_sockfd(0), m_addrLen(0)
{
}

Socket::Socket(const D_UInt32 ip, const D_UInt16 port) : m_sockfd(0), m_addrLen(0)
{
    SetAddr(ip, port);        
}

Socket::Socket(const D_UInt8* ip, const D_UInt16 port) : m_sockfd(0), m_addrLen(0)
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

D_Bool Socket::InitSocket(const D_Int32 domain, const D_Int32 type)
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

D_Int32 Socket::Send(const D_UInt8* msg, const D_UInt32 msgLen, const D_Int32 flags)
{
	return send(m_sockfd, msg, msgLen, flags);
}

D_Int32 Socket::Recv(D_UInt8** buf, const D_UInt32 bufLen, const D_Int32 flags)
{
	return recv(m_sockfd, *buf, bufLen, flags);
}

D_Bool Socket::Shutdown(const D_Int32 how)
{
	// how = 0 : stop receive data
	// how = 1 : stop send data
	// how = 2 : both above way
	return shutdown(m_sockfd, how) == 0;
}

void Socket::SetAddr(const D_UInt32 ip, const D_UInt16 port)
{
	m_addr.sin_family = AF_INET;		// RF_INET
	m_addr.sin_port = htons(port);		// port
	m_addr.sin_addr.s_addr = ip;		// IP
	memset(&(m_addr.sin_zero),'\0', 8);		// set 0
	m_addrLen = sizeof(struct sockaddr);
}

void Socket::SetAddr(const D_UInt8* ip, const D_UInt16 port)
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

D_UInt32 Socket::GetIP() const
{
	return ntohl(m_addr.sin_addr.s_addr);
}

D_UInt16 Socket::GetPort() const
{
	return ntohs(m_addr.sin_port);
}

D_Bool Socket::InitOption()
{
	D_Bool ret = true;

	// address reuse flag, 1 reuse
	D_Int32 reuse = 1;
	// send time limit, unit ms
	//int stime = 1000;
	// receive time limit, unit ms
	//int rtime = 1000;
	// receive and send data buffer size
	D_Int32 bufsize = 0xFFFF;
	// don't copy data from system buffer to socket buffer
	D_Int32 nosize = 0;

	// set address reuse
	if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(D_Int32)) == -1)
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
	if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDBUF, (const char*)&bufsize, sizeof(D_Int32)) == -1)
	{
		ret = false;
	}

	// set receive data buffer size
	if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, (const char*)&bufsize, sizeof(D_Int32)) == -1)
	{
		ret = false;
	}
	
	// don't copy data from system buffer to socket buffer when send data
	if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDBUF, (const char*)&nosize, sizeof(D_Int32)) == -1)
	{
		ret = false;
	}

	// don't copy data from system buffer to socket buffer when receive data
	if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, (const char*)&nosize, sizeof(D_Int32)) == -1)
	{
		ret = false;
	}

	// let data send completly after execute close socket
	struct STR_Linger
	{
		D_Int16 l_onoff;
		D_Int16	l_linger;
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

DUYE_POSIX_NS_END

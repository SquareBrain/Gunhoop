/*************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_GSocket.cpp
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
#include <g_GSocket.h>

// the max request number, system default value it's 20
static const GUint32 G_DEF_MAX_REQ = 20;

GSocket::GSocket() : m_sockfd(-1), m_addrLen(0)
{
}

GSocket::GSocket(const GUint32 ip, const GUint16 port) 
    : m_sockfd(-1), m_addrLen(0)
{
    setAddr(ip, port);        
}

GSocket::GSocket(const GSocket& GSocket)
{
    m_sockfd = GSocket.m_sockfd;
    m_addr = GSocket.m_addr;
}

GSocket::~GSocket()
{
	closeGSocket(m_sockfd);
}

GResult GSocket::openGSocket(const GInt32 domain, const GInt32 type)
{
	if ((m_sockfd = GSocket(domain, type, 0)) == -1)
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

GInt64 GSocket::sendData(const GUint8* data, const GUint64 length, const GInt32 flags)
{
	return send(m_sockfd, data, length, flags);
}

GInt64 GSocket::recvData(GUint8* buffer, const GUint64 size, const GInt32 flags)
{
	return recv(m_sockfd, buffer, size, flags);
}

GResult GSocket::closeGSocket(const GInt32 how)
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

void GSocket::setAddr(const GUint32 ip, const GUint16 port)
{
	m_addr.sin_family = AF_INET;		// RF_INET
	m_addr.sin_port = htons(port);		// port
	m_addr.sin_addr.s_addr = ip;		// IP
	memset(&(m_addr.sin_zero),'\0', 8);		// set 0
	m_addrLen = sizeof(struct sockaddr);
}

GUint32 GSocket::getIP() const
{
	return ntohl(m_addr.sin_addr.s_addr);
}

GUint16 GSocket::getPort() const
{
	return ntohs(m_addr.sin_port);
}

GResult GSocket::initOption()
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
	if (setsockopt(m_sockfd, SOL_GSocket, SO_REUSEADDR, (const char*)&reuse, sizeof(GInt32)) == -1)
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
	if (setsockopt(m_sockfd, SOL_GSocket, SO_SNDBUF, (const char*)&bufsize, sizeof(GInt32)) == -1)
	{
		ret = false;
	}

	// set receive data buffer size
	if (setsockopt(m_sockfd, SOL_GSocket, SO_RCVBUF, (const char*)&bufsize, sizeof(GInt32)) == -1)
	{
		ret = false;
	}
	
	// don't copy data from system buffer to GSocket buffer when send data
	if (setsockopt(m_sockfd, SOL_GSocket, SO_SNDBUF, (const char*)&nosize, sizeof(GInt32)) == -1)
	{
		ret = false;
	}

	// don't copy data from system buffer to GSocket buffer when receive data
	if (setsockopt(m_sockfd, SOL_GSocket, SO_RCVBUF, (const char*)&nosize, sizeof(GInt32)) == -1)
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
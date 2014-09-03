/*************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_socket.h
* @version     
* @brief      
* @author	duye
* @date		2014-02-16
* @note 
*
*  2. 2014-06-20 duye move to gohoop project 
*  1. 2014-02-16 duye Created this file
* 
*/

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <string>
#include <g_type.h>

namespace gsys {
	
typedef GUint32 GAddrIPv4;
typedef GUint8* GAddrIPv6;

/** 
 * @brief address family
 */
typedef enum 
{
	// AF_INET
	G_AF_IPV4,
	// AF_INET6
	G_AF_IPV6,
	G_AF_LOCAL,
	G_AF_UNIX,
	G_AF_ROUTE,
	G_AF_PACKET
} AddrFamily;

/** 
 * @brief transfer type
 */
typedef enum 
{
	// tcp
	G_SOCK_STREAM,
	// udp
	G_SOCK_DGRAM,
	G_SOCK_SEQPACKET,
	G_SOCK_RAW,
	G_SOCK_RDM,
	G_SOCK_PACKET,
	G_SOCK_NONBLOCK,
	G_SOCK_CLOEXEC
} SockType;

/** 
 * @brief network protocol
 */
typedef enum 
{
	// SOCK_STREAM
	G_IPPROTO_TCP,
	// SOCK_DGRAM
	G_IPPROTO_UDP,
	G_IPPROTO_SCTP,
	G_IPPROTO_TIPC
} NetProtocol;
	
class SockEntify
{
public:
	SockEntify();
	virtual ~SockEntify();
	
	/**
	 * @brief set/get socket fd
	 * @return 
	 */		
	void setSockfd(const GInt32 sockfd) const;
	GInt32 getSockfd() const;	
	
	/**
	 * @brief set/get addr length
	 * @return 
	 */		
	GUint32 getAddrLen() const;	
	
	/**
	 * @brief set/get IP address
	 * @return
	 */		
	virtual GUint32 getIP() const;
	virtual GUint8* getIP() const;

	/**
	 * @brief set/get port
	 * @return 
	 */		
	virtual GUint32 getPort() const = 0;
	
protected:
	// socket file descrition
	GInt32			m_sockfd;	
	// sock length
	GUint32			m_addrLen;	
};

/** 
 * @brief IPv4 
 */
class IPv4Entity : public SockEntify
{
public:
	/**
	 * auto get local address, and rand setting a port
	 */
	IPv4Entity();
	explicit IPv4Entity(const GUint32 ip, const GUint16 port = 0);
	~IPv4Entity();
	
	/**
	 * @brief set/get IP address
	 * @return
	 */		
	GUint32 getIP() const;
	GUint8* getIP() const;

	/**
	 * @brief set/get port
	 * @return 
	 */		
	GUint32 getPort() const;
	
	/**
	 * @brief set/get sock addr
	 * @return 
	 */		
	const sockaddr_in& getSockAddr() const;	
	
private:
	// address
	sockaddr_in		m_sockAddr;
};

/** 
 * @brief IPv6
 */
class IPv6Entity : public SockEntify
{
public:
	/**
	 * auto get local address, and rand setting a port
	 */
	IPv6Entity();
	explicit IPv6Entity(const GUint8* ip, const GUint16 port = 0);
	~IPv6Entity();
	
	/**
	 * @brief set/get IP address
	 * @return
	 */		
	GUint8* getIP() const;

	/**
	 * @brief set/get port
	 * @return 
	 */		
	GUint32 getPort() const;
	
	/**
	 * @brief set/get sock addr
	 * @return 
	 */		
	const sockaddr_in6& getSockAddr() const;	
	
private:
	// address
	sockaddr_in6	m_sockAddr;
};

/** 
 * @brief server socket class
 */
class Socket
{
public:
	explicit Socket(const AddrFamily& addrFamily);
	explicit Socket(const GAddrIPv4& ip, const GUint16 port = 0);
	explicit Socket(const GAddrIPv6& ip, const GUint16 port = 0);
	~Socket();
	
	/**
	 * @brief set/get IP address
	 * @return
	 */		
	GUint32 getIP() const;
	GUint8* getIPv6() const;

	/**
	 * @brief set/get port
	 * @return 
	 */		
	GUint32 getPort() const;	
	
	/**
	 * @brief init socket
	 * @return G_YES/G_NO
	 * @note 
	 */		
	GResult init(const AddrFamily& family, const SockType& type, const NetProtocol& protocol);
	
	/**
	 * @brief shutdown connecting
	 * @param [in] how : way
	 * @return G_YES/G_NO
	 * @note 
	 */	
	GResult uninit(const GInt32 how = 0);	
	
	/**
	 * @brief send data
	 * @param [in] data : send data
	 * @param [in] len : data length
	 * @param [in] flags : flags
	 * @return size/-1
	 * @note 
	 */		
	GInt64 send(const GUint8* data, const GUint64 len, const GInt32 flags = MSG_NOSIGNAL);
	
	/**
	 * @brief receive data
	 * @param [out] buffer : output buffer
	 * @param [in] bufferSize : buffer size
	 * @param [in] flags : flags
	 * @return size/-1
	 * @note 
	 */	
	GInt64 recv(GUint8* buffer, const GUint64 size, const GInt32 flags = 0);	
	
private:
	/**
	 * @brief setting socket options 
	 * @return G_YES/G_NO
	 */		
	GResult initOption();
	
private:
	AddrFamily		m_addrFamily;
	SockEntify*		m_sockEntity;
	bool			m_isInit;
};
	
/** 
 * @brief server socket class
 */
class ServerSocket
{
public:
	/**
	 * @brief auto find location host
	 */
	explicit ServerSocket(Socket* socket);
	~ServerSocket();
	
	/**
	 * @brief bind address and port
	 * @return G_YES/G_NO
	 * @note 
	 */	
	GResult bind();	
	
	/**
	 * @brief listen port
	 * @param [in] maxConnectNum : the max client connect number, default is 20
	 * @return G_YES/G_NO
	 * @note 
	 */	
	GResult listen(const GUint32 maxConnectNum = 20);	
	
	/**
	 * @brief waitting for connect
	 * @param [in] clientSockAddr : client sock address
	 * @return G_YES/G_NO
	 * @note 
	 */	
	GResult accept(sockaddr_in& clientSockAddr);
	
	/**
	 * @brief send data
	 * @param [in] data : send data
	 * @param [in] len : data length
	 * @param [in] flags : flags
	 * @return size/-1
	 * @note 
	 */		
	GInt64 send(const GUint8* data, const GUint64 len, const GInt32 flags = MSG_NOSIGNAL);
	
	/**
	 * @brief receive data
	 * @param [out] buffer : output buffer
	 * @param [in] bufferSize : buffer size
	 * @param [in] flags : flags
	 * @return size/-1
	 * @note 
	 */	
	GInt64 recv(GUint8* buffer, const GUint64 size, const GInt32 flags = 0);
	
private:
	Socket* 	m_socket;
};

/** 
 * @brief client socket class
 */
class ClientSocket
{
public:
	/**
	 * @brief connect location server
	 */		
	ClientSocket(Socket* socket);
	~ClientSocket();
	
	/**
	 * @brief connect socket
	 * @return G_YES/G_NO
	 * @note 
	 */		
	GResult connect();
	
	/**
	 * @brief send data
	 * @param [in] data : send data
	 * @param [in] len : data length
	 * @param [in] flags : flags
	 * @return size/-1
	 * @note 
	 */		
	GInt64 send(const GUint8* data, const GUint64 len, const GInt32 flags = MSG_NOSIGNAL);
	
	/**
	 * @brief receive data
	 * @param [out] buffer : output buffer
	 * @param [in] bufferSize : buffer size
	 * @param [in] flags : flags
	 * @return size/-1
	 * @note 
	 */	
	GInt64 recv(GUint8* buffer, const GUint64 size, const GInt32 flags = 0);
	
private:
	Socket* 	m_socket;	
};
}

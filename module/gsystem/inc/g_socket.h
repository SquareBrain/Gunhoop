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
	
/** 
 * @brief socket base class
 */
class SockEntity
{
public:
	SockEntity();
	~SockEntity();
	
	/**
	 * @brief set/get IP address
	 * @return
	 */		
	void setIP(const GUint32 ip);
	GUint32 getIP() const;

	/**
	 * @brief set/get port
	 * @return 
	 */		
	void setPort(const GUint32 port) const;
	GUint32 getPort() const;
	
	/**
	 * @brief set/get socket fd
	 * @return 
	 */		
	void setSockfd(const GInt32 sockfd) const;
	GInt32 getSockfd() const;
	
	/**
	 * @brief set/get sockaddr
	 * @return 
	 */		
	void setSockAddr(const sockaddr_in sockaddr) const;
	sockaddr_in getSockAddr() const;	
	
	/**
	 * @brief set/get address length
	 * @return 
	 */		
	void setSockLen(const socklen_t sockLen) const;
	socklen_t getSockLen() const;	
	
private:
	GUint32			m_ip;
	GUint16			m_port;
	// socket file descrition
	GInt32			m_sockfd;
	// address
	sockaddr_in		m_sockAddr;
	// sock length
	socklen_t		m_sockLen;	
};

class SockTransfer
{
public:
	SockTransfer();
	~SockTransfer();
	
	/**
	 * @brief send data
	 * @param [in] sockEntity : SockEntity
	 * @param [in] data : send data
	 * @param [in] dataLen : data length
	 * @param [in] flags : flags
	 * @return size/-1
	 * @note 
	 */		
	static GInt64 send(const SockEntity& sockEntity, 
		const GUint8* data, 
		const GUint64 len, 
		const GInt32 flags);
	
	/**
	 * @brief receive data
	 * @param [in] sockEntity : SockEntity
	 * @param [out] buffer : output buffer
	 * @param [in] bufferSize : buffer size
	 * @param [in] flags : flags
	 * @return size/-1
	 * @note 
	 */	
	static GInt64 recv(const SockEntity& sockEntity, 
		GUint8* buffer, 
		const GUint64 size, 
		const GInt32 flags);	
};
	
/** 
 * @brief server socket class
 */
class ServerSocket
{
public:
	ServerSocket();
	
	/**
	 * @brief constructor
	 * @param [in] ip : ip address
	 * @param [in] port : port
	 * @note 
	 */			
	 explicit ServerSocket(const GUint32 ip, const GUint16 port);
	 
	~ServerSocket();
	
	/**
	 * @brief set/get IP address
	 * @return
	 */		
	void setIP(const GUint32 ip);
	GUint32 getIP() const;

	/**
	 * @brief set/get port
	 * @return 
	 */		
	void setPort(const GUint32 port) const;
	GUint32 getPort() const;	
	
	/**
	 * @brief init socket
	 * @return G_YES/G_NO
	 * @note 
	 */		
	GResult init(const GInt32 domain = AF_INET, const GInt32 type = SOCK_STREAM);
	
	/**
	 * @brief shutdown connecting
	 * @param [in] how : way
	 * @return G_YES/G_NO
	 * @note 
	 */	
	GResult uninit(const GInt32 how = 0);	
	
	/**
	 * @brief bind address and port
	 * @return G_YES/G_NO
	 * @note 
	 */	
	GResult bind();	
	
	/**
	 * @brief listen port
	 * @return G_YES/G_NO
	 * @note 
	 */	
	GResult listen();	
	
	/**
	 * @brief waitting for connect
	 * @return G_YES/G_NO
	 * @note 
	 */	
	GResult accept();	
	
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
	SockEntity	m_sockEntity;
};

/** 
 * @brief client socket class
 */
class ClientSocket
{
public:
	ClientSocket();

	/**
	 * @brief constructor
	 * @param [in] ip : ip address
	 * @param [in] port : port
	 * @note 
	 */			
	explicit ClientSocket(const GUint32 ip, const GUint16 port);
	~ClientSocket();
	
	/**
	 * @brief set/get IP address
	 * @return
	 */		
	void setIP(const GUint32 ip);
	GUint32 getIP() const;

	/**
	 * @brief set/get port
	 * @return 
	 */		
	void setPort(const GUint32 port) const;
	GUint32 getPort() const;

	/**
	 * @brief init socket
	 * @param [in] domain : domain
	 * @param [in] type : socket type
	 * @return G_YES/G_NO
	 * @note 
	 */		
	GResult init(const GInt32 domain = AF_INET, const GInt32 type = SOCK_STREAM);
	
	/**
	 * @brief shutdown connecting
	 * @param [in] how : way
	 * @return G_YES/G_NO
	 * @note 
	 */	
	GResult uninit(const GInt32 how = 0);	
	
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
	/**
	 * @brief setting socket options 
	 * @return G_YES/G_NO
	 */		
	GResult initOption();

private:
	SocketEntity	m_socketEntity;				
};
}

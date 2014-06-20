/*************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     socket.h
* @version     
* @brief      
* @author   duye
* @date     2014-02-16
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

G_NS_GCOMMON_BEG 

// brief : posix socket wrapper class
class Socket
{
public:
    // the max request number, system default value it's 20
    static const GUint32 MAX_REQ_NUM = 20;
    
public:
	Socket();

	// brief : constructor
	// @para [in]ip : ip address
	// @para [in]port : port
	// note		
	Socket(const GUint32 ip, const GUint16 port);
	Socket(const GUint8* ip, const GUint16 port);

	// brief : copy constructor
	// @para [in]socket : Socket
	// note	
	Socket(const Socket& socket);
	
	~Socket();
	
	// brief : init socket
	// @para [in]domain : domain
	// @para [in]type : socket type
	// return : true/false
	// note	
	bool InitSocket(const GInt32 domain = AF_INET, const GInt32 type = SOCK_STREAM/*SOCK_DGRAM*/);
	
	// brief : send data
	// @para [in]data : send data
	// @para [in]dataLen : data length
	// @para [in]flags : flags
	// return : the size of send, failure return -1
	// note		
	GInt32 Send(const GUint8* data, const GUint32 dataLen, const GInt32 flags = MSG_NOSIGNAL);

	// brief : receive data
	// @para [out]buffer : output buffer
	// @para [in]bufferSize : buffer size
	// @para [in]flags : flags
	// return : the size of received, failure return -1
	// note		
	GInt32 Recv(GUint8** buffer, const GUint32 bufferSize, const GInt32 flags = 0);

	// brief : shutdown connecting 
	// @para [in]how : way
	// return : ture/false
	// note		
	bool Shutdown(const GInt32 how = 0);
	
	// brief : setting address
	// @para [in]ip : ip address
	// @para [in]port : port
	// note		
	void SetAddr(const GUint32 ip, const GUint16 port);	
	void SetAddr(const GUint8* ip, const GUint16 port);

	// brief : get ip address
	// return : ip address
	// note			
	GUint32 GetIP() const;
	std::string GetIPString() const;

	// brief : get port
	// return : port
	// note			
	GUint16 GetPort() const;

	// brief : setting address
	// @para [in]addr : address
	// note
	void SetAddr(sockaddr_in addr);

	// brief : get address
	// return : address
	// note		
	sockaddr_in GetAddr() const;

private:
	// brief : setting socket options 
	// return : true/false
	// note			
	bool InitOption();

private:
    // socket file descrition
	GInt32         m_sockfd;
	// address
	sockaddr_in		m_addr;
	// address length
	socklen_t		m_addrLen;			
};

G_NS_GCOMMON_END

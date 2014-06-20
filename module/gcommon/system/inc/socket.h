/*************************************************************************************
**  
*    @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     duye_socket.h
* @version     
* @brief      
* @author   duye
* @date     2014-2-16
* @note 
*
*  1. 2014-2-16 duye Created this file
* 
*/

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <string>

#include <duye/posix/inc/duye_posix_def.h>

DUYE_POSIX_NS_BEG 

// brief : posix socket wrapper class
class Socket
{
public:
    // the max request number, system default value it's 20
    static const D_UInt32 MAX_REQ_NUM = 20;
    
public:
	Socket();

	// brief : constructor
	// @para [in]ip : ip address
	// @para [in]port : port
	// note		
	Socket(const D_UInt32 ip, const D_UInt16 port);
	Socket(const D_UInt8* ip, const D_UInt16 port);

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
	D_Bool InitSocket(const D_Int32 domain = AF_INET, const D_Int32 type = SOCK_STREAM/*SOCK_DGRAM*/);
	
	// brief : send data
	// @para [in]data : send data
	// @para [in]dataLen : data length
	// @para [in]flags : flags
	// return : the size of send, failure return -1
	// note		
	D_Int32 Send(const D_UInt8* data, const D_UInt32 dataLen, const D_Int32 flags = MSG_NOSIGNAL);

	// brief : receive data
	// @para [out]buffer : output buffer
	// @para [in]bufferSize : buffer size
	// @para [in]flags : flags
	// return : the size of received, failure return -1
	// note		
	D_Int32 Recv(D_UInt8** buffer, const D_UInt32 bufferSize, const D_Int32 flags = 0);

	// brief : shutdown connecting 
	// @para [in]how : way
	// return : ture/false
	// note		
	D_Bool Shutdown(const D_Int32 how = 0);
	
	// brief : setting address
	// @para [in]ip : ip address
	// @para [in]port : port
	// note		
	void SetAddr(const D_UInt32 ip, const D_UInt16 port);	
	void SetAddr(const D_UInt8* ip, const D_UInt16 port);

	// brief : get ip address
	// return : ip address
	// note			
	D_UInt32 GetIP() const;
	std::string GetIPString() const;

	// brief : get port
	// return : port
	// note			
	D_UInt16 GetPort() const;

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
	D_Bool InitOption();

private:
    // socket file descrition
	D_Int32         m_sockfd;
	// address
	sockaddr_in		m_addr;
	// address length
	socklen_t		m_addrLen;			
};

DUYE_POSIX_NS_END

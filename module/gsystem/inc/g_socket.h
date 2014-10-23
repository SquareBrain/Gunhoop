/*************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_socket.h
* @version     
* @brief      
* @author   duye
* @date	    2014-02-16
* @note 
*
*  2. 2014-06-20 duye move to gohoop project 
*  1. 2014-02-16 duye Created this file
* 
*/
#pragma once

#include <sys/socket.h>
#include <sys/epoll.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <memory>
#include <g_result.h>
#include <g_type.h>

namespace gsys {

/** 
 * @brief address family
 */
/*
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
*/

/** 
 * @brief transfer type
 */
/*
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
*/

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

/** 
 * @brief network receive data mode
 */
typedef enum 
{
    G_RECV_BLOCK = 0,
    G_RECV_NONBLOCK
} RecvMode;

class SockAddr
{
public:
    /**
     * @brief auto get local address, and rand setting a port
     * @param [in] ip : ip address
     * @param [in] port : port, default is 0, will random generate
     */
    SockAddr();
    explicit SockAddr(const GUint32 ip, const GUint16 port = 0);
    ~SockAddr();
	
    /**
     * @brief set/get IP address
     * @return
     */
    void setIP(const GUint32 ip);
    GUint32 ip();
    GUint8* ipStr();

    /**
     * @brief set/get port
     * @return 
     */
    void setPort(const GUint16 port);
    GUint16 port();
	
    /**
     * @brief set/get sock addr
     * @return 
     */		
    sockaddr_in& addr();
    
    /**
     * @brief get sock address length
     * @return 
     */	    
    GUint16 addrLen() const;
    
private:
    sockaddr_in	    m_addr;
    GUint16         m_addrLen;
};

/** 
 * @brief IPv6
 */
class IPv6Addr
{
public:
    /**
     * auto get local address, and rand setting a port
     */
    IPv6Addr();
    explicit IPv6Addr(const GUint8 ip[16], const GUint16 port = 0);
    ~IPv6Addr();
	
    /**
     * @brief set/get IP address
     * @return
     */		
    GUint8* ip();

    /**
     * @brief set/get port
     * @return 
     */		
    GUint16 port();
	
    /**
     * @brief set/get sock addr
     * @return 
     */		
    sockaddr_in6& addr();
    
    /**
     * @brief get sock address length
     * @return 
     */	        
    GUint16 addrLen() const;
        
private:
    // address
    sockaddr_in6    m_sockAddr;
    GUint16         m_addrLen;
};

/** 
 * @brief server socket class
 */
class Socket
{
public:
    Socket();
    ~Socket();

    /**
     * @brief open socket
     * @param [in] protocol : network protocol
     * @param [in] if_name : interface name
     * @return G_YES/G_NO
     * @note 
     */		
    GResult open(const NetProtocol& protocol, const std::string& if_name);
    /**
     * @brief shutdown connecting
     * @param [in] how : default is 2
     * @return G_YES/G_NO
     * @note 
     *     how = 0 : stop receive data
     *     how = 1 : stop send data
     *     how = 2 : both above way
     */	
    GResult close(const GInt32 how = 2);
    
    /**
     * @brief get sock fd
     * @return sock fd
     */
    GInt32 sockfd() const;
	
    /**
     * @brief get last error string
     * @return error string
     * @note 
     */		
    GInt8* getError();		
	
private:
    /**
     * @brief setting socket options 
     * @param [in] if_name : interface name
     * @return G_YES/G_NO
     */		
    GResult initOption(const std::string& if_name);
	
    /**
     * @brief origin set program running error
     * @param [in] error : error string
     * @note 
     */		
    void setError(const GInt8* args, ...);	
	
private:
    GInt32      m_sockfd;	
    bool        m_isInit;
    GInt8       m_error[G_ERROR_BUF_SIZE];
};
	
/**
 * @brief transfer api
 */
class Transfer
{
public:	
    Transfer();
    ~Transfer();
    
    /**
     * @brief send data
     * @param [in] socket : socket
     * @param [in] data : send data
     * @param [in] len : data length
     * @param [in] flags : default is MSG_NOSIGNAL
     * @return on success, return the number of characters sent. on error, return -1
     * @note 
     */		
    static GInt64 send(Socket& socket, const GUint8* data, const GUint64 len, const GInt32 flags = MSG_NOSIGNAL);
    static GInt64 sendmsg(Socket& socket, const struct msghdr* msg, const GInt32 flags = MSG_NOSIGNAL);
    static GInt64 sendto(Socket& socket, SockAddr& dst_addr, const GUint8* data, const GUint64 len, const GInt32 flags = MSG_NOSIGNAL);
   
    /**
     * @brief receive data
     * @param [in] socket : socket
     * @param [out] buffer : output buffer
     * @param [in] size : buffer size
     * @param [in] flags : flags
     * @return on success, return the number of characters received. on error, return -1
     * @note 
     */	
    static GInt64 recv(Socket& socket, GUint8* buffer, const GUint64 size, const GInt32 flags = 0);	
    static GInt64 recvmsg(Socket& socket, struct msghdr* msg, const GInt32 flags = 0);
    static GInt64 recvfrom(Socket& socket, SockAddr& src_addr, GUint8* buffer, const GUint64 size, const GInt32 flags = 0);    
};

/**
 * @brief socket info
 */
class SocketInfo
{
public:
    SocketInfo() {}
    ~SocketInfo() {}
    
    void setProtocol(const NetProtocol& protocol) { m_protocol = protocol; }
    const NetProtocol& protocol() const { return m_protocol; }
    
    void setServerIP(const GUint32& server_ip) { m_serverIP = server_ip; }
    GUint32 serverIP() const { return m_serverIP; }
    
    void setServerPort(const GUint16& server_port) { m_serverPort = server_port; }
    GUint16 serverPort() const { return m_serverPort; }
    
    void setClinetPort(const GUint16& client_port) { m_clientPort = client_port; }
    GUint16 clientPort() const { return m_clientPort; }
    
    void setLocalIfName(const std::string& local_if_name) { m_localIfName = local_if_name; }
    const std::string& localIfName() const { return m_localIfName; }
    
private:
    NetProtocol   m_protocol;
    GUint32       m_serverIP;
    GUint16       m_serverPort;
    GUint16       m_clientPort;
    std::string   m_localIfName;
};

/** 
 * @brief server socket class
 */
class SocketServer
{
public:
    SocketServer();
    ~SocketServer();
    
    /**
     * @brief server bind
     * @param [in] socket_info : socket information
     * @return G_YES/G_NO
     */
    GResult bind(const SocketInfo& socket_info);
    
    /**
     * @brief server listen port
     * @param [in] connect_num : server max connect client number, default is 20
     * @return G_YES/G_NO
     */
    GResult listen(const GUint32 connect_num = 1024);
    
    /**
     * @brief accept client to connect
     * @param [out] client_addr : output client address infomation
     * @param [in] mode : block or unblock, default block
     * @return success : socket fd, error : -1
     */
    GInt32 accept(SockAddr& client_addr, const RecvMode& mode = G_RECV_BLOCK);
	
    /**
     * @brief receive data
     * @param [in] client_addr : client address
     * @param [out] buffer : output buffer
     * @param [in] size : output buffer size
     * @param [in] mode : block or unblock, default block
     * @return size/-1
     * @note 
     */	
    GInt64 recvfrom(SockAddr& client_addr, GUint8* buffer, const GUint64 size, const RecvMode& mode = G_RECV_BLOCK);
    
    /**
     * @brief close server socket
     * @reture G_YES/G_NO
     * @note 
     */	
    GResult close();
	
    /**
     * @brief get last error string
     * @return error string
     * @note 
     */		
    GInt8* getError();	
	
private:
    /**
     * @brief origin set program running error
     * @param [in] error : error string
     * @note 
     */		
    void setError(const GInt8* args, ...);
	
private:
    Socket      m_socket;
    SockAddr	m_addr;
    SocketInfo  m_socketInfo;
    GInt8       m_error[G_ERROR_BUF_SIZE];
};

/** 
 * @brief client socket class
 */
class SocketClient
{
public:
    SocketClient();
    ~SocketClient();
    
    /**
     * @brief connect socket
     * @param [in] socket_info : socket information
     * @return G_YES/G_NO
     */       
    GResult connect(const SocketInfo& socket_info);
    
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
     * @param [in] mode : block(G_RECV_BLOCK) or unblock(G_RECV_UNBLOCK), default block
     * @return size/-1
     * @note 
     */	
    GInt64 recv(GUint8* buffer, const GUint64 size, const RecvMode& mode = G_RECV_BLOCK);

    /**
     * @brief close client socket
     * @reture G_YES/G_NO
     * @note 
     */	
    GResult close();
    
    /**
     * @brief get last error string
     * @return error string
     * @note 
     */		
    GInt8* getError();	
	
private:
    /**
     * @brief origin set program running error
     * @param [in] error : error string
     * @note 
     */		
    void setError(const GInt8* args, ...);
	
private:
    Socket      m_socket;	
    SockAddr	m_addr;    
    SocketInfo  m_socketInfo;
    GInt8       m_error[G_ERROR_BUF_SIZE];
};

/**
 * @brief epoll socket server
 */
class Epoll
{
public:
    Epoll();
    ~Epoll();
    
    /**
     * @brief add fd
     * @param [in] fd : fd
     * @return G_YES/G_NO
     */      
    GResult addfd(const GInt32 fd, );
    
    /**
     * @brief modify fd
     * @param [in] fd : fd
     * @return G_YES/G_NO
     */      
    GResult modfd(const GInt32 fd);
    
    /**
     * @brief delete fd
     * @param [in] fd : fd
     * @return G_YES/G_NO
     */
    GResult delfd(const GInt32 fd);
    
    /**
     * @brief wait event
     */
    GResult waitEvent();
    
private:
    // inherit from class ThreadTask
    GResult run();
    
private:
    GInt32       m_epollfd; 
    SocketServer m_socketServer;
    static const GUint32 m_defMaxEvents = 1024;
};
}

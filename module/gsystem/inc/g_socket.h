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
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <string>
#include <memory>

#include <g_type.h>

namespace gsys {

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

/** 
 * @brief IPv4 
 */
class IPv4Addr
{
public:
    /**
     * auto get local address, and rand setting a port
     */
    IPv4Addr();
    explicit IPv4Addr(const GUint32 ip, const GUint16 port = 0);
    ~IPv4Addr();

    /**
     * @brief set/get IP address
     * @return
     */		
    GUint32 getIP();
    GUint8* getIPStr();

    /**
     * @brief set/get port
     * @return 
     */		
    GUint16 getPort();
	
    /**
     * @brief set/get sock addr
     * @return 
     */		
    sockaddr_in& getSockAddr();
    
    /**
     * @brief get sock address length
     * @return 
     */	    
    GUint16 getAddrLen() const;
    
private:
    sockaddr_in	    m_sockAddr;
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
    GUint8* getIPStr();

    /**
     * @brief set/get port
     * @return 
     */		
    GUint16 getPort();
	
    /**
     * @brief set/get sock addr
     * @return 
     */		
    sockaddr_in6& getSockAddr();
    
    /**
     * @brief get sock address length
     * @return 
     */	        
    GUint16 getAddrLen() const;
        
private:
    // address
    sockaddr_in6	m_sockAddr;
    GUint16         m_addrLen;
};

/** 
 * @brief server socket class
 */
class Socket
{
public:
    Socket();
    explicit Socket(const IPv4Addr& addr);
    ~Socket();

    /**
     * @brief init socket
     * @return G_YES/G_NO
     * @note 
     */		
    GResult init(const SockType& type, const NetProtocol& protocol);
	
    /**
     * @brief shutdown connecting
     * @param [in] how : way
     * @return G_YES/G_NO
     * @note 
     */	
    GResult uninit(const GInt32 how = 0);	
	
    void setAddr(const IPv4Addr& addr);
    const IPv4Addr& getIPv4Addr() const;
    
    GResult bind();
    GResult listen(const GUint32 max_connect_num = 20);
    GResult accept(sockaddr_in& client_addr);
    GResult accept(sockaddr_in6& client_addr);
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
    GInt64 sendmsg(const struct msghdr* msg, const GInt32 flags = MSG_NOSIGNAL);
    GInt64 sendto(const sockaddr_in& dst_addr, const GUint8* data, const GUint64 len, const GInt32 flags = MSG_NOSIGNAL);
    GInt64 sendto(const sockaddr_in6& dst_addr, const GUint8* data, const GUint64 len, const GInt32 flags = MSG_NOSIGNAL);
	
    /**
     * @brief receive data
     * @param [out] buffer : output buffer
     * @param [in] size : buffer size
     * @param [in] flags : flags
     * @return size/-1
     * @note 
     */	
    GInt64 recv(GUint8* buffer, const GUint64 size, const GInt32 flags = 0);	
    GInt64 recvmsg(struct msghdr* msg, const GInt32 flags = 0);
    GInt64 recvfrom(sockaddr_in& src_addr, GUint8* buffer, const GUint64 size, const GInt32 flags = 0);
    GInt64 recvfrom(sockaddr_in6& src_addr, GUint8* buffer, const GUint64 size, const GInt32 flags = 0);
	
    /**
     * @brief get last error string
     * @return error string
     * @note 
     */		
    GInt8* getError();		
	
private:
    /**
     * @brief setting socket options 
     * @return G_YES/G_NO
     */		
    GResult initOption();
	
    /**
     * @brief origin set program running error
     * @param [in] error : error string
     * @note 
     */		
    void setError(const GInt8* args, ...);	
	
private:
    GInt32      m_sockfd;	
    IPv4Addr    m_ipv4Addr;
    bool        m_isInit;
    GInt8       m_error[G_ERROR_BUF_SIZE];
};
	
/** 
 * @brief server socket class
 */
class SocketServer
{
public:
    SocketServer();
    
    /**
     * @brief auto find location host
     */
    explicit SocketServer(const GUint32 server_ip, const GUint16 server_port);
    ~SocketServer();
    
    const Socket& getSocket() const;
	
    /**
     * @brief bind address and port
     * @return G_YES/G_NO
     * @note 
     */	
    GResult bind();	
	
    /**
     * @brief listen port
     * @param [in] max_connect_num : the max client connect number, default is 20
     * @return G_YES/G_NO
     * @note 
     */	
    GResult listen(const GUint32 max_connect_num = 20);	
	
    /**
     * @brief waitting for connect
     * @param [in] client_addr : client sock address
     * @return G_YES/G_NO
     * @note 
     */	
    GResult accept(IPv4Addr& client_addr);
	
    /**
     * @brief receive data
     * @param [in] client_addr : client address
     * @param [out] buffer : output buffer
     * @param [in] size : output buffer size
     * @param [in] flags : flags, default is 0
     * @return size/-1
     * @note 
     */	
    GInt64 recvfrom(IPv4Addr& client_addr, GUint8* buffer, const GUint64 size, const GInt32 flags = 0);
	
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
    GInt8       m_error[G_ERROR_BUF_SIZE];
};

/** 
 * @brief client socket class
 */
class SocketClient
{
public:
    SocketClient();
    
    /**
     * @brief connect location server
     */		
    explicit SocketClient(const GUint32 server_ip, const GUint16 server_port);
    ~SocketClient();
    
    const Socket& getSocket() const;
    
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
    Socket    m_socket;	
    GInt8     m_error[G_ERROR_BUF_SIZE];
};

/**
 * @brief epoll socket server
 */
class EpollServer
{
public:
    EpollServer();
    
    /**
     * @brief constructor
     * @param [in] server_ip : server ip address
     * @param [in] server_port : server port, default is 0, indent to random generate
     * @param [in] interface : net card name, default is eth0, network communication card
     */
    explicit EpollServer(const GUint32 server_ip, const GUint16 server_port = 0, const GInt8* interface = "eth0");
    virtual ~EpollServer();
    
    GResult init();
	
private:
    SocketServer	m_socketServer;
};

/**
 * @brief epoll socket client
 */
class EpollClient
{
public:
    EpollClient();
   
    /**
     * @brief constructor
     * @param [in] server_ip : server ip address
     * @param [in] server_port : server port, default is 0, indent to random generate
     * @param [in] interface : net card name, default is eth0, network communication card
     */	
    explicit EpollClient(const GUint32 server_ip, const GUint16 server_port = 0, const GInt8* interface = nullptr);
    virtual ~EpollClient();
   
    GResult init();
	
private:
    SocketClient    m_socketClient;
};

}

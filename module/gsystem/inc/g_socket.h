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
    ~Socket();

    /**
     * @brief init socket
     * @param [in] type : socket type
     * @param [in] protocol : network protocol
     * @param [in] if_name : interface name
     * @return G_YES/G_NO
     * @note 
     */		
    GResult init(const SockType& type, const NetProtocol& protocol, const std::string& if_name);
	
    /**
     * @brief shutdown connecting
     * @param [in] how : default is 2
     * @return G_YES/G_NO
     * @note 
     *     how = 0 : stop receive data
     *     how = 1 : stop send data
     *     how = 2 : both above way
     */	
    GResult uninit(const GInt32 how = 2);
    
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
     * @param [in] flags : flags
     * @return size/-1
     * @note 
     */		
    static GInt64 send(Socket& socket, const GUint8* data, const GUint64 len, const GInt32 flags = MSG_NOSIGNAL);
    static GInt64 sendmsg(Socket& socket, const struct msghdr* msg, const GInt32 flags = MSG_NOSIGNAL);
    static GInt64 sendto(Socket& socket, const SockAddr& dst_addr, const GUint8* data, const GUint64 len, const GInt32 flags = MSG_NOSIGNAL);
   
    /**
     * @brief receive data
     * @param [in] socket : socket
     * @param [out] buffer : output buffer
     * @param [in] size : buffer size
     * @param [in] flags : flags
     * @return size/-1
     * @note 
     */	
    static GInt64 recv(Socket& socket, GUint8* buffer, const GUint64 size, const GInt32 flags = 0);	
    static GInt64 recvmsg(Socket& socket, struct msghdr* msg, const GInt32 flags = 0);
    static GInt64 recvfrom(Socket& socket, SockAddr& src_addr, GUint8* buffer, const GUint64 size, const GInt32 flags = 0);    
};

/** 
 * @brief server socket class
 */
class SocketServer
{
public:
    SocketServer();
    
    /**
     * @brief constructor
     * @param [in] server_ip : server ip address
     * @param [in] server_port : server port, default is 0, indent to random generate
     * @param [in] if_name : net card name, default is eth0, network communication card, default is eth0 
     */     
    explicit SocketServer(const GUint32 server_ip, const GUint16 server_port = 0, const std::string& if_name = "eth0");
    ~SocketServer();
    
    /**
     * @brief set server address
     * @param [in] server_ip : server ip address
     */
    void setIP(const GUint32 server_ip);
    
    /**
     * @brief set server port
     * @param [in] server_port : server bind port
     */
    void setPort(const GUint16 server_port);
    
    /**
     * @brief set interface
     * @param [in] if_name : server socket communication interface name
     */
    void setIf(const std::string& if_name);
    
    /**
     * @brief server bind port
     * @return G_YES/G_NO
     */
    GResult bind();
    
    /**
     * @brief server listen port
     * @param [in] connect_num : server max connect client number, default is 20
     * @return G_YES/G_NO
     */
    GResult listen(const GUint32 connect_num = 20);
    
    /**
     * @brief accept client to connect
     * @param [out] client_addr : output client address infomation
     * @return G_YES/G_NO
     */
    GResult accept(SockAddr& client_addr);
	
    /**
     * @brief receive data
     * @param [in] client_addr : client address
     * @param [out] buffer : output buffer
     * @param [in] size : output buffer size
     * @param [in] flags : flags, default is 0
     * @return size/-1
     * @note 
     */	
    GInt64 recvfrom(SockAddr& client_addr, GUint8* buffer, const GUint64 size, const GInt32 flags = 0);
	
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
    std::string m_ifName;
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
     * @brief constructor
     * @param [in] server_ip : server ip address
     * @param [in] server_port : server port, default is 0, indent to random generate
     * @param [in] if_name : net card name, default is eth0, network communication card, default is eth0 
     */     
    explicit SocketClient(const GUint32 server_ip, const GUint16 server_port = 0, const std::string& if_name = "eth0");    
    ~SocketClient();
    
    /**
     * @brief set server address
     * @param [in] server_ip : server ip address
     */
    void setIP(const GUint32 server_ip);
    
    /**
     * @brief set server port
     * @param [in] server_port : server bind port
     */
    void setPort(const GUint16 server_port);
    
    /**
     * @brief set interface
     * @param [in] if_name : server socket communication interface name
     */
    void setIf(const std::string& if_name);    

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
    Socket		m_socket;	
    SockAddr	m_addr;    
    std::string m_ifName;
    GInt8       m_error[G_ERROR_BUF_SIZE];
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
     * @param [in] interface : net card name, default is eth0, network communication card, default is eth0 
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
     * @param [in] interface : net card name, default is eth0, network communication card, default is eth0
     */	
    explicit EpollClient(const GUint32 server_ip, const GUint16 server_port = 0, const GInt8* interface = "eth0");
    virtual ~EpollClient();
   
    GResult init();
	
private:
    SocketClient    m_socketClient;
};

}

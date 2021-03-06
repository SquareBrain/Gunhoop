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
#include <list>

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
    // for SOCK_STREAM
    G_IPPROTO_TCP,
    // for SOCK_DGRAM
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
    void setIp(const GUint32 ip);
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
     * @param [in] ifName : interface name
     * @return G_YES/G_NO
     * @note 
     */		
    GResult open(const NetProtocol& protocol, const std::string& ifName);
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
    GInt8* error();		
	
private:
    /**
     * @brief setting socket options 
     * @param [in] ifName : interface name
     * @return G_YES/G_NO
     */		
    GResult initOption(const std::string& ifName);
	
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
 * @brief Multicast Socket
 */
class Multicast
{
public:
    Multicast();
    ~Multicast();
    
private:

};

/**
 * @brief broadcast
 */
class Broadcast
{
public:
    Broadcast();
    ~Broadcast();
    
private:
    
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
    SocketInfo() :  m_protocol(G_IPPROTO_TCP)
        , m_serverIp(0)
        , m_serverPort(0)
        , m_clientPort(0)
        , m_localIfName("eth0")
        , m_maxConnectNum(1024) {}
        
    ~SocketInfo() {}
    
    void setProtocol(const NetProtocol& protocol) { m_protocol = protocol; }
    const NetProtocol& protocol() const { return m_protocol; }
    
    void setServerIp(const GUint32& serverIp) { m_serverIp = serverIp; }
    GUint32 serverIp() const { return m_serverIp; }
    
    void setServerPort(const GUint16& serverPort) { m_serverPort = serverPort; }
    GUint16 serverPort() const { return m_serverPort; }
    
    void setClinetPort(const GUint16& clientPort) { m_clientPort = clientPort; }
    GUint16 clientPort() const { return m_clientPort; }
    
    void setLocalIfName(const std::string& localIfName) { m_localIfName = localIfName; }
    const std::string& localIfName() const { return m_localIfName; }
   
    void setMaxConnectNum(const GUint32& maxConnectNum) { m_maxConnectNum = maxConnectNum; }
    GUint32 maxConnectNum() const { return m_maxConnectNum; }
    
private:
    NetProtocol   m_protocol;
    GUint32       m_serverIp;
    GUint16       m_serverPort;
    GUint16       m_clientPort;
    std::string   m_localIfName;
    // server max connect client number, default is 1024
    GUint32       m_maxConnectNum;
};

/**
 * @brief epoll socket server
 */
class Epoll
{
public:
    typedef enum
    {
        G_RECV_UN,
        G_RECV_FD,
        G_SEND_FD
    } EventType;
    
    class Event
    {
    public:
        Event() : m_fd(-1), m_type(G_RECV_UN) {}
        Event(const GInt32 fd, const EventType type, void* data = nullptr) 
            : m_fd(fd), m_type(type), m_data(data) {}
        ~Event() {}
        
        /**
         * @brief get/set fd
         * @return fd
         */
        void setfd(const GInt32 fd) { m_fd = fd; }
        GInt32 fd() const { return m_fd; }
        
        /**
         * @brief get/set events
         * @return events
         */
        void setType(const EventType type) { m_type = type; }
        EventType type() const { return m_type; }    
        
        /**
         * @brief get/set user data
         * @return user data
         */
        void setData(void* data) { m_data = data; }
        void* data() { return m_data; }  
        
        /**
         * @brief is received data
         * @return G_YES/G_NO
         */
        GResult isRecv() { return m_type == G_RECV_FD ? G_YES : G_NO; }
        
         /**
         * @brief is sent data
         * @return G_YES/G_NO
         */
        GResult isSend() { return m_type == G_SEND_FD ? G_YES : G_NO; }     
        
    private:
        GInt32     m_fd;	
        EventType  m_type;
        void*      m_data;
    };
    
    typedef std::list<Event> EventList;
    
public:
    Epoll();
    ~Epoll();
    
    /**
     * @brief open epoll
     * @param [in] maxEvent : the number of the max events
     * @return G_YES/G_NO
     */
    GResult open(const GUint32 maxEvent = 1024);
    
     /**
     * @brief close epoll
     * @return G_YES/G_NO
     */
    GResult close();   
    
    /**
     * @brief add fd
     * @param [in] fd : fd
     * @param [in] events : epoll events, default is EPOLLIN | EPOLLOUT
     * @return G_YES/G_NO
     */      
    GResult addfd(const GInt32 fd, const GUint32 events = EPOLLIN | EPOLLOUT);
    
    /**
     * @brief modify fd
     * @param [in] fd : fd
     * @param [in] events : epoll evnets, default is EPOLLIN | EPOLLOUT
     * @return G_YES/G_NO
     */      
    GResult modfd(const GInt32 fd, const GUint32 events = EPOLLIN | EPOLLOUT);
    
    /**
     * @brief delete fd
     * @param [in] fd : fd
     * @return G_YES/G_NO
     */
    GResult delfd(const GInt32 fd);
    
    /**
     * @brief wait event
     * @param [out] eventList : return event
     * @param [in] timeout : wait time out, default is -1, indicate block, millisecond
     * @return G_YES/G_NO
     */
    GResult wait(EventList& eventList, const GUint32 timeout = -1);
    
    /**
     * @brief get last error string
     * @return error string
     * @note 
     */		
    GInt8* error();	
    
private:
    // create epoll
    GResult create();
    
    /**
     * @brief init error
     */
    void initError();
    
    /**
     * @brief origin set program running error
     * @param [in] error : error string
     * @note 
     */		
    void setError(const GInt8* args, ...);    
    
private:
    GInt32      m_epollfd; 
    GUint32     m_maxEvents;
    GInt8       m_error[G_ERROR_BUF_SIZE];
    GUint32     m_errorHeaderOffset;
};

/** 
 * @brief server socket class
 */
class ServerSocket
{
public:
    ServerSocket();
    ~ServerSocket();
    
    /**
     * @brief server bind and listen
     * @param [in] socketInfo : socket information
     * @return G_YES/G_NO
     */
    GResult open(const SocketInfo& socketInfo);
    
    /**
     * @brief close server socket
     * @reture G_YES/G_NO
     * @note 
     */	
    GResult close();    
    
    /**
     * @brief accept client to connect
     * @param [out] clientAddr : output client address infomation
     * @param [out] sockfd : new sockfd
     * @param [in] mode : block or unblock, default block
     * @return G_YES/G_NO
     */
    GResult accept(SockAddr& clientAddr, GInt32& sockfd, const RecvMode& mode = G_RECV_BLOCK);
    
    /**
     * @brief send data
     * @param [in] data : send data
     * @param [in] len : data length
     * @return size/-1
     * @note 
     */		
    GInt64 send(const GUint8* data, const GUint64 len);
    
    /**
     * @brief send data for UDP protocol
     * @param [in] dstAddr : destination address
     * @param [in] data : send data
     * @param [in] len : data length
     * @return on success, return the number of characters sent. on error, return -1
     */
    GInt64 sendto(SockAddr& dstAddr, const GUint8* data, const GUint64 len);

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
     * @brief receive data
     * @param [out] src_addr : source address
     * @param [out] buffer : output buffer
     * @param [in] size : buffer size
     * @param [in] mode : block(G_RECV_BLOCK) or unblock(G_RECV_UNBLOCK), default block
     * @return on success, return the number of characters received. on error, return -1
     * @note 
     */
    GInt64 recvfrom(SockAddr& srcAddr, GUint8* buffer, const GUint64 size, const RecvMode& mode = G_RECV_BLOCK);  
    
    /**
     * @brief get server socket
     * @return socket
     */
    Socket& socket();
	
    /**
     * @brief get last error string
     * @return error string
     * @note 
     */		
    GInt8* error();	
	
private:
    /**
     * @brief init error
     */
    void initError();
    
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
    GUint32     m_errorHeaderOffset;
};

/** 
 * @brief client socket class
 */
class ClientSocket
{
public:
    ClientSocket();
    ~ClientSocket();
    
    /**
     * @brief open socket connect
     * @param [in] socketInfo : socket information
     * @return G_YES/G_NO
     */       
    GResult open(const SocketInfo& socketInfo);
    
    /**
     * @brief close client socket
     * @reture G_YES/G_NO
     * @note 
     */	
    GResult close();    
    
    /**
     * @brief send data
     * @param [in] data : send data
     * @param [in] len : data length
     * @return size/-1
     * @note 
     */		
    GInt64 send(const GUint8* data, const GUint64 len);
    
    /**
     * @brief send data for UDP protocol
     * @param [in] dstAddr : destination address
     * @param [in] data : send data
     * @param [in] len : data length
     * @return on success, return the number of characters sent. on error, return -1
     */
    GInt64 sendto(SockAddr& dstAddr, const GUint8* data, const GUint64 len);    
    
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
     * @brief receive data
     * @param [out] srcAddr : source address
     * @param [out] buffer : output buffer
     * @param [in] size : buffer size
     * @param [in] mode : block(G_RECV_BLOCK) or unblock(G_RECV_UNBLOCK), default block
     * @return on success, return the number of characters received. on error, return -1
     * @note 
     */
    GInt64 recvfrom(SockAddr& srcAddr, GUint8* buffer, const GUint64 size, const RecvMode& mode = G_RECV_BLOCK);         
    
    /**
     * @brief get last error string
     * @return error string
     * @note 
     */		
    GInt8* error();	
	
private:
    /**
     * @brief init error
     */
    void initError();
    
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
    GUint32     m_errorHeaderOffset;
};
}

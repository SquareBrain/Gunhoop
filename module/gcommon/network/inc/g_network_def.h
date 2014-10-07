/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_network.h
* @version     
* @brief      
* @author   duye
* @date     2014-05-13
* @note 
*
*  2. 2014-06-21 duye move to gohoop 
*  1. 2014-05-13 duye Created this file
* 
*/

#pragma once

#include <g_system.h>

static const GUint16 IPV4_BUF_SIZE = 16;

namespace gcom {
	
/**
 * IP type
 */
typedef enum
{
	IP_TYPE_V4,
	IP_TYPE_V6
} IPAddrType;

/**
 * network convert class
 */
class NetWorkConv
{
public:
	NetWorkConv();
	~NetWorkConv();
	
	/**
	 * @brief string IP to integer
	 * @param [in] str_ip :  input string IP
	 * @param [out] int_ip : output integer IP
	 * @return G_YES/G_NO
	 */
	static GResult ipToInteger(const std::string& str_ip, GUint32& int_ip);
	
	/**
	 * @brief integer IP to string
	 * @param [in] int_ip :  input integer IP
	 * @param [out] str_ip : output string IP
	 * @return G_YES/G_NO
	 */
	static GResult ipToString(const GUint32 int_ip, std::string& str_ip);	
	
	/**
	 * @brief string MAC to integer
	 * @param [in] str_mac :  input string MAC, like : 00:60:6E:73:01:77
	 * @param [out] int_mac : output integer MAC, long long
	 * @return G_YES/G_NO
	 */
	static GResult macToInteger(const std::string& str_mac, GUint64& int_mac);
	
	/**
	 * @brief bytes MAC to integer
	 * @param [in] bytes_mac : input string MAC, 6 bytes
	 * @param [out] int_mac : output integer MAC, long long
	 * @return G_YES/G_NO
	 */
	static GResult macToInteger(const GInt8 bytes_mac[6], GUint64& int_mac);	
	
	/**
	 * @brief string MAC to bytes
	 * @param [in] str_mac :  input integer MAC, like : 00:60:6E:73:01:77
	 * @param [out] bytes_mac : output bytes MAC, 6 bytes
	 * @return G_YES/G_NO
	 */
	static GResult macToBytes(const std::string& str_mac, GInt8 bytes_mac[6]);
	
	/**
	 * @brief integer MAC to bytes
	 * @param [in] int_mac : integer MAC, long long, 8 byte
	 * @param [out] bytes_mac : output string MAC, 6 bytes
	 * @return G_YES/G_NO
	 */
	static GResult macToBytes(const GUint64 int_mac, GInt8 bytes_mac[6]);
	
	/**
	 * @brief integer MAC to string
	 * @param [in] int_mac : integer MAC, 8 byte, long long
	 * @param [out] str_mac : output string MAC, like : 00:60:6E:73:01:77
	 * @return G_YES/G_NO
	 */
	static GResult macToString(const GUint64 int_mac, std::string& str_mac);	
	
	/**
	 * @brief bytes MAC to string
	 * @param [in] bytes_mac : bytes MAC, 6 bytes
	 * @param [out] str_mac : output string MAC, like : 00:60:6E:73:01:77
	 * @return G_YES/G_NO
	 */
	static GResult macToString(GInt8 bytes_mac[6], std::string& str_mac);	
};
	
/**
 * IP address
 */
class IPAddr
{
public:
    IPAddr();
	explicit IPAddr(const GUint32 ip);
	explicit IPAddr(const std::string& ip);
	
	~IPAddr();

    void setIP(const GUint32 ip);
    void setIP(const std::string& ip);
    
	GUint32 getIP() const;
    const std::string& getIPStr() const;
	
private:
    GUint32     m_ip;
    std::string m_ipStr;
};

/**
 * MAC address
 */
class MacAddr
{
public:
    MacAddr();
	explicit MacAddr(GUint64 mac);
	explicit MacAddr(const std::string& mac);

    void setMac(GUint64 mac);
    void setMac(const std::string& mac);
    
	GUint64 getMac() const;
	const std::string& getMacStr() const;
	
private:
	GUint64 		m_mac;
	std::string		m_macStr;
};

/**
 * IP and port pair
 */
class IPPortPair
{
public:
    IPPortPair();
	explicit IPPortPair(const IPAddr& ip_addr, const GUint16 port);
	~IPPortPair();
	
	void setIPAddr(const IPAddr& ip_addr);
	const IPAddr& getIPAddr() const;
	
	void setPort(const GUint16 port);
	GUint16 getPort() const;
	
private:
	IPAddr  m_ipAddr;
	GUint16	m_port;
};

/**
 * network address 
 */
class NetAddr
{
public:
	NetAddr();
    explicit NetAddr(const MacAddr mac_addr, const IPAddr& ip_addr, const GUint16 port);
	~NetAddr();
	
	void setMacAddr(const MacAddr& mac_addr);
	const MacAddr& getMacAddr() const;
	
	void setIPAddr(const IPAddr& ip_addr);
	const IPAddr& getIPAddr() const;
	
	void setPort(const GUint16 port);
	GUint16 getPort() const;
	
private:
	MacAddr	m_macAddr;
    IPAddr  m_ip;
    GUint16 m_port;
};
 
/**
 * socket address
 */
class SocketAddr
{
public:
    SocketAddr();
	explicit SocketAddr(const NetAddr& src_addr, const NetAddr& dst_addr);
	~SocketAddr();
	
	void setSrcAddr(const NetAddr& src_addr);
	const NetAddr& getSrcAddr() const;
	
	void setDstAddr(const NetAddr& dst_addr);
	const NetAddr& getDstAddr() const;
	
private:
	NetAddr	m_srcAddr;
	NetAddr	m_dstAddr;
};

}

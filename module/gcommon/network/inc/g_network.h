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
	 * @param [in] strIP :  input string IP
	 * @param [out] intIP : output integer IP
	 * @return G_YES/G_NO
	 */
	GResult ipToInteger(const std::string& strIP, GUint32& intIP);
	
	/**
	 * @brief integer IP to string
	 * @param [in] intIP :  input integer IP
	 * @param [out] strIP : output string IP
	 * @return G_YES/G_NO
	 */
	GResult ipToString(const GUint32 intIP, std::string& strIP);	
	
	/**
	 * @brief string MAC to integer
	 * @param [in] strMac :  input string MAC
	 * @param [out] intMac : output integer MAC
	 * @return G_YES/G_NO
	 */
	GResult macToInteger(const std::string& strMac, GUint32& intMac);
	
	/**
	 * @brief integer MAC to string
	 * @param [in] intMac :  input integer MAC
	 * @param [out] strMac : output string MAC
	 * @return G_YES/G_NO
	 */
	GResult macToString(const GUint32 intMac, std::string& strMac);	
};
	
/**
 * IP address
 */
class IPAddr
{
public:
	explicit IPAddr(const GUint32 ip);
	explicit IPAddr(const GInt8* ip);
	explicit IPAddr(const std::string& ip);
	
	~IPAddr();
	
	GUint32 getIP() const;
	const std::string& getIP() const;
	
private:
	GUint32			m_ip;
	std::string		m_ipStr;
};

/**
 * IPv6 address
 */
class IPv6Addr
{
public:
	explicit IPv6Addr(const GInt8* ip);
	explicit IPv6Addr(const std::string& ip);
	~IPv6Addr();
	
	const std::string& getIP() const;
	
private:
	std::string		m_ipStr;	
};

/**
 * MAC address
 */
class MacAddr
{
public:
	explicit MacAddr(GUint64 mac);
	explicit MacAddr(const GInt8* mac);
	explicit MacAddr(const std::string& mac);
	
	GUint64 getMac() const;
	const std::string& getMac() const;
	
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
	explicit IPPortPair(const IPAddr& ipAddr, const GUint16 port);
	explicit IPPortPair(const IPv6Addr& ipAddr, const GUint16 port);
	~IPPortPair();
	
	void setIPv4(const IPAddr& ipAddr);
	const IPAddr& getIPv4() const;
	
	void setIPv6(const IPv6Addr& ipAddr);
	const IPv6Addr& getIPv6() const;
	
	void setPort(const GUint16 port);
	const GUint16 getPort() const;
	
private:
	IPAddr			m_ipv4;
	IPv6Addr		m_ipv6;
	GUint16			m_port;
	IPAddrType		m_ipType;
};

/**
 * network address 
 */
class NetAddr
{
public:
	explicit NetAddr(const IPAddrType& ipAddrType/*IP_TYPE_V4*/);
	~NetAddr();
	
	void setMacAddr(const MacAddr& macAddr);
	const MacAddr& getMacAddr() cosnt;
	
	void setIPv4Addr(const IPAddr& ipAddr);
	const IPAddr& getIPv4Addr() const;
	
	void setIPv6Addr(const IPv6Addr& ipAddr);
	const IPv6Addr& getIPv6Addr() const;
	
	void setPort(const GUint16 port);
	GUint16 getPort() const;
	
private:
	MacAddr		m_macAddr;
	IPAddr		m_ipv4Addr;
	IPv6Addr	m_ipv6Addr;
	GUint16		m_port;
};
 
/**
 * socket address
 */
clas SocketAddr
{
public:
	explicit SocketAddr(const NetAddr& srcAddr, const NetAddr& dstAddr);
	~SocketAddr();
	
	void setSrcAddr(const NetAddr& srcAddr);
	const NetAddr& getSrcAddr() const;
	
	void setDstAddr(const NetAddr& dstAddr);
	const NetAddr& getDstAddr() const;
	
private:
	NetAddr		m_srcAddr;
	NetAddr		m_dstAddr;
};

}

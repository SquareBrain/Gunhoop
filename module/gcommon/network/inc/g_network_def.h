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
	 * @param [in] strMac :  input string MAC, like : 00:60:6E:73:01:77
	 * @param [out] intMac : output integer MAC, long long
	 * @return G_YES/G_NO
	 */
	GResult macToInteger(const std::string& strMac, GUint64& intMac);
	
	/**
	 * @brief bytes MAC to integer
	 * @param [in] bytesMac : input string MAC, 6 bytes
	 * @param [out] intMac : output integer MAC, long long
	 * @return G_YES/G_NO
	 */
	GResult macToInteger(const GInt8 bytesMac[6], GUint64& intMac);	
	
	/**
	 * @brief string MAC to bytes
	 * @param [in] strMac :  input integer MAC, like : 00:60:6E:73:01:77
	 * @param [out] bytesMac : output bytes MAC, 6 bytes
	 * @return G_YES/G_NO
	 */
	GResult macToBytes(const std::string& strMac, GInt8 bytesMac[6]);
	
	/**
	 * @brief integer MAC to bytes
	 * @param [in] intMac : integer MAC, long long, 8 byte
	 * @param [out] bytesMac : output string MAC, 6 bytes
	 * @return G_YES/G_NO
	 */
	GResult macToBytes(const GUint64 intMac, GInt8 bytesMac[6]);
	
	/**
	 * @brief integer MAC to string
	 * @param [in] intMac : integer MAC, 8 byte, long long
	 * @param [out] strMac : output string MAC, like : 00:60:6E:73:01:77
	 * @return G_YES/G_NO
	 */
	GResult macToString(const GUint64 intMac, std::string& strMac);	
	
	/**
	 * @brief bytes MAC to string
	 * @param [in] bytesMac : bytes MAC, 6 bytes
	 * @param [out] strMac : output string MAC, like : 00:60:6E:73:01:77
	 * @return G_YES/G_NO
	 */
	GResult macToString(GInt8 bytesMac[6], std::string& strMac);	
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
	IPv4Addr		m_ipv4Addr;
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
	~IPPortPair();
	
	void setIPAddr(const IPAddr& ipAddr);
	const IPAddr& getIPAddr() const;
	
	void setPort(const GUint16 port);
	const GUint16 getPort() const;
	
private:
	IPAddr			m_ipAddr;
	GUint16			m_port;
};

/**
 * network address 
 */
class NetAddr
{
public:
	NetAddr();
	~NetAddr();
	
	void setMacAddr(const MacAddr& macAddr);
	const MacAddr& getMacAddr() cosnt;
	
	void setIPv4Addr(const IPAddr& ipAddr);
	const IPAddr& getIPv4Addr() const;
	
	void setPort(const GUint16 port);
	GUint16 getPort() const;
	
private:
	MacAddr		m_macAddr;
	IPAddr		m_ipAddr;
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

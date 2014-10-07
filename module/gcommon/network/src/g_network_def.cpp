/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_network_def.cpp
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
#include <g_network_def.h>

namespace gcom {
    
NetWorkConv::NetWorkConv() {}
NetWorkConv::~NetWorkConv() {}

GResult NetWorkConv::ipToInt(const std::string& strIP, GUint32& intIP)
{
    std::list<std::string> split_list;
    IS_NOED(Convert::splitString(strIP, '.', split_list));
    IS_FALSED(split_list.size() == 0, G_ERROR_INVALID_PARAMETERS);
    
    GUint32 ip_array[4] = {0};
    GUint16 i = 0;
    std::list<std::string>::iterator iter = split_list.begin();
    for (; iter != split_list.end(); ++iter)
    {
        ip_array[i] = std::stoul(*iter);
    }
    
    intIP = (ip_array[0] << 24) + (ip_array[1] << 16) + (ip_array[2] << 8) + ip_array[3];
    
    return G_YES;
}

GResult NetWorkConv::ipToString(const GUint32 intIP, std::string& strIP)
{
    GInt8 tmp_buf[16] = {0};  
    sprintf(tmp_buf, "%u.%u.%u.%u",  
        (strIP & 0xff000000) >> 24,  
        (strIP & 0x00ff0000) >> 16,  
        (strIP & 0x0000ff00) >> 8,  
        (strIP & 0x000000ff));
    
    strIP.assign(tmp_buf);
        
    return G_YES;
}

GResult NetWorkConv::macToInteger(const std::string& strMac, GUint64& intMac)
{
    std::list<std::string> split_list;
    IS_NOED(Convert::splitString(strIP, '.', split_list));
    IS_FALSED(split_list.size() == 0, G_ERROR_INVALID_PARAMETERS);
    
    GUint32 ip_array[4] = {0};
    GUint16 i = 0;
    std::list<std::string>::iterator iter = split_list.begin();
    for (; iter != split_list.end(); ++iter)
    {
        ip_array[i] = std::stoul(*iter);
    }
    
    intIP = (ip_array[0] << 24) + (ip_array[1] << 16) + (ip_array[2] << 8) + ip_array[3];
    
    return G_YES;  
    return G_YES;
}

GResult NetWorkConv::macToInteger(const GInt8 bytesMac[6], GUint64& intMac)
{
    return G_YES;
}

GResult NetWorkConv::macToBytes(const std::string& strMac, GInt8 bytesMac[6])
{
    return G_YES;
}

GResult NetWorkConv::macToBytes(const GUint64 intMac, GInt8 bytesMac[6])
{
    return G_YES;
}

GResult NetWorkConv::macToString(const GUint64 intMac, std::string& strMac)
{
    return G_YES;
}

GResult NetWorkConv::macToString(GInt8 bytesMac[6], std::string& strMac)
{
    return G_YES;
}

IPAddr::IPAddr(const GUint32 ip) : m_ip(ip) 
{
    NetWorkConv::ipToString(m_ip, m_ipStr);
}

IPAddr::IPAddr(const GInt8* ip) : m_ipStr(ip)
{
    NetWorkConv::ipToInteger(m_ipStr, m_ip);
}

IPAddr::IPAddr(const std::string& ip) : m_ipStr(ip)
{
    NetWorkConv::ipToInteger(m_ipStr, m_ip);
}

IPAddr::~IPAddr() 
{
}

GUint32 IPAddr::getIP() const
{
    return m_ip;
}

const std::string& IPAddr::getIP(const GUint32 ip) const 
{
    return m_ipStr;
}

MacAddr::MacAddr(const GUint64 mac) : m_mac(mac)
{
    NetWorkConv::macToString(m_mac, m_macStr);
}

MacAddr::MacAddr(const GInt8* mac) : m_macStr(mac)
{
    NetWorkConv::macToInteger(m_macStr, m_mac);
}

MacAddr::MacAddr(const std::string& mac) : m_macStr(mac)
{
    NetWorkConv::macToInteger(m_macStr, m_mac);
}

GUint64 MacAddr::getMac() const
{
	return m_mac;
}

const std::string& MacAddr::getMacStr() const
{
	return m_macStr;
}

IPPortPair::IPPortPair() {}

IPPortPair::IPPortPair(const IPAddr& ip_addr, const GUint16 port) : m_ipv4(ip_addr), m_port(port)
{
}

IPPortPair::~IPPortPair() {}

void IPPortPair::setIPAddr(const IPAddr& ip_addr)
{
    m_ipAddr = ip_addr;
}

const IPAddr& IPPortPair::getIPAddr() const
{
    return m_ipAddr;
}

void IPPortPair::setPort(const GUint16 port)
{
    m_port = port;
}

GUint16 IPPortPair::getPort() const
{
    return m_port;
}

NetAddr::NetAddr() {}

NetAddr::NetAddr(const MacAddr mac_addr, const IPAddr& ip_addr, const GUint16 port)
	: m_macAddr(mac_addr)
{
	m_ipPortPair.setIPAddr(ip_addr);
	m_ipPortPair.setPort(port);
}

NetAddr::~NetAddr() {}

void NetAddr::setMacAddr(const MacAddr& mac_addr)
{
    m_macAddr = mac_addr;
}

const MacAddr& NetAddr::getMacAddr() const
{
    return m_macAddr;
}

void NetAddr::setIPAddr(const IPAddr& ip_addr)
{
    m_ipPortPair.setIPAddr(ip_addr);
}

const IPAddr& NetAddr::getIPAddr() const
{
    return m_ipPortPair.getIPAddr();
}

void NetAddr::setPort(const GUint16 port)
{
    m_ipPortPair.setPort(port);
}

GUint16 NetAddr::getPort() const
{
    return m_ipPortPair.getPort();
}

SocketAddr::SocketAddr() {}

SocketAddr::SocketAddr(const NetAddr& src_addr, const NetAddr& dst_addr) 
	: m_srcAddr(src_addr), m_dstAddr(dst_addr)
{
}

SocketAddr::~SocketAddr()
{
}

void SocketAddr::setSrcAddr(const NetAddr& src_addr)
{
    m_srcAddr = src_addr;
}

const NetAddr& SocketAddr::getSrcAddr() const
{
    return m_srcAddr;
}

void SocketAddr::setDstAddr(const NetAddr& dst_addr)
{
    m_dstAddr = dst_addr;
}

const NetAddr& SocketAddr::getDstAddr() const
{
    return m_dstAddr;
}

}

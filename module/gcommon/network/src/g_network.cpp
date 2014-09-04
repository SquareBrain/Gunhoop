/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_network.cpp
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

#include <g_network.h>

namespace gcom {
    
NetWorkConv::NetWorkConv() {}
NetWorkConv::~NetWorkConv() {}

GResult NetWorkConv::ipToInt(const std::string& strIP, GUint32& intIP)
{
    std::string tmpStr;
    GUint16 begPos = 0;
    
    for (GUint16 i = 0; i < strIP.size(); i++)
    {
        if (strIP[i] != '.' && (strIP[i] < '0' || strIP[i] > '9'))
        {
            return G_NO;
        }
        
        if (i == 0 && strIP[i] == '.')
        {
            return G_NO;
        }
        
        if (strIP[i] == '.')
        {
            tmpStr.assign(strIP.subString(beg, i - beg));
        }
    }
    
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

GResult NetWorkConv::macToInteger(const std::string& strMac, GUint32& intMac)
{
    return G_YES;
}

GResult NetWorkConv::macToString(const GUint32 intMac, std::string& strMac)
{
    return G_NO;
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

IPv6Addr::IPv6Addr(const GInt8* ip) : m_ipStr(ip)
{
}

IPv6Addr::IPv6Addr(const std::string& ip) : m_ipStr(ip)
{
}

const std::string& IPv6Addr::getIP() const
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

IPPortPair::IPPortPair(const IPAddr& ipAddr, const GUint16 port) : m_ipAddrType(IP_TYPE_V4), m_ipv4(ipAddr), m_port(port)
{
}

IPPortPair::IPPortPair(const IPv6Addr& ipAddr, const GUint16 port) : m_ipAddrType(IP_TYPE_V6), m_ipv6(ipAddr), m_port(port)
{
}

IPPortPair::~IPPortPair()
{
}

void IPPortPair::setIPv4(const IPAddr& ipAddr)
{
    m_ipv4 = ipAddr;
}

const IPAddr& IPPortPair::getIPv4() const
{
    return m_ipv4;
}

void IPPortPair::setIPv6(const IPv6Addr& ipAddr)
{
    m_ipv6 = ipAddr;
}

const IPv6Addr& IPPortPair::getIPv6() const
{
    return m_ipv6;
}

void IPPortPair::setPort(const GUint16 port)
{
    m_port = port;
}

GUint16 IPPortPair::getPort() const
{
    return m_port;
}

NetAddr::NetAddr(const IPAddrType& ipAddrType = IP_TYPE_V4) : m_ipAddrType(ipAddrType)
{
}

NetAddr::~NetAddr()
{
}

void NetAddr::setMacAddr(const MacAddr& macAddr)
{
    m_macAddr = macAddr;
}

const MacAddr& NetAddr::getMacAddr() const
{
    return m_macAddr;
}

void NetAddr::setIPv4Addr(const IPAddr& ipAddr)
{
    m_ipv4Addr = ipAddr;
}

const IPAddr& getIPv4Addr() const
{
    return m_ipv4Addr;
}

void NetAddr::setIPv6Addr(const IPv6Addr& ipAddr)
{
    m_ipv6Addr = ipAddr;
}

const IPv6Addr& NetAddr::getIPv6Addr() const
{
    return m_ipv6Addr;
}

void NetAddr::setPort(const GUint16 port)
{
    m_port = port;
}

GUint16 NetAddr::getPort() const
{
    return m_port;
}

SocketAddr::SocketAddr(const NetAddr& srcAddr, const NetAddr& dstAddr) : m_srcAddr(srcAddr), m_dstAddr(dstAddr)
{
}

SocketAddr::~SocketAddr()
{
}

void SocketAddr::setSrcAddr(const NetAddr& srcAddr)
{
    m_srcAddr = srcAddr;
}

const NetAddr& SocketAddr::getSrcAddr() const
{
    return m_srcAddr;
}

void SocketAddr::setDstAddr(const NetAddr& dstAddr)
{
    m_dstAddr = dstAddr;
}

const NetAddr& SocketAddr::getDstAddr() const
{
    return m_dstAddr;
}

}

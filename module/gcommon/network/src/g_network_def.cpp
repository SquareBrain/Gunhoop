/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_network_def.cpp
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
#include <g_function.h>
#include <g_network_def.h>

namespace gcom {
    
NetWorkConv::NetWorkConv() {}
NetWorkConv::~NetWorkConv() {}

GResult NetWorkConv::ipToInteger(const std::string& str_ip, GUint32& int_ip)
{
    std::list<std::string> split_list;
    IS_NO_R(Convert::splitString(str_ip, '.', split_list));
    IS_NO_RR(split_list.size() == 4, G_ERROR_INVALID_PARAMETERS);
    
    GUint32 ip_array[4] = {0};
    GUint16 i = 0;
    std::list<std::string>::iterator iter = split_list.begin();
    for (; iter != split_list.end(); ++iter)
    {
        ip_array[i] = std::stoul(*iter);
    }
    
    int_ip = (ip_array[0] << 24) + (ip_array[1] << 16) + (ip_array[2] << 8) + ip_array[3];
    
    return G_YES;
}

GResult NetWorkConv::ipToString(const GUint32 int_ip, std::string& str_ip)
{
    GInt8 tmp_buf[16] = {0};  
    sprintf(tmp_buf, "%u.%u.%u.%u",  
        (int_ip & 0xff000000) >> 24,  
        (int_ip & 0x00ff0000) >> 16,  
        (int_ip & 0x0000ff00) >> 8,  
        (int_ip & 0x000000ff));
    
    str_ip.assign(tmp_buf);
        
    return G_YES;
}

GResult NetWorkConv::macToInteger(const std::string& str_mac, GUint64& int_mac)
{
    std::list<std::string> split_list;
    IS_NO_R(Convert::splitString(str_mac, ':', split_list));
    IS_NO_RR(split_list.size() == 6, G_ERROR_INVALID_PARAMETERS);
    
    GUint32 ip_array[6] = {0};
    GUint16 i = 0;
    std::list<std::string>::iterator iter = split_list.begin();
    for (; iter != split_list.end(); ++iter)
    {
        ip_array[i] = std::stoul(*iter);
    }
    
    int_mac = (ip_array[0] << 40) 
            + (ip_array[1] << 32) 
            + (ip_array[2] << 24) 
            + (ip_array[3] << 16) 
            + (ip_array[4] << 8) 
            + ip_array[5];
    
    return G_YES;
}

GResult NetWorkConv::macToInteger(const GInt8 bytes_mac[6], GUint64& int_mac)
{
    int_mac = (bytes_mac[0] << 40) 
            + (bytes_mac[1] << 32) 
            + (bytes_mac[2] << 24) 
            + (bytes_mac[3] << 16) 
            + (bytes_mac[4] << 8) 
            + bytes_mac[5];
    
    return G_YES;
}

GResult NetWorkConv::macToBytes(const std::string& str_mac, GInt8 bytes_mac[6])
{
    std::list<std::string> split_list;
    IS_NO_R(Convert::splitString(str_mac, ':', split_list));
    IS_NO_RR(split_list.size() == 6, G_ERROR_INVALID_PARAMETERS);
    
    GUint16 i = 0;
    std::list<std::string>::iterator iter = split_list.begin();
    for (; iter != split_list.end(); ++iter)
    {
        bytes_mac[i] = std::stoul(*iter);
    }
        
    return G_YES;
}

GResult NetWorkConv::macToBytes(const GUint64 int_mac, GInt8 bytes_mac[6])
{
    bytes_mac[0] = (int_mac & 0xff0000000000) >> 40;
    bytes_mac[1] = (int_mac & 0x00ff00000000) >> 32;
    bytes_mac[2] = (int_mac & 0x0000ff000000) >> 24;
    bytes_mac[3] = (int_mac & 0x000000ff0000) >> 16;
    bytes_mac[4] = (int_mac & 0x00000000ff00) >> 8;
    bytes_mac[5] = (int_mac & 0x0000000000ff);
    
    return G_YES;
}

GResult NetWorkConv::macToString(const GUint64 int_mac, std::string& str_mac)
{
    GInt8 tmp_buf[18] = {0};  
    sprintf(tmp_buf, "%0x:%0x:%0x:%0x:%0x:%0x",  
        (int_mac & 0xff0000000000) >> 40,  
        (int_mac & 0x00ff00000000) >> 32,  
        (int_mac & 0x0000ff000000) >> 24,  
        (int_mac & 0x000000ff0000) >> 16,
        (int_mac & 0x00000000ff00) >> 8,
        (int_mac & 0x0000000000ff));
        
    Convert::toupper(tmp_buf);
    str_mac.assign(tmp_buf);
    
    return G_YES;
}

GResult NetWorkConv::macToString(GInt8 bytes_mac[6], std::string& str_mac)
{
    GInt8 tmp_buf[18] = {0};  
    sprintf(tmp_buf, "%0x:%0x:%0x:%0x:%0x:%0x", 
        bytes_mac[5], 
        bytes_mac[4], 
        bytes_mac[3], 
        bytes_mac[2], 
        bytes_mac[1], 
        bytes_mac[0]);
        
    Convert::toupper(tmp_buf);
    str_mac.assign(tmp_buf);
    
    return G_YES;	
}

IPAddr::IPAddr() : m_ip(0) {}

IPAddr::IPAddr(const GUint32 ip) : m_ip(ip) 
{
    if (m_ip != 0)
    {
    	NetWorkConv::ipToString(m_ip, m_ipStr);
    }
}

IPAddr::IPAddr(const std::string& ip) : m_ip(0), m_ipStr(ip)
{
    if (!m_ipStr.empty())
    {
    	NetWorkConv::ipToInteger(m_ipStr, m_ip);
    }
}

IPAddr::~IPAddr() {}

void IPAddr::setIP(const GUint32 ip)
{
    m_ip = 0;
    m_ipStr.clear();

    if (ip != 0)
    {
    	m_ip = ip;
    	NetWorkConv::ipToString(m_ip, m_ipStr);
    }
}

void IPAddr::setIP(const std::string& ip)
{
    m_ip = 0;
    m_ipStr.clear();
    
    if (!ip.empty())
    {
    	m_ipStr = ip;
    	NetWorkConv::ipToInteger(m_ipStr, m_ip);
    }
}

GUint32 IPAddr::ip() const
{
    return m_ip;
}

const std::string& IPAddr::ipStr() const 
{
    return m_ipStr;
}

MacAddr::MacAddr() : m_mac(0) {}

MacAddr::MacAddr(const GUint64 mac) : m_mac(mac)
{
    if (m_mac != 0)
    {
    	NetWorkConv::macToString(m_mac, m_macStr);
    }
}

MacAddr::MacAddr(const std::string& mac) : m_mac(0), m_macStr(mac)
{
    if (!m_macStr.empty())
    {
    	NetWorkConv::macToInteger(m_macStr, m_mac);
    }
}

void MacAddr::setMac(GUint64 mac)
{
    m_mac = 0;
    m_macStr.clear();
    
    if (mac != 0)
    {
    	m_mac = mac;
    	NetWorkConv::macToString(m_mac, m_macStr);
    }
}

void MacAddr::setMac(const std::string& mac)
{
    m_mac = 0;
    m_macStr.clear();
    
    if (!mac.empty())
    {
    	m_macStr = mac;
    	NetWorkConv::macToInteger(m_macStr, m_mac);
    }
}

GUint64 MacAddr::mac() const
{
    return m_mac;
}

const std::string& MacAddr::macStr() const
{
    return m_macStr;
}

IPPortPair::IPPortPair() : m_port(0) {}

IPPortPair::IPPortPair(const IPAddr& ip_addr, const GUint16 port) 
    : m_ipAddr(ip_addr), m_port(port) {}

IPPortPair::~IPPortPair() {}

void IPPortPair::setIPAddr(const IPAddr& ip_addr)
{
    m_ipAddr = ip_addr;
}

const IPAddr& IPPortPair::ipAddr() const
{
    return m_ipAddr;
}

void IPPortPair::setPort(const GUint16 port)
{
    m_port = port;
}

GUint16 IPPortPair::port() const
{
    return m_port;
}

NetAddr::NetAddr() {}

NetAddr::NetAddr(const MacAddr mac_addr, const IPAddr& ip_addr, const GUint16 port)
    : m_macAddr(mac_addr), m_ip(ip_addr), m_port(port) {}

NetAddr::~NetAddr() {}

void NetAddr::setMacAddr(const MacAddr& mac_addr)
{
    m_macAddr = mac_addr;
}

const MacAddr& NetAddr::macAddr() const
{
    return m_macAddr;
}

void NetAddr::setIPAddr(const IPAddr& ip_addr)
{
    m_ip = ip_addr;
}

const IPAddr& NetAddr::ipAddr() const
{
    return m_ip;
}

void NetAddr::setPort(const GUint16 port)
{
    m_port = port;
}

GUint16 NetAddr::port() const
{
    return m_port;
}

SocketAddr::SocketAddr() {}

SocketAddr::SocketAddr(const NetAddr& src_addr, const NetAddr& dst_addr) 
    : m_srcAddr(src_addr), m_dstAddr(dst_addr) {}

SocketAddr::~SocketAddr() {}

void SocketAddr::setSrcAddr(const NetAddr& src_addr)
{
    m_srcAddr = src_addr;
}

const NetAddr& SocketAddr::srcAddr() const
{
    return m_srcAddr;
}

void SocketAddr::setDstAddr(const NetAddr& dst_addr)
{
    m_dstAddr = dst_addr;
}

const NetAddr& SocketAddr::dstAddr() const
{
    return m_dstAddr;
}

}

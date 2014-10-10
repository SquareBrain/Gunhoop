/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_network_server.cpp
* @version     
* @brief      
* @author   duye
* @date	    2014-10-07
* @note 
*
*  1. 2014-10-07 duye Created this file
* 
*/
#include <g_network_server.h>

namespace gcom {

NetworkServer::NetworkServer() {}

NetworkServer::NetworkServer(const IPPortPair& server_addr, const std::string& net_card = "eth0") 
    : m_serverAddr(server_addr)
    , m_netCard(net_card) {}
    
NetworkServer::~NetworkServer() {}

void NetworkServer::addInterface(NetworkServerInterface* interface)
{
    gsys::AutoLock auto_lock(m_interfaceList.mutex());
    InterfaceList::iterator iter = m_interfaceList.begin();
    for (; iter != m_interfaceList.end(); ++iter)
    {
        if (interface == *iter)
        {
            return;
        }
    }
    
    m_interfaceList.push_back(interface);
}

void NetworkServer::removeInterface(NetworkServerInterface* interface)
{
    gsys::AutoLock auto_lock(m_interfaceList.mutex());
    InterfaceList::iterator iter = m_interfaceList.begin();
    for (; iter != m_interfaceList.end(); ++iter)
    {
        if (*iter == interface)
        {
            m_interfaceList.erase(iter);
            break;
        }
    }
}

const IPPortPair& NetworkServer::getServerAddr() const
{
    return m_serverAddr;
}

const std::string NetworkServer::getNetCard() const
{
    return m_netCard;
}

GResult NetworkServer::run()
{
    return routine();
}
}

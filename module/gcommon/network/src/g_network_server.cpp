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

NetworkServer::NetworkServer() : m_netCard("eth0") {}
NetworkServer::NetworkServer(const IPPortPair& server_addr, const std::string& net_card) 
    : m_serverAddr(server_addr)
    , m_netCard(net_card) {}
    
NetworkServer::~NetworkServer() {}

GResult NetworkServer::keepWork()
{
    switch (state())
    {
        case G_SERVER_INIT:
            return start();
            break;
        case G_SERVER_WORK:
            return G_YES;
            break;
        case G_SERVER_STOP:
        case G_SERVER_FAULT:
            return restart();
            break;
        default:
            break;
    }
    
    return G_NO;
}

GResult NetworkServer::addObserver(NetworkServerObserver* observer)
{
    IS_NULL_RE(observer);
    gsys::AutoLock auto_lock(m_observerList.mutex());
    ObserverList::const_iterator iter = m_observerList.begin();
    for (; iter != m_observerList.end(); ++iter)
    {
        if (*iter == observer)
        {
            m_observerList.push_back(observer);
			return G_YES;
        }
    }
    
    return G_NO;
}

GResult NetworkServer::removeObserver(NetworkServerObserver* observer)
{
    IS_NULL_RE(observer);
    gsys::AutoLock auto_lock(m_observerList.mutex());
    m_observerList.remove(observer);
    return G_YES;
}

void NetworkServer::setServerAddr(const IPPortPair& server_addr)
{
    m_serverAddr = server_addr; 
}

const IPPortPair& NetworkServer::serverAddr() const
{
    return m_serverAddr;
}
    
GResult NetworkServer::isValidServerAddr() const
{
    if (m_serverAddr.ipAddr().ipStr().empty())
    {
    	return G_NO;
    }    
    
    return G_YES;
}

void NetworkServer::setNetCard(const std::string& net_card)
{
    m_netCard = net_card;
}

const std::string& NetworkServer::netCard() const
{
    return m_netCard;
}

void NetworkServer::setState(const ServerState state)
{
    m_state = state;    
}

const ServerState& NetworkServer::state() const
{
    return m_state;
}

const NetworkServer::ObserverList& NetworkServer::observerList() const
{
    return m_observerList;
}

GResult NetworkServer::run()
{
    return routine();
}

}

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

void NetworkServer::addObserver(NetworkServerObserver* observer)
{
    IS_NULL_R(observer);
    
    gsys::AutoLock auto_lock(m_observerList.mutex());
    ObserverList::iterator iter = m_observerList.begin();
    for (; iter != m_observerList.end(); ++iter)
    {
        if (*iter == observer)
        {
            return;
        }
    }
    
    m_interfaceList.push_back(observer);
}

void NetworkServer::removeObserver(NetworkServerObserver* observer)
{
    IS_NULL_R(observer);
    
    gsys::AutoLock auto_lock(m_observerList.mutex());
    ObserverList::iterator iter = m_observerList.begin();
    for (; iter != m_observerList.end(); ++iter)
    {
        if (*iter == observer)
        {
            m_interfaceList.erase(iter);
            break;
        }
    }
}

const IPPortPair& NetworkServer::serverAddr() const
{
    return m_serverAddr;
}

const std::string& NetworkServer::netCard() const
{
    return m_netCard;
}

const ServerState& NetworkServer::state() const
{
    return m_state;
}

ObserverList& NetworkServer::observerList() const
{
    return m_observerList;
}

GResult NetworkServer::run()
{
    return routine();
}

GResult NetworkServerMonitor::keepWork(NetworkServer* server)
{
    IS_NULL_R(server);
    
    switch (server->state)
    {
        case SERVER_INIT:
            return server->start();
            break;
        case SERVER_WORK:
            return G_YES;
            break;
        case SERVER_STOP:
        case SERVER_FAULT:
            return server->restart();
            break;
        default:
            break;
    }
    
    return G_NO;
}

}

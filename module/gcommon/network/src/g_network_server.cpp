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
#include <g_utils.h>
#include <g_network_server.h>

namespace gcom {
    
//static const GInt8* G_LOG_PREFIX = "gohoop.gcom.network.server";
    
BaseMessage::BaseMessage() : m_dataLen(0), m_orgData(nullptr) {}
BaseMessage::~BaseMessage() 
{
    clear();
}   
    
void BaseMessage::setData(GInt8* data, const GUint64 len)
{
    if (m_orgData != nullptr)
    {
        clear();
    }
    
    m_dataLen = len;
    m_orgData = data;
}
    
GInt8* BaseMessage::data() const
{
    return m_orgData;
}

GUint64 BaseMessage::length() const
{
    return m_dataLen;
}
    
void BaseMessage::clear()
{
    delete [] m_orgData;
    m_orgData = nullptr;    
}
    
///////////////////////////////////////////////////////////////////
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

ClientAgent::ClientAgent() : m_sockfd(-1) {}
ClientAgent::ClientAgent(const GInt32 sockfd, const NetAddr& clientAddr)
    : m_sockfd(sockfd), m_netAddr(clientAddr) {}
    
ClientAgent::~ClientAgent() {}

void ClientAgent::setSockfd(const GInt32 sockfd)
{
    m_sockfd = sockfd;  
}
    
GInt32 ClientAgent::sockfd() const
{
    return m_sockfd;
}

void ClientAgent::setClientAddr(const NetAddr& clientAddr)
{
    m_netAddr = clientAddr;   
}
    
const NetAddr& ClientAgent::clientAddr() const
{
    return m_netAddr;
}
    
}

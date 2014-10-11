/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_network_client.cpp
* @version     
* @brief      
* @author   duye
* @date	    2014-09-30
* @note 
*
*  1. 2014-09-30 duye Created this file
* 
*/
#include <g_network_client.h>

namespace gcom {

NetworkClient::NetworkClient() {}
NetworkClient::NetworkClient(const IPPortPair& server_addr) 
  : m_serverAddr(server_addr)
  , m_state(CLIENT_INIT) {}

NetworkClient::~NetworkClient() {}

const IPPortPair& NetworkClient::serverAddr() const
{
    return m_serverAddr;
}

const std::string& NetworkClient::netCard() const
{
    return m_netCard;
}

void NetworkClient::setState(const ClientState& state)
{
    m_state = state;
}

const ClientState& NetworkClient::state() const
{
    return m_state;
}

GResult NetworkClient::addObserver(NetworkClientObserver* observer)
{
    IS_NULL_RE(observer);
    gsys::AutoLock auto_lock(m_observerList.mutex());
    ObserverList::const_iterator iter = m_observerList.begin();
    for (; iter != m_observerList.end(); ++iter)
    {
        if (*iter == observer)
        {
            return G_YES;
        }
    }

    m_observerList.push_back(observer);
    return G_YES;
}

GResult NetworkClient::removeObserver(NetworkClientObserver* observer)
{
    IS_NULL_RE(observer);
    gsys::AutoLock auto_lock(m_observerList.mutex());
    m_observerList.remove(observer);
    return G_YES;
}

GResult NetworkClient::run()
{
    return this->msgLoop();
}

}

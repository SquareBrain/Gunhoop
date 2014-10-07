/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_network_client.cpp
* @version     
* @brief      
* @author	duye
* @date		2014-09-30
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
  , m_connectState(CONNECT_INIT) {}

NetworkClient::~NetworkClient() {}

const IPPortPair& NetworkClient::getServerAddr() const
{
    return m_serverAddr;
}

const ClientConnectState& NetworkClient::getConnectState() const
{
    return m_connectState;
}

GResult NetworkClient::addObserver(NetworkClientInterface* observer)
{
    IS_YES_R(findObserver(observer));
    m_observerList.push_back(observer);
	return G_YES;
}

GResult NetworkClient::removeObserver(NetworkClientInterface* observer)
{
    m_observerList.remove(observer);
	return G_YES;
}

GResult NetworkClient::run()
{
    return this->msgLoop();
}

GResult NetworkClient::findObserver(NetworkClientInterface* observer)
{
    ObserverList::const_iterator iter = m_observerList.begin();
    for (; iter != m_observerList.end(); ++iter)
    {
        if (*iter == observer)
        {
            return G_YES;
        }
    }
    
    return G_NO;    
}

}

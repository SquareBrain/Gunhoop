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
    
NetworkClient::NetworkClient() : m_connectState(CONNECT_INIT) {}

NetworkClient::NetworkClient(const IPPortPair& server_addr) 
    : m_serverAddr(server_addr)
    , m_connectState(CONNECT_INIT) {}
    
NetworkClient::~NetworkClient() {}

void NetworkClient::setServerAddr(const IPPortPair& server_addr)
{
    m_serverAddr = server_addr;
}

const IPPortPair& NetworkClient::getServerAddr() const
{
    return m_serverAddr;
}

void NetworkClient::setConnectState(const ClientConnectState& connect_state)
{
    m_connectState = connect_state;
}

const ClientConnectState& NetworkClient::getConnectState() const
{
    return m_connectState;
}

}

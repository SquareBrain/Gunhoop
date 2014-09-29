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
NetworkClient::NetworkClient(const IPPortPair& ip_port_pair) : m_ipPortPair(ip_port_pair) {}
NetworkClient::~NetworkClient() {}

}

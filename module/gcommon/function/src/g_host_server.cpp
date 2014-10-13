/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_host_server.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-10-13
* @note 
*
*  1. 2014-10-13 duye Created this file
* 
*/
#include <g_host_server.h>

HostServer::HostServer() : m_serverState(G_HSERVER_INIT) {}
HostServer::~HostServer() {}

void HostServer::setState(const HostServerState& state) 
{ 
    m_state = state; 
}

const HostServerState& HostServer::state() const 
{ 
    return m_state; 
}

GResult HostServer::run() 
{ 
    return routine(); 
}	

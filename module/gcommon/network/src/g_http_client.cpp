/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_http_client.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-08-29
* @note 
*
*  1. 2014-08-29 duye Created this file
* 
*/
#include <g_http_client.h>

namespace gcom {

HttpClient::HttpClient() {}
HttpClient::HttpClient(const IPPortPair& server_addr) : m_serverAddr(server_addr) {}
HttpClient::~HttpClient() {}

}

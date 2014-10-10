/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_server_factory.h
* @version     
* @brief      
* @author   duye
* @date	    2014-10-10
* @note 
*
*  1. 2014-10-10 duye Created this file
* 
*/
#pragma once

#include <g_network_server.h>

namespace gcom {
    
/**
 * @brief server type
 */
typedef enum
{
    SERVER_UNKNOWN = 0,
    SERVER_FTP,
    SERVER_HTTP,
    SERVER_RPC,
    SERVER_TCP,
    SERVER_UDP,
    SERVER_CLI
} ServerType;
    
/**
 * @brief server factory
 */
class ServerFactory
{
public:
    ServerFactory() {}
    virtual ~ServerFactory() {}
    
    virtual void createServer(const ServerType& server_type) = 0;
    virtual void destroyServer(NetworkServer* server) = 0;
};

}

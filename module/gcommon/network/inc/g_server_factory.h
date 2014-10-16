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
    G_SERVER_UNKNOWN = 0,
    G_SERVER_FTP,
    G_SERVER_HTTP,
    G_SERVER_RPC,
    G_SERVER_TCP,
    G_SERVER_UDP,
    G_SERVER_CLI
} ServerType;
    
/**
 * @brief server factory
 */
class ServerFactory : public Singleton<ServerFactory>
{
public:
    ServerFactory();
    ~ServerFactory();
    
    /**
     * @brief create a new server
     * @param [in] server_type : server type
     * @return server object
     */
    NetworkServer* create(const ServerType& server_type);
    
    /**
     * @brief destroy server
     * @param [in] server_type : server type
     * @param [in] server : server object
     */
    void destroy(const ServerType& server_type, NetworkServer* server);
};

}

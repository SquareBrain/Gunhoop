/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_client_factory.h
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

#include <g_singleton.h>
#include <g_network_client.h>

namespace gcom {
    
/**
 * @brief server type
 */
typedef enum
{
    G_CLIENT_UNKNOWN = 0,
    G_CLIENT_FTP,
    G_CLIENT_HTTP,
    G_CLIENT_RPC,
    G_CLIENT_TCP,
    G_CLIENT_UDP,
    G_CLIENT_CLI
} ClientType;
    
/**
 * @brief server factory
 */
class ClientFactory : public Singleton<ClientFactory>
{
public:
    ClientFactory();
    ~ClientFactory();
    
    /**
     * @brief create new client
     * @param [in] client_type : client type
     */
    NetworkClient* create(const ClientType& client_type);
    
    /**
     * @brief destroy client
     * @param [in] client_type : client type
     * @param [in] client : client
     */
    void destroy(const ClientType& client_type, NetworkClient* client);
};

}

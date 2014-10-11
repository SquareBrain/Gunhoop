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

#include <g_network_client.h>

namespace gcom {
    
/**
 * @brief server type
 */
typedef enum
{
    CLIENT_UNKNOWN = 0,
    CLIENT_FTP,
    CLIENT_HTTP,
    CLIENT_RPC,
    CLIENT_TCP,
    CLIENT_UDP,
    CLIENT_CLI
} ClientType;
    
/**
 * @brief server factory
 */
class ClientFactory : public Singleton<ClientFactory>
{
public:
    ClientFactory() {}
    virtual ~ClientFactory() {}
    
    virtual NetworkClient* createClient(const ClientType& client_type);
    virtual NetworkClient* createClient(const std::string& client_name);
    void destroyClient(NetworkClient* client);
};

}

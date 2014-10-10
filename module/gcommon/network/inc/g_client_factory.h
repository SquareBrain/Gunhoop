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
    SERVER_UNKNOWN = 0,
    SERVER_FTP,
    SERVER_HTTP,
    SERVER_RPC,
    SERVER_TCP,
    SERVER_UDP,
    SERVER_CLI
} ClientType;
    
/**
 * @brief server factory
 */
class ClientFactory
{
public:
    ClientFactory() {}
    virtual ~ClientFactory() {}
    
    virtual NetworkClient* createClient(const ClientType& client_type) = 0;
    virtual void destroyClient(NetworkClient* client) = 0;
};

}

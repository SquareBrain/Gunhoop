/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_network_client.h
* @version     
* @brief      
* @author	duye
* @date		2014-09-30
* @note 
*
*  1. 2014-09-30 duye Created this file
* 
*/

#include <g_system.h>
#include <g_network_def.h>

namespace gcom {
    
/**
 * @brief connect state
 */
typedef enum
{
    CONNECT_INIT,
    CONNECT_ON,
    CONNECT_OFF
} ClientConnectState;

/**
 * @brief network client component base class
 */
class NetworkClient
{
public:
    NetworkClient();
    explicit NetworkClient(const IPPortPair& ip_port_pair);
    virtual ~NetworkClient();
    
    virtual GResult connect() = 0;
    
protected:
    IPPortPair  m_ipPortPair;
};
}

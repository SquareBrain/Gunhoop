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

namespace gcom {
/**
 * @brief network client component base class
 */
class NetworkClient
{
public:
    NetworkClient() {}
    virtual ~NetworkClient() {}
    
    virtual GResult connect() = 0;
};
}

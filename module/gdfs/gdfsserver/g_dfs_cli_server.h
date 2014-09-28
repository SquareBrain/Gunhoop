/************************************************************************************
**  
* @copyright (c) 2010-2019, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_dfs_cli_server.h
* @version     
* @brief      
* @author   duye
* @date     2014-09-28
* @note 
*
*  1. 2014-09-28 duye Created this file
* 
*/

#pragma once

#include <g_common.h>


namespace gdfs {
    
/**
 * @brief DFS http service
 */
class DFSCliService : public gcom::TcpService
{
public:
    DFSCliService(const IPPortPair& ipPortPair);
    ~DFSCliService();
    
    /**
     * @brief startup http server
     * @return G_YES/G_NO
     */
    GResult start();
    
    /**
     * @brief stop http server
     * @return G_YES/G_NO
     */
    GResult stop();
    
private:
    // inherit form base class gcom::HttpService
    GResult onRequest();
    
private:
    IPPortPair      m_ipPortPair;
};

}

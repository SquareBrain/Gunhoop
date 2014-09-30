/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_dfs_client.h
* @version     
* @brief      
* @author   duye
* @date     2014-09-30
* @note 
*
*  1. 2014-09-30 duye Created this file
* 
*/
#pragma once

#include <g_rpc_client.h>

namespace gdfs {

class DfsClient : public gcom::NetworkClientInterface
{
public:
    DfsClient();
    ~DfsClient();
    
    /**
     * @brief handle server response message, user need implement
     * @param [in] msg : message data
     * @return G_YES/G_NO
     */
    GResult serverResponse(const GInt8* msg);
    
private:    
};

}

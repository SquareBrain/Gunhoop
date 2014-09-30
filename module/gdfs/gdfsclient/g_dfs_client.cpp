/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_dfs_client.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-09-30
* @note 
*
*  1. 2014-09-30 duye Created this file
* 
*/
#include <g_system.h>
#include <g_logger.h>
#include <g_dfs_client.h>

namespace gdfs {
    
static const GInt8* LOG_PREFIX = "dfs.client";
  
DfsClient::DfsClient() {}
DfsClient::~DfsClient() {}

GResult DfsClient::start()
{
    // start client thread
    this->startTask();
    
    return G_YES;    
}

GResult DfsClient::onServerResponse(const GInt8* msg)
{
    G_LOG_IN();
    // TODB
    // handle server response
    G_LOG_OUT();
    
    return G_YES;
}

GResult DfsClient::run()
{
    G_LOG_IN();
    
    // add observer
    m_rpcClient.addObserver(this);
    
    // event loop
    while (true)
    {
        if (m_rpcClient.)
        char* cmd = gets();
        
        if (cmd == nullptr)
        {
            
        }
        else if (cmd == )
        {
            
        }
    }
    
    G_LOG_OUT();
    
    return G_YES;
}

}

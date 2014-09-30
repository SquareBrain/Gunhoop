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
#include <g_dfs_client.h>

namespace gdfs {
  
DfsClient::DfsClient() {}
DfsClient::~DfsClient() {}

GResult DfsClient::start()
{
    return G_YES;    
}

GResult DfsClient::serverResponse(const GInt8* msg)
{
    // TODB
    // handle server response
    return G_YES;
}

GResult DfsClient::run()
{
    // event loop
    while (true)
    {
        char* cmd = gets();
        
        if (cmd)
    }
    
    return G_YES;
}

}

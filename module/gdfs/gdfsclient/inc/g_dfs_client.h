/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_dfs_client.h
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

#include <g_system.h>
#include <g_rpc_client.h>

namespace gdfs {

class DfsClient : public gcom::NetworkClientInterface, public gsys::ThreadTask
{
public:
    DfsClient();
    ~DfsClient();
    
    /**
     * @brief start dfs client
     * @param [in] server_ip : server ip
     * @param [in] server_port : server port
     * @param [in] interface : communication net card in local
     * @return G_YES/G_NO
     */
    GResult start(const std::string& server_ip, const GUint16 server_port, const std::string& interface = "eth0");
    
    /**
     * @brief handle server response message, user need implement
     * @param [in] msg : message data
     * @return G_YES/G_NO
     */
    GResult onServerResponse(const GInt8* msg);
    
private:
    // inherite from base class gsys::ThreadTask, to create new thread
    // to be called, when call this->startTask();
    GResult run();
    
private:    
    gcom::RpcClient     m_rpcClient;
};

}

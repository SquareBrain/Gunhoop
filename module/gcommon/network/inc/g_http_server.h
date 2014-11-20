/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_http_server.h
* @version     
* @brief      
* @author   duye
* @date	    2014-08-29
* @note 
*
*  1. 2014-08-29 duye Created this file
* 
*/
#pragma once

#include <g_network_server.h>

namespace gcom {
    
/**
 * @brief http client request message
 */
class HttpReqMessage : public BaseMessage
{
public:
    HttpReqMessage() {}
    ~HttpReqMessage() {}
};
    
/**
 * @brief http server response message
 */
class HttpRespMessage : public BaseMessage
{
public:
    HttpRespMessage() {}
    ~HttpRespMessage() {}
};
    
/**
 * @brief http server observer
 */
class HttpServerObserver : public NetworkServerObserver
{
public:
    virtual ~HttpServerObserver();
    
    /**
     * @brief http client request message handler
     * @param [in] reqMsg : request message
     * @return G_YES/G_NO
     */    
    virtual GResult onRequest(HttpReqMessage* reqMsg) = 0;
    
private:    
    /**
     * @brief message handler
     * @param [in] message : message
     * @return G_YES/G_NO
     */
    GResult onMessage(BaseMessage* message);
};
    
/**
 * @brief http server
 */
class HttpServer : public NetworkServer
{
public:
    HttpServer();
    
    /**
     * @brief constructor
     * @param [in] server_addr : ftp server address
     * @param [in] net_card : network card for communication, defualt is eth0
     */    
    explicit HttpServer(const IPPortPair& server_addr, const std::string& net_card = "eth0");
    virtual ~HttpServer();
	
    /**
     * @brief startup service
     * @return G_YES/G_NO
     */       
    GResult start();

    /**
     * @brief startup service
     * @param [in] server_addr : ftp server address
     * @param [in] net_card : network card for communication, defualt is eth0
     * @return G_YES/G_NO
     */       
    GResult start(const IPPortPair& server_addr, const std::string& net_card = "eth0");

    /**
     * @brief restart service
     * @return G_YES/NO
     */
    GResult restart();
    
    /**
     * @brief stop service
     * @return G_YES/G_NO
     */
    GResult stop();

    /**
     * @brief message loop handle, new thread
     * @note derive class implemention
     * @return G_YES/G_NO
     */
    GResult routine();      
};
}

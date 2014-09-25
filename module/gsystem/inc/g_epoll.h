/*************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_epoll.h
* @version     
* @brief      
* @author	duye
* @date		2014-09-25
* @note 
*
*  1. 2014-09-25 duye Created this file
* 
*/

#pragma once

#include <g_type.h>
#include <g_socket.h>

namespace gsys {
  
class EpollEvent
{
public:
    EpollEvent();
    ~EpollEvent();
};

/**
 * @brief epoll server user interface
 */

class EpollServerInterface
{
public:
    virtual EpollServerInterface();
    virtual GResult onAccept(const EpollEvnet& epollEvent) = 0;
};

/**
 * @brief epoll server wrapper
 */
class EpollServer : public ThreadTask
{
public:
    explicit EpollServer(EpollServerInterface* interface = nullptr);
    explicit EpollServer(const GUint32 server_ip, const GUint16 port, EpollServerInterface* interface = nullptr);
    ~EpollServer();
    
    GResult init();
    
    void setServerIP(const GUint32 server_ip);
    void setPort(const GUint16 port);
    
private:
    GResult run();
    
private:
    Socket      m_socket;
};

}

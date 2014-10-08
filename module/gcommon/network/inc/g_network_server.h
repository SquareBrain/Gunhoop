/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_network_server.h
* @version     
* @brief      
* @author	duye
* @date		2014-08-29
* @note 
*
*  1. 2014-08-29 duye Created this file
* 
*/
#pragma once

#include <string>
#include <g_system.h>
#include <g_network_def.h>

namespace gcom {

class NetworkServer : public gsys::ThreadTask
{
public:
	NetworkServer();
	/**
	 * @brief constructor
	 * @param [in] server_addr : ftp server address
	 * @param [in] net_card : network card for communication, defualt is eth0
	 */    
    explicit NetworkServer(const IPPortPair& server_addr, const std::string& net_card = "eth0");
	virtual ~NetworkServer();

    /**
     * @brief startup service
	 * @return G_YES/G_NO
	 */       
	virtual GResult start() = 0;

    /**
     * @brief startup service
	 * @param [in] server_addr : ftp server address
	 * @param [in] net_card : network card for communication, defualt is eth0
	 * @return G_YES/G_NO
	 */       
	virtual GResult start(const IPPortPair& server_addr, const std::string& net_card = "eth0") = 0;

    /**
     * @brief stop service
     * @return G_YES/G_NO
     */
	virtual GResult stop() = 0;

    /**
     * @brief message loop handle, new thread
     * @note derive class implemention
     * @return G_YES/G_NO
     */
    virtual GResult msgLoop() = 0;    

    /**
     * @brief get server address
     * @return server address
     */
    const IPPortPair& getServerAddr() const; 

     /**
     * @brief get net card
     * @return net card
     */
    const std::string getNetCard() const;    
	
private:	
	// inherit from base class gsys::ThreadTask
	// to run new service thread
	GResult run();

private:
    // server address
    IPPortPair  m_serverAddr;
    std::string m_netCard;
};

}

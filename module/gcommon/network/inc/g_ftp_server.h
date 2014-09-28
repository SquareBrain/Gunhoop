/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_ftp_server.h
* @version     
* @brief      
* @author   duye
* @date     2014-09-24
* @note 
*
*  1. 2014-09-24 duye Created this file
* 
*/

#pragma once

#include <g_system.h>

namespace gcom {

class FtpClient
{
public:
	FtpClient();
	explicit FtpClient(const IPPortPair& server);
	~FtpClient();
};
}

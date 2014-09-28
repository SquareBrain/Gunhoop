/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_network_service.h
* @version     
* @brief      
* @author   duye
* @date     2014-08-29
* @note 
*
*  1. 2014-08-29 duye Created this file
* 
*/

#pragma once

#include <g_system.h>

namespace gcom {

class NetworkService : public gsys::ThreadTask
{
public:
	NetworkService() {}
	virtual ~NetworkService() {}
	
	virtual GResult start() = 0;
	virtual GResult stop() = 0;
	virtual GResult run() = 0;
};

}

/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_slave.h
* @version     
* @brief      
* @author   duye
* @date     2014-06-22
* @note 
*
*  1. 2014-06-22 duye Created this file
* 
*/
#pragma once

#include <g_type.h>

G_NS_GMAPREDUCE_BEG

class Slave 
{
public:
	Slave();
	~Slave();
	GResult Start();
};

G_NS_GMAPREDUCE_END

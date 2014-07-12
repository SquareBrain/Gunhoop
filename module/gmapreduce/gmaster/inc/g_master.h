/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_master.h
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

#include <g_common.h>
#include <g_system.h>

G_NS_GMAPREDUCE_BEG

class Master
{
public:
	Master();
	~Master();

	GResult start();
};

G_NS_END

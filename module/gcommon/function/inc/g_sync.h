/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_sync.h
* @version     
* @brief      
* @author   	duye
* @date     	2014-07-18
* @note 
*
*  1. 2014-07-18 duye Created this file
* 
*/
#pragma once

#include <g_system.h>
#include <g_namespace.h>

/** 
 * @brief synchronized code scope
 */
#define synchronized(mtx) for (GInt32 i = 0; i < 1; i ++) for (GAutoLock lock(mtx); i < 1; i ++)

G_NS_GCOMMON_BEG

/** 
 * @brief secrity object template
 */
template <class T>
class SecrityObj : public T, public GMutex
{
public:
	SecrityObj() {}
	~SecrityObj() {}
};

G_NS_END

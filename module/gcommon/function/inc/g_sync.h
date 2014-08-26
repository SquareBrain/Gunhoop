/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_sync.h
* @version     
* @brief      
* @author	duye
* @date		2014-07-18
* @note 
*
*  1. 2014-07-18 duye Created this file
* 
*/
#pragma once

#include <g_system.h>

using namespace gsys;

/** 
 * @brief synchronized code scope
 */
#define synchronized(mtx) for (GInt32 i = 0; i < 1; i ++) for (AutoLock lock(mtx); i < 1; i ++)

namespace gcom {

/** 
 * @brief secrity object template
 */
template <class T>
class SecrityObj : public T, public Mutex
{
public:
	SecrityObj() {}
	~SecrityObj() {}
};
}

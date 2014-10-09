/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_singleton.h
* @version     
* @brief      
* @author   duye
* @date     2014-06-18
* @note 
*
*  1. 2014-06-18 duye Created this file
* 
*/
#pragma once

#include <g_system.h>

namespace gcom {

/** 
 * @brief singletion template
 */
template<typename T>
class Singleton
{
public:
    static T& getInstance();
    
protected:
    Singleton() {}
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);
};

template<typename T>
T& Singleton<T>::GetInstance()
{
    static T t;
    return t;
}
}

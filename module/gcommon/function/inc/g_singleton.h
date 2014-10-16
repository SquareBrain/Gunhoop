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
    static T& instance();
    
protected:
    Singleton() {}
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);
};

template<typename T>
T& Singleton<T>::instance()
{
    static T t;
    return t;
}

template<typename T>
class SingletonObj : public T
{
public:
    static T& instance();
    
protected:
    SingletonObj() {}
    SingletonObj(const SingletonObj&);
    SingletonObj& operator=(const SingletonObj&);
};

template<typename T>
T& SingletonObj<T>::instance()
{
    static T t;
    return t;
}

}

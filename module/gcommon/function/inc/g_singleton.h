/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_singleton.h
* @version     
* @brief      
* @author   	duye
* @date     	2014-06-18
* @note 
*
*  1. 2014-06-18 duye Created this file
* 
*/

#pragma once

#include <g_system.h>
#include <g_namespace.h>

G_NS_GCOMMON_BEG

/** 
 * @brief singletion template
 */
template<typename T>
class Singleton
{
public:
    typedef T object_type;
    
public:
    static T& GetInstance();
    
protected:
    Singleton() {}
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);

private:
    static T m_instance;
};

template<typename T> typename Singleton<T>::object_type Singleton<T>::m_instance;

template<typename T>
T& Singleton<T>::GetInstance()
{
    return &m_instance;
}

G_NS_END

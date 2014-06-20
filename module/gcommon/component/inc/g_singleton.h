/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_singleton.h
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

#include <g_type.h>

G_NS_GCOMMON_BEG

template<typename T>
class Singleton
{
public:
    typedef T object_type;
    
public:
    static T* GetInstance();
    
protected:
    Singleton() {}
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);

private:
    static T* m_instance;
};

template<typename T> typename Singleton<T>::object_type* Singleton<T>::m_instance = NULL;

template<typename T>
T* Singleton<T>::GetInstance()
{
    if (m_instance == NULL)
    {
        static T instance;
        m_instance = &instance;
    }

    return m_instance;
}

G_NS_GCOMMON_END

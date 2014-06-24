/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Gohoop Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_objpool.h
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

#include <list>
#include <g_type.h>

G_NS_GCOMMON_BEG

/** 
 *  一个示范类，描述在此
 */
class ObjPoolIf
{
public:
    virtual ~ObjPoolIf() {}
    virtual GResult Init() = 0;
    virtual GResult Uninit() = 0;
};

/** 
 *  一个示范类，描述在此
 */
template <class T>
class ObjPool
{
public:
    typedef std::list<T*> ObjList;

public:
    ObjPool();
    ObjPool(const GUint32 count);
    ~ObjPool();
    
    /**
     * 一个普通函式 描述和参数等等的叙述
     * @param a 整数参数
     * @param s 字串指针参数
     * @see Test() 参看..
     * @return 返回值描述
     */
    void SetCount(const GUint32 count);

private:
    ObjPool(const ObjPool<T>&);
    ObjPool<T>& operator=(const ObjPool<T>&); 

    void InitObjPool();
    void ReleaseObjPool();
    
private:    
    GUint32     m_count;
    ObjList     m_used;
    ObjList     m_idle;
};

template <class T>
ObjPool<T>::ObjPool() : m_count(0)
{
}

template <class T>
ObjPool<T>::ObjPool(const GUint32 count) : m_count(count)
{
    InitObjPool();
}

template <class T>
ObjPool<T>::~ObjPool()
{
    ReleaseObjPool();   
}

template <class T>
void ObjPool<T>::SetCount(const GUint32 count)
{
    ReleaseObjPool();
    m_count = count;
    InitObjPool();
}

template <class T>
void ObjPool<T>::InitObjPool()
{
    if (m_count == 0)
    {
        return;
    }
    
    for (GUint32 i = 0; i < m_count; i++)
    {
        T* t = new T;
        if (t->Init())
        {
            m_idle.push_back(new T); 
        }
    }
}

template <class T>
void ObjPool<T>::ReleaseObjPool()
{
    if (m_count == 0)
    {
        return;
    }
}

G_NS_GCOMMON_END

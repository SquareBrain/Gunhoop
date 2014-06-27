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
 *  provide user inheritance
 */
class ObjPoolIf
{
public:
    virtual ~ObjPoolIf() {}

    /**
     * be call by object pool, before used object
     * @return G_YES/G_NO
     */    
    virtual GResult init() = 0;

    /**
     * be call by object pool, after using finished
     * @return G_YES/G_NO
     */   
    virtual GResult uninit() = 0;
};

/** 
 * mamagement object set
 */
template <class T>
class ObjPool
{
public:
    typedef std::list<T*> ObjList;

public:
    ObjPool();
    /**
     * constructor
     * @param count : the size of object pool
     */    
    ObjPool(const GUint32 count);
    ~ObjPool();
    
    /**
     * set the size of object pool
     * @param count : the size of object pool
     */
    void setCount(const GUint32 count);

    /**
     * get the size of object pool
     * @return count : the size of object pool
     */
    GUint32 getCount() const;

private:
    ObjPool(const ObjPool<T>&);
    ObjPool<T>& operator=(const ObjPool<T>&); 

    void initObjPool();
    void releaseObjPool();
    
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
    initObjPool();
}

template <class T>
ObjPool<T>::~ObjPool()
{
    releaseObjPool();   
}

template <class T>
void ObjPool<T>::setCount(const GUint32 count)
{
    releaseObjPool();
    m_count = count;
    initObjPool();
}

template <class T>
GUint32 ObjPool<T>::getCount() const
{
    return m_count;
}

template <class T>
void ObjPool<T>::initObjPool()
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
void ObjPool<T>::releaseObjPool()
{
    if (m_count == 0)
    {
        return;
    }
}

G_NS_END

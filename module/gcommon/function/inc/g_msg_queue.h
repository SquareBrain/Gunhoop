/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_msg_queue.h
* @version     
* @brief      
* @author   duye
* @date     2014-10-13
* @note 
*
*  1. 2014-10-13 duye Created this file
* 
*/
#pragma once

#include <list>
#include <g_system.h>

namespace gcom {
  
/**
 * @brief message queue
 */
template <type T>
class MsgQueue
{
public:
    MsgQueue();
    ~MsgQueue();
    
    /**
     * @brief push message
     * @param [in] msg : message
     */
    GResult push(T* msg);
    
    /**
     * @brief pop message
     * @return message
     */
    T* pop();
    
private:
    std::list<T*> m_msgList;
    gsys::Condition m_cond;
};

}

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
 * @brief access mode
 */
typedef enum
{
    G_POP_BLOCK = 0,
    G_POP_NOBLOCK
} AccessMode;

/**
 * @brief message queue
 */
template <class T>
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
    std::list<T*>   m_msgList;
    gsys::Condition m_cond;
};

template <class T>
MsgQueue::MsgQueue() : m_mode(G_POP_BLOCK) {}

template <class T>
GResult MsgQueue::push(T* msg)
{
    
	return G_YES;
}

template <class T>
T* MsgQueue::pop(const AccessMode& mode = G_POP_BLOCK)
{
    
	return G_NO;	
}

}

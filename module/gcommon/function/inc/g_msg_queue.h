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

#include <queue>
#include <g_system.h>

namespace gcom {

static const GUint64 G_DEF_QUEUE_SIZE = 1024;
  
/**
 * @brief access mode
 */
typedef enum
{
    G_MSG_BLOCK = 0,
    G_MSG_NONBLOCK
} MsgMode;

/**
 * @brief message queue
 */
template <class T>
class MsgQueue
{
public:
    /**
     * @brief constructor
     * @param [in] queue_size : queue size, default is G_DEF_QUEUE_SIZE
     */
    MsgQueue(const GUint64 queue_size = G_DEF_QUEUE_SIZE);
    ~MsgQueue();
    
    /**
     * @brief push message
     * @param [in] msg : message
     */
    GResult push(T* msg);
    
    /**
     * @brief pop message
     * @param [in] mode : pop block
     * @return message
     */
    T* pop(const MsgMode& mode = G_POP_BLOCK);
    
private:
    std::queue<T*>      m_queue;
    gsys::Semaphore     m_semaphore;
    GUint64             m_queueSize;
};

template <class T>
MsgQueue::MsgQueue(const GUint64 queue_size) : m_queueSize(queue_size) {}

template <class T>
GResult MsgQueue::push(T* msg)
{
    if (m_queue.size() >= m_queueSize)
    {
        return G_NO;
    }

    m_queue.push(msg);
    m_semaphore.post();
    
    return G_YES;
}

template <class T>
T* MsgQueue::pop(const MsgMode& mode)
{
    if (mode == G_POP_BLOCK)
    {
        m_semaphore.wait();
    }
    else if (mode = G_MSG_NONBLOCK)
    {
        m_semaphore.tryWait();   
    }
    else
    {
        return nullptr;	
    }
    
    T* msg = m_queue.front();
    m_queue.pop();
    
    return msg;	
}
}

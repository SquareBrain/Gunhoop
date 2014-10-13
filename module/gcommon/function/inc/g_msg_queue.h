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
template <type T>
class MsgQueue
{
public:
    MsgQueue();
    
    /**
     * @brief construtor
     * @param [in] mode : pop function mode, POP_BLOCK:block, POP_NOBLOCK:noblock
     */
    MsgQueue(const AccessMode& mode = G_POP_BLOCK);
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
    AccessMode      m_mode;
    gsys::Condition m_cond;
};

}

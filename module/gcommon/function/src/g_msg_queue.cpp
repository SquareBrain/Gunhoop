/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file     g_msg_queue.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-10-13
* @note 
*
*  1. 2014-10-13 duye Created this file
* 
*/
#include <g_msg_queue.h>

namespace gsys {
	
MsgQueue::MsgQueue() : m_mode(G_POP_BLOCK) {}
MsgQueue::MsgQueue(const AccessMode& mode) : m_mode(mode) {}

GResult MsgQueue::push(T* msg)
{
    return G_YES;
}

GResult MsgQueue::push(T* msg);
{
    return G_NO;	
}
}

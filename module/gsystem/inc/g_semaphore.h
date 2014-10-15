/************************************************************************************
**  
* @copyright (c) 2010-2019, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_semaphore.h
* @version     
* @brief      
* @author   duye
* @date     2014-06-20
* @note 
*
*  1. 2014-06-20 duye Created this file
* 
*/
#pragma once

#include <semaphore.h>
#include <g_type.h>

namespace gsys {
	
/** 
 * POSIX semaphore wrapper
 */
class Semaphore
{
public:
    Semaphore() {}
    ~Semaphore() {}

    /**
     * @brief wait resouce number > 0
     * @return true:successed, false:don't waited resouce
     */
    bool wait();
    
    /** 
     * @brief try wait inc resouce number
     * @return true:successed, false:don't waited resouce
     */
    bool tryWait();

    /** 
     * @brief wait for timeout
     * @parma [in] timeout : millisecond
     * @return true:successed, false:don't waited resouce
     */
    bool timedWait(const GUint64 timeout);

    /** 
     * @brief post semaphore, inc resouce number
     * @return true:successed, false:don't waited resouce
     */
    bool post()
	
private:
    sem_t   m_sem;
};

}

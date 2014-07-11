/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file	    g_thread.h
* @version     
* @brief      
* @author   duye
* @date     2013-11-26
* @note 
*
* 3. 2014-06-21 duye move to gohoop project 
* 2. 2014-01-12 duye Add comments   
* 1. 2013-11-26 duye Created this file
* 
*/

#pragma once

#include <pthread.h> 
#include <g_type.h>

/** 
 * thread state
 */
enum ThreadState
{
    /** 
     * running state
     */	
	THR_STATE_RUN = 0,
    /** 
     * stoped state
     */		
	THR_STATE_STOP,
    /** 
     * exit state
     */		
	THR_STATE_EXIT
};

/** 
 * the pointer of thread enter
 */
typedef void* (*ThreadFunPoint_t)(void*);

/** 
 * be inherited ty user
 */
class Runnable
{
public:
	virtual ~Runnable() {}
	
    /**
     * user thread entry function
     * @return G_YES/G_NO
     * @note 
     */			
	virtual GResult run() = 0;
};

/** 
 * POSIX thread wrapper
 */
class Thread
{
public:
    /**
     * constructor
	 * @param [in] target : user run target object
	 * @param [in] autoRel : whether support automatic release, default is yes
     * @note 
     */		
    explicit Thread(Runnable* target, const bool autoRel = true);
    ~Thread();
	
    /**
     * startup thread
     * @return G_YES/G_NO
     * @note 
     */		
	GResult start();

    /**
     * get thread ID
     * @return thread ID
     * @note 
     */		
	GUint32 getThreadId() const;

private:
	Thread(const Thread&);
	void operator=(const Thread&);
	static void* enterPoint(void* argument);

private:
	// thread ID
	pthread_t	    m_threadId;
	// indicate whether is detached with main thread£¬default is detached
	bool		    m_autoRel;
	// user thread object
	Runnable*	    m_runnable;
};

/** 
 * thread base class, be inherited by user
 */
class ThreadTask
{
public:
    /**
     * constructor
     * @para [in] autoRel : whether is detached with main thread, default is detached
     * @note 
     */		
	explicit ThreadTask(const bool autoRel = true);
	virtual ~ThreadTask();

    /**
     * startup thread
     * @return G_YES/G_NO
     * @note 
     */		
	GResult start();

    /**
     * thread entry function
     * @return G_YES/G_NO
     * @note 
     */			
	virtual GResult run() = 0;

private:
	// brief : prevate copying
	ThreadTask(const ThreadTask&);
	void operator=(const ThreadTask&);
	
	// brief : inner used for starting thread
	// @para [in]argument : thread argument
	// return : thread return description
	static void* enterPoint(void* argument);	

private:
	// thread ID
	pthread_t	    m_threadId;	
	// whether is detached with main thread, default is ture, 
	// indicate detached with main thread
	bool		    m_autoRel;
};

/** 
 * POSIX thread static API used outside
 */
class ThreadUtil
{
public:
    /**
     * create thread
	 * @param [in] entry : thread entry fucntion pointer
	 * @param [in] argument : user data
	 * @param [in] autoRel : whether support automatic release, default is yes
	 * @return thread ID / -1
     * @note 
     */		
	static GInt32 createThread(void* entry, void* argument, const bool autoRel = true);
};
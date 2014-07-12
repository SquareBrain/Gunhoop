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
enum GThreadState
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
typedef void* (*GThreadFunPoint_t)(void*);

/** 
 * be inherited ty user
 */
class GRunnable
{
public:
	virtual ~GRunnable() {}
	
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
class GThread
{
public:
    /**
     * constructor
	 * @param [in] target : user run target object
	 * @param [in] autoRel : whether support automatic release, default is yes
     * @note 
     */		
    explicit GThread(GRunnable* target, const bool autoRel = true);
    ~GThread();
	
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
	GUint32 getGThreadId() const;

private:
	GThread(const GThread&);
	void operator=(const GThread&);
	static void* enterPoint(void* argument);

private:
	// GThread ID
	pthread_t	    m_threadId;
	// indicate whether is detached with main GThread£¬default is detached
	bool		    m_autoRel;
	// user GThread object
	GRunnable*	    m_runnable;
};

/** 
 * thread base class, be inherited by user
 */
class GThreadTask
{
public:
    /**
     * constructor
     * @para [in] autoRel : whether is detached with main GThread, default is detached
     * @note 
     */		
	explicit GThreadTask(const bool autoRel = true);
	virtual ~GThreadTask();

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
	GThreadTask(const GThreadTask&);
	void operator=(const GThreadTask&);
	
	// brief : inner used for starting GThread
	// @para [in]argument : GThread argument
	// return : GThread return description
	static void* enterPoint(void* argument);	

private:
	// GThread ID
	pthread_t	    m_threadId;	
	// whether is detached with main GThread, default is ture, 
	// indicate detached with main GThread
	bool		    m_autoRel;
};

/** 
 * POSIX GThread static API used outside
 */
class GThreadUtil
{
public:
    /**
     * create thread
	 * @param [in] entry : GThread entry fucntion pointer
	 * @param [in] argument : user data
	 * @param [in] autoRel : whether support automatic release, default is yes
	 * @return GThread ID / -1
     * @note 
     */		
	static GInt32 createGThread(void* entry, void* argument, const bool autoRel = true);
};
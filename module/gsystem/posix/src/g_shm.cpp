/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_shm.cpp
* @version     
* @brief      
* @author	duye
* @date		2014-02-22
* @note 
*
*  2. 2014-06-21 duye move to gohoop 
*  1. 2014-02-22 duye Created this file
* 
*/
#include <g_shm.h>

using namespace gsys;

// default GShm size is 10M
static const GUint64 G_DEF_GShm_SIZE = 1024 * 1024 * 10;

GShm::GShm() : m_shmSize(0), m_shmAddr(NULL), m_initFlags(false)
{
	m_shmPath[0] = 0;
}

GShm::GShm(const GInt8* shmPath, const GUint64 shmSize) 
    : m_shmSize(shmSize), m_shmAddr(NULL), m_initFlags(false)
{
	setPath(shmPath);
	init();
}

GShm::~GShm()
{
	uninit();    
}

void GShm::setPath(const GInt8* shmPath)
{
	GUint32 len = strlen(shmPath);
	memcpy(m_shmPath, shmPath, len);
	m_shmPath[len] = 0;
}

void GShm::setSize(const GUint64 shmSize)
{
	m_shmSize = shmSize;    
}
     
GResult GShm::sync()
{
	if (!m_initFlags)
	{
		//G_LOG_ERROR(G_LOG_PREFIX, "GShm hasn't init");
		return G_NO;
	}

	if (msync(m_shmAddr, m_shmSize, MS_SYNC) < 0)
	{
		//G_LOG_ERROR(G_LOG_PREFIX, "call msync() failed");
		return SYNC_SHM_FAILED;
	}

	return G_YES;    
} 
 
GResult GShm::write(const GUint32 offset, const GInt8* data, const GUint32 length)
{
	if (!m_initFlags)
	{
		//G_LOG_ERROR(G_LOG_PREFIX, "GShm hasn't init");
		return G_NO;
	} 

	if (data == NULL)
	{
		//G_LOG_ERROR(G_LOG_PREFIX, "input data is NULL");
		return WRITE_SHM_PARA_FAILED;
	}

	if (length == 0)
	{
		//G_LOG_ERROR(G_LOG_PREFIX, "input data length is zero");
		return WRITE_SHM_PARA_FAILED;
	}

	if (offset + length > m_shmSize)
	{
		//G_LOG_ERROR(G_LOG_PREFIX, "will write data length over GShm size");
		return WRITE_SHM_PARA_FAILED;
	}

	memcpy((GInt8*)m_shmAddr + offset, data, length);

	return G_YES;        
}

GResult GShm::read(const GUint32 offset, GInt8* buffer, const GUint32 size)
{
	if (!m_initFlags)
	{
		//G_LOG_ERROR(G_LOG_PREFIX, "GShm hasn't init");
		return G_NO;
	} 

	if (buffer == NULL)
	{
		//G_LOG_ERROR(G_LOG_PREFIX, "output buffer is NULL");
		return WRITE_SHM_PARA_FAILED;
	}

	if (size == 0)
	{
		//G_LOG_ERROR(G_LOG_PREFIX, "ouput buffer size is zero");
		return WRITE_SHM_PARA_FAILED;
	}

	if (offset + size > m_shmSize)
	{
		//G_LOG_ERROR(G_LOG_PREFIX, "will read length over GShm size");
		return WRITE_SHM_PARA_FAILED;
	}

	memcpy((GInt8*)buffer, (GInt8*)m_shmAddr + offset, size);

	return G_YES;     
}

GResult GShm::init()
{
	if (m_initFlags)
	{
		//G_LOG_ERROR(G_LOG_PREFIX, "GShm had init");
		return G_NO;
	}

	if (strlen(m_shmPath) == 0)
	{
		//G_LOG_ERROR(G_LOG_PREFIX, "GShm path is empty");
		return SHM_PATH_EMPTY;
	}

	GInt32 fd = ::open(m_shmPath, O_RDWR | O_CREAT);
	if (fd < 0)
	{
		//G_LOG_ERROR(G_LOG_PREFIX, "open GShm file failed");
		return OPEN_SHM_FAILED;
	}

	// setting file size
	if (m_shmSize == 0)
	{
		m_shmSize = G_DEF_GShm_SIZE;   
	}

	ftruncate(fd, m_shmSize);
	m_shmAddr = mmap(NULL, m_shmSize, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
	if (m_shmAddr == MAP_FAILED)
	{
		close(fd);
		//G_LOG_ERROR(G_LOG_PREFIX, "call mmap() failed");
		return MMAP_SHM_FAILED;
	}

	close(fd);

	m_initFlags = true;

	return G_YES;
}
    
GResult GShm::uninit()
{
	if (!m_initFlags)
	{
		//G_LOG_ERROR(G_LOG_PREFIX, "GShm hasn't init");
		return G_NO;
	}

	if (munmap(m_shmAddr, m_shmSize) < 0)
	{
		//G_LOG_ERROR(G_LOG_PREFIX, "call munmap failed");
		return MUNMAP_SHM_FAILED;
	}

	m_shmAddr = NULL;

	m_initFlags = false;

	return G_YES;
}

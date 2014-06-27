/************************************************************************************
**  
* @copyright (c) 2013-2100, ChengDu Duyer Technology Co., LTD. All Right Reserved.
*
*************************************************************************************/
/**
* @file		g_shm.cpp
* @version     
* @brief      
* @author   duye
* @date     2014-02-22
* @note 
*
*  2. 2014-06-21 duye move to gohoop 
*  1. 2014-02-22 duye Created this file
* 
*/

#include <g_shm.h>

static const GInt8* G_LOG_PREFIX = "gohoop.gcommon.system.shm";

// default shm size is 10M
static const GUint64 G_DEF_SHM_SIZE = 1024 * 1024 * 10;

G_NS_GCOMMON_BEG

Shm::Shm() : m_shmSize(0), m_shmAddr(NULL), m_initFlags(false)
{
    m_shmPath[0] = 0;
}

Shm::Shm(const GInt8* shmPath, const GUint64 shmSize) 
    : m_shmSize(shmSize), m_shmAddr(NULL), m_initFlags(false)
{
    setShmPath(shmPath);
    init();
}

Shm::~Shm()
{
    uninit();    
}

void Shm::setShmPath(const GInt8* shmPath)
{
    GUint32 len = strlen(shmPath);
    memcpy(m_shmPath, shmPath, len);
    m_shmPath[len] = 0;  
    init();
}

GInt8* Shm::getShmPath()
{
    return m_shmPath;
}

void Shm::setShmSize(const GUint64 shmSize)
{
    m_shmSize = shmSize;    
}

GUint64 Shm::getShmSize() const
{
    return m_shmSize;
}
     
GResult Shm::syncShm()
{
    if (!m_initFlags)
    {
        return G_NO;
    }
    
    if (msync(m_shmAddr, m_shmSize, MS_SYNC) < 0)
    {
        return SYNC_SHM_FAILED;
    }

    return G_YES;    
} 
 
GResult Shm::writeShm(const GUint32 offset, const GInt8* data, const GUint32 size)
{
    if (!m_initFlags)
    {
        return G_NO;
    } 

    if (data == NULL || size == 0 || offset + size > m_shmSize)
    {
        return WRITE_SHM_PARA_FAILED;
    }

    memcpy((GInt8*)m_shmAddr + offset, data, size);
    
    return G_YES;        
}

GResult Shm::readShm(const GUint32 offset, GInt8* data, const GUint32 size)
{
    if (!m_initFlags)
    {
        return G_NO;
    } 

    if (data == NULL || size == 0 || offset + size > m_shmSize)
    {
        return READ_SHM_PARA_FAILED;
    }

    memcpy((GInt8*)data, (GInt8*)m_shmAddr + offset, size);
    
    return G_YES;     
}

GResult Shm::init()
{
    if (!m_initFlags)
    {
        return G_NO;
    }

    if (strlen(m_shmPath) == 0)
    {
        return SHM_PATH_EMPTY;
    }
    
    GInt32 fd = open(m_shmPath, O_RDWR | O_CREAT);
    if (fd < 0)
    {
        return OPEN_SHM_FAILED;
    }

    // setting file size
    if (m_shmSize == 0)
    {
        m_shmSize = G_DEF_SHM_SIZE;   
    }
    
    ftruncate(fd, m_shmSize);
    m_shmAddr = mmap(NULL, m_shmSize, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (m_shmAddr == MAP_FAILED)
    {
        close(fd);
        return MMAP_SHM_FAILED;
    }

    close(fd);

    m_initFlags = true;
    
    return G_YES;
}
    
GResult Shm::uninit()
{
    if (!m_initFlags)
    {
        return G_NO;
    }
    
    if (munmap(m_shmAddr, m_shmSize) < 0)
    {
        return MUNMAP_SHM_FAILED;
    }

    m_shmAddr = NULL;

    m_initFlags = false;

    return G_YES;
}

G_NS_END

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

static const GInt8* LOG_PREFIX = "posix.ipc.shm";

G_NS_GCOMMON_BEG

Shm::Shm() : m_shmSize(0), m_shmAddr(NULL)
{
}

Shm::Shm(const GInt8* path, const GUint32 size) : m_shmSize(size), m_shmAddr(NULL)
{
    m_shmPath.assign(path);      
}

Shm::Shm(const std::string& path, const GUint32 size) 
    : m_shmPath(path), m_shmSize(size),  m_shmAddr(NULL)
{
}

Shm::~Shm()
{
    Uninit();    
}

void Shm::SetPath(const GInt8* path)
{
    m_shmPath.assign(path);    
}

void Shm::SetPath(const std::string& path)
{
    m_shmPath = path;    
}

const std::string& Shm::GetPath() const
{
    return m_shmPath;
}

void Shm::SetSize(const GUint32 size)
{
    m_shmSize = size;    
}

GUint32 Shm::GetSize() const
{
    return m_shmSize;
}

GResult Shm::Init()
{
    if (m_shmPath.empty())
    {
        return SHM_PATH_EMPTY;
    }
    
    D_Int32 fd = open(m_shmPath.c_str(), O_RDWR | O_CREAT);
    if (fd < 0)
    {
        return OPEN_SHM_FAILED;
    }

    // setting file size
    ftruncate(fd, m_shmSize);

    // bind shm file
    if (m_shmAddr != NULL)
    {
        Uninit();   
	} 
    
    m_shmAddr = mmap(NULL, m_shmSize, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (m_shmAddr == MAP_FAILED)
    {
        close(fd);
        return MMAP_SHM_FAILED;
    }

    close(fd);
    
    return G_OK;
}
    
GResult Shm::Uninit()
{
    if (m_shmAddr == NULL)
    {
        return SHM_NO_INIT;
    }
    
    if (munmap(m_shmAddr, m_shmSize) < 0)
    {
        return MUNMAP_SHM_FAILED;
    }

    m_shmAddr = NULL;

    return G_OK;
}
     
GResult Shm::Sync()
{
    if (m_shmAddr == NULL)
    {
        return SHM_NO_INIT;
    }
    
    if (msync(m_shmAddr, m_shmSize, MS_SYNC) < 0)
    {
        return SYNC_SHM_FAILED;
    }

    return G_OK;    
} 
 
GResult Shm::Write(const GUint32 offset, const GInt8* data, const GUint32 size)
{
    G_ASSERT(data != NULL && size > 0);

    if (data == NULL || size == 0 || offset + size > m_shmSize)
    {
        return WRITE_SHM_PARA_FAILED;
    }

    memcpy((GInt8*)m_shmAddr + offset, data, size);
    
    return G_OK;        
}

GResult Shm::Read(const GUint32 offset, GInt8* data, const GUint32 size)
{
    G_ASSERT(data != NULL && size > 0);
    
    if (data == NULL || size == 0 || offset + size > m_shmSize)
    {
        return READ_SHM_PARA_FAILED;
    }

    memcpy((GInt8*)data, (GInt8*)m_shmAddr + offset, size);
    
    return G_OK;     
}

G_NS_GCOMMON_END

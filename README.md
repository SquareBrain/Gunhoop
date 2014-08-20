Gohoop简介
====
    Gohoop分布式存储与计算平台(Gohoop Distributed Storage And Compute Platforms)，简称GDSCP。
    开发语言C++，可运行在Linux平台上。

背景
====
    1. 实现大数据分布式存储。
    2. 实现任务分布式运算。
    3. 对外以RPC方式提供。

发布
====
### Gohoop.1.0.4 2015.06.30
    增加结构和非结构化数据存取服务，包括：
    GStoreMaster 进程
    GStoreServer 进程
    GSQLMaster 进程
    GSQLServer 进程

### Gohoop.1.0.2 2015.02.28
    增加文件系统实现、Mapreduce框架实现、丰富基础库，包括：
    GDFSMaster 进程
    GDFSServer 进程
    GTaskMaster 进程
    GTaskServer 进程
    基础库增加和修复

### Gohoop.1.0.0 2014.10.31
    基础库实现，包括：
    libgutils.so(包括gini gxml glogger gzip等子模块)
    libgsystem.so

下载
====
[结构化和非结构化存取服务版gohoop.1.0.4](https://github.com/Gohoop/Gohoop/tag/gohoop.1.0.0.tar.gz) <br />
[文件系统版gohoop.1.0.2](https://github.com/Gohoop/Gohoop/tag/gohoop.1.0.2.tar.gz) <br />
[基础库版gohoop.1.0.0](https://github.com/Gohoop/Gohoop/tag/gohoop.1.0.4.tar.gz) <br />

部署
====
    部署分为Master端和Slave端，Master负责管理、控制Slave端服务。

### Master端
----
    GTaskMaster、GDFSMaster、GStoreMaster、GSQLMaster

### Slave端
----
    GTaskServer、GDFSServer、GStoreServer、GSQLServer

安装
====
building
----

使用
====
> 详细见gohoop_guide.pdf

### GTaskMaster & GTaskServer

### GDFSMaster & GDFSServer

### GStoreMaster & GStoreServer

### GSQLMaster & GSQLServer

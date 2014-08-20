Gohoop简介
====
Gohoop分布式存储与计算平台(Gohoop Distributed Storage And Compute Platforms)，简称GDSCP。开发语言C++，可运行在Linux平台上。

背景
====
1. 实现大数据分布式存储。
2. 实现任务分布式运算。
3. 对外以RPC方式提供。


发布
====

###2015.06.30 Gohoop.1.0.4 实现非结构化数据存取服务、结构化数据存取服务，包括：
    > GStoreMaster 进程
    > GStoreServer 进程
    > GSQLMaster 进程
    > GSQLServer 进程

###2015.02.28 Gohoop.1.0.2 文件系统实现、Mapreduce框架实现、丰富基础库，包括：
    > GDFSMaster 进程
    > GDFSServer 进程
    > GTaskMaster 进程
    > GTaskServer 进程
    > addition and fixed bug 基础库

###2014.10.31 Gohoop.1.0.0 基础库实现，包括：
    > libgutils.so(gini.a gxml.a glogger.a gzip.a)
    > libgsystem.so

下载
====
https://github.com/Gohoop/Gohoop/tag/xxx.rar.gz

部署
====
> 部署分为Master端和Slave端，Master负责管理、控制Slave端服务。

###Master端
----
GTaskMaster、GDFSMaster、GStoreMaster、GSQLMaster

###Slave端
----
GTaskServer、GDFSServer、GStoreServer、GSQLServer

安装
====
building
----

使用
====
> 详细见gohoop_guide.pdf

GTaskMaster & GTaskServer
----

GDFSMaster & GDFSServer
----

GStoreMaster & GStoreServer
----

GSQLMaster & GSQLServer
----

Gohoop简介
====
Gohoop分布式存储与计算平台(Gohoop Distributed Storage And Compute Platforms)，简称GDSCP。
开发语言C++，可运行在Linux平台上。实现大数据分布式存储，实现任务分布式运算，对外服务以多种方式提供。

背景
====
在大数据平台已经非常多的情况下，为何我们还要开始这样一个项目，有几点：<br/>
1.通过这样一个需要几年才能完成的项目，可以提升自己的系统设计能力和全局视野。<br/>
2.软件开发是一个实践过程，只有实践才能领悟其真谛，内在的东西才能升华。<br/>
3.如有利可图，那也是付出该有的收获；如只是镜中花水中月，那也是一段意义非凡的精神享受。<br/>

软件结构
====
### 基础库
1. gsystem<br/>
    生成物：libgsystem.so libgsystem.a<br/>
    依赖：系统API、标准库<br/>

2. gutils<br/> 
    生成物：libgutils.so libgutils.a<br/>
    依赖：gsystem、标准库<br/>

    子模块1 gini -提供解析ini文件接口<br/>
    子模块2 gxml -提供解析xml文件接口<br/>
    
3. glogger<br/>
    生成物：libglogger.so libglogger.a<br/> 
    依赖：gsystem、gutils、标准库<br/>

4. gcommon<br/> 
    生成物：libgcommon.so libgcommon.a<br/> 
    依赖：gsystem、gutils、glogger、标准库<br/>

    子模块1 filesys     -提供封装的文件系统API<br/>
    子模块2 function    -提供封装的语言特性组件<br/>
    子模块3 iostream    -提供封装的I/O操作组件<br/>
    子模块4 mapreduce   -提供map、reduce基础结构<br/>
    子模块5 network     -提供网络编程组件<br/>

### 服务进程
1. gdfs 文件系统服务<br/>
    子模块1 gdfsmaster -部署在master服务器上，管理slave结点<br/>
    子模块2 gdfsserver -部署在slave服务器上，管理本地文件系统<br/>
    子模块3 gdfsclient -外部访问文件系统的命令行客户端程序<br/>

2. gtask 任务执行跟踪服务<br/>
    子模块1 gtaskmaster -部署在master服务器上，管理slave结点<br/>
    子模块2 gtaskserver -部署在slave服务器上，管理任务执行<br/>

3. gsql 结构化数据存储和查询服务<br/>
    子模块1 gsqlmaster -部署在master服务器上，管理slave结点<br/>
    子模块2 gsqlserver -部署在slave服务器上，管理本地结构化数据存储和查询<br/>

4. gstore 非结构化数据存储和查询服务<br/>
    子模块1 gstoremaster -部署在master服务器上，管理slave结点<br/>
    子模块2 gstoreserver -部署在slave服务器上，管理非结构化数据存储和查询<br/>

### 测试模块 
gtest包括了对各个库和服务的测试程序实现。

发布
====
版本号约定：A.B.C <br/>
A : 主版本号，框架设计，或其它重大更改，需增加此版本号。<br/>
B : 次版本号，新增模块、重大功能优化、阶段性迭代完成等，需增加此版号。<br/>
C : Bug版本号，bug修复版本。<br/>

### Gohoop.1.2.0 2015.06.30
增加结构和非结构化数据存取服务，包括：<br/>
GStoreMaster 进程<br/>
GStoreServer 进程<br/>
GSQLMaster 进程<br/>
GSQLServer 进程<br/>

### Gohoop.1.1.0 2015.02.28
增加文件系统服务、Task服务，Mapreduce框架实现、丰富基础库，包括：<br/>
GDFSMaster 进程<br/>
GDFSServer 进程<br/>
GDFSClient 进程<br/>
GTaskMaster 进程<br/>
GTaskServer 进程<br/>
基础库增加和修复<br/>

### Gohoop.1.0.0 2014.10.31
基础库实现，包括：<br/>
libgsystem.so<br/>
libgutils.so(包括gini gxml gzip)<br/>
libglogger.so<br/>
libgcommon.so(包括filesys function iostream network)<br/>
    
下载
====
[gohoop.1.2.0 新增结构化和非结构化存取服务](https://github.com/Gohoop/Gohoop/tag/gohoop.1.0.0.tar.gz) <br />
[gohoop.1.1.0 新增文件系统服务](https://github.com/Gohoop/Gohoop/tag/gohoop.1.0.2.tar.gz) <br />
[gohoop.1.0.0 仅完成基础库实现](https://github.com/Gohoop/Gohoop/tag/gohoop.1.0.4.tar.gz) <br />

部署
====
部署分为Master端和Slave端。Master负责管理、控制Slave端服务，Slave端负责具休的数据存储和运行行为。

### Master端布署
----
GTaskMaster、GDFSMaster、GStoreMaster、GSQLMaster

### Slave端布署
----
GTaskServer、GDFSServer、GStoreServer、GSQLServer

安装
====
1. 配置
./configuration --prefix=/etc/usr/local

2. 编译
make 

3. 安装
make install

4. 测试安装
gohoop_guide

使用
====
详细见gohoop_guide.pdf

### GTaskMaster & GTaskServer

### GDFSMaster & GDFSServer

### GStoreMaster & GStoreServer

### GSQLMaster & GSQLServer

# SimpleMuduo

基于LinuxIO多路复用Epoll的简单Muduo网络库C++实现

## 描述
- 基础类：
  - Channel:对于单个fd的封装，将fd事件拆分为本身事件类型用于检测事件，epoll触发事件类型用于处理事件
  - InetAddress:对于socket等所需的struct sockaddr的封装
  - ThreadPool:线程池的封装
  - Socket:对于socket的封装，封装socket的bind，listen，accept，connect等API的接口，以及设置socket fd的属性接口
  - Buffer:对于读写事件缓冲区的封装，方便流式读取或者写入 
  - Epoll:对于linux epoll API的封装，poll接口一次性返回该Epoll上触发的事件，channel可在此更新自己的本身事件
- 基本类：
  - Acceptor:对于创建socket并监听以及返回新的连接这一整个流程事件的封装。管理Socket对象(创建以及生命周期)
  - Connection:对于fd触发事件时所需后续流程事件的封装，管理Channel对象
- 综合类：
  - EventLoop:对于不停检测是否有可用的事件的封装，管理Epoll对象
  - Server：对于开启一个socket，监听连接，以及释放连接的流程封装。管理ThreadPool对象、Connection对象、Acceptor对象

## 设计
- 封装思想：基于事件，事件流程的封装
- OBP思想：使用std::bind机制，不使用继承等适配器方法，解耦
- 主从多线程Reactor模式：事件驱动模型，主Reactor主要负责接收连接并将连接分发至不同的从Reactor
- LT + ET
- one loop one thread思想：主线程处理主Reactor(EpollLoop)事件处理，子线程每一个负责一个从Reactor的事件处理 

## 使用
- cd build  
- cmake ..  
- make  
- ./server -i [ip] -p [port]  
- ./client -t [发送消息线程数] -m [发送消息消息数] -w [可选：连接后几秒开始发送]  

## 后续改进
- 将Connection的事件注册回调至Server实现事件处理与事件触发分离，支持多种自定义处理
- 跨平台的支持
- 代码分析以及性能优化 

## 参考
- chenshuo/muduo ><https://github.com/chenshuo/muduo>
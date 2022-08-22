#pragma once

#include <map>
#include <memory>
#include <vector>

namespace simplemuduo
{
    class EventLoop;
    class Acceptor;
    class Socket;
    class Connection;
    class ThreadPool;
    
    class Server
    {
    private:
        EventLoop&                              m_main_eventloop;
        Acceptor*                               m_acceptor;
        ThreadPool*                             m_threadpool;
        std::map<int, Connection*>              m_connections;
        std::vector<EventLoop*>                 m_sub_eventloop;
        int                                     m_sub_add_idx;

    public:
        Server(EventLoop& eventloop, const char* ip, const int port);
        ~Server();
        void handleAcceptEvent(Socket& sock);
        void handleReleaseEvent(Connection* connection);
        int getNextSubEventLoopIdx();
    }; 
}
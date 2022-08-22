#include <stdio.h>
#include <functional>

#include "./include/Server.h"
#include "./include/EventLoop.h"
#include "./include/Socket.h"
#include "./include/Acceptor.h"
#include "./include/Connection.h"
#include "./include/Channel.h"
#include "./include/ThreadPool.h"

using namespace simplemuduo;

Server::Server(EventLoop& eventloop, const char* ip, const int port):
    m_main_eventloop(eventloop),
    m_acceptor(nullptr),
    m_threadpool(new ThreadPool),
    m_sub_add_idx(0)
{
    m_acceptor = new Acceptor(m_main_eventloop, ip, port);
    m_acceptor->setHandleAccept(std::bind(&Server::handleAcceptEvent, this, std::placeholders::_1));

    for (int i = 0; i < m_threadpool->getSize(); ++i)
    {
        m_sub_eventloop.push_back(new EventLoop);
    }

    for (int i = 0; i < m_threadpool->getSize(); ++i)
    {
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, m_sub_eventloop[i]);
        m_threadpool->pushTask(sub_loop);
    }
}

Server::~Server()
{
    if (m_acceptor != nullptr)
    {
        delete m_acceptor;
    }
    if (m_threadpool != nullptr)
    {
        delete m_threadpool;
    }
    for (auto i : m_sub_eventloop)
    {
        delete i;
    }
    m_sub_eventloop.clear();
}

void Server::handleAcceptEvent(Socket& sock)
{
    Connection* connection = new Connection(*m_sub_eventloop[getNextSubEventLoopIdx()], sock);
    if (connection->getChannel() == nullptr)
    {
        printf("new connection error\n");
        delete connection;
    }
    else
    {
        connection->setReleaseCall(std::bind(&Server::handleReleaseEvent, this, std::placeholders::_1));
        m_connections[connection->getChannel()->getFd()] = connection;
        printf("handleAcceptEvent fd=%d m_connections.size=%d\n", connection->getChannel()->getFd(), (int)m_connections.size());
    }
}

void Server::handleReleaseEvent(Connection* connection)
{
    m_connections.erase(connection->getChannel()->getFd());
    printf("handleReleaseEvent fd=%d m_connections.size=%d\n", connection->getChannel()->getFd(), (int)m_connections.size());
    delete connection;
}

int Server::getNextSubEventLoopIdx()
{
    m_sub_add_idx++;
    if (m_sub_add_idx >= m_sub_eventloop.size() + 1)
    {
        m_sub_add_idx = 1;
    }
    return m_sub_add_idx - 1;
}
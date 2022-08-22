#include <sys/epoll.h>

#include "./include/Acceptor.h"
#include "./include/InetAddress.h"
#include "./include/Socket.h"
#include "./include/Channel.h"
#include "./include/EventLoop.h"

using namespace simplemuduo;

Acceptor::Acceptor(EventLoop& eventloop, const char* ip, const int port):
    m_eventloop(eventloop),
    m_socket(nullptr),
    m_accept_ch(nullptr)
{
    InetAddress inetaddr(ip, port);

    m_socket = new Socket;
    m_socket->bind(inetaddr);
    m_socket->listen();
    //m_socket->setFdNoBlock(m_socket->getSocketFd()); //accept资源消耗小，使用阻塞即可

    m_accept_ch = new Channel(m_eventloop, m_socket->getSocketFd());
    m_accept_ch->setEvents(EPOLLIN | EPOLLRDHUP);
    m_accept_ch->setHandleReadEvent(std::bind(&Acceptor::handleAccept, this));
    m_accept_ch->setThreadPooled(false);
    m_eventloop.updateChannel(m_accept_ch);
}

Acceptor::~Acceptor()
{
    if (m_socket != nullptr)
    {
        delete m_socket;
    }
    if (m_accept_ch != nullptr)
    {
        delete m_accept_ch;
    }
}

void Acceptor::setHandleAccept(std::function<void(Socket& sock)> call)
{
    m_accept_callback = call;
}

void Acceptor::handleAccept()
{
    if (m_accept_callback)
    {
        m_accept_callback(*m_socket);
    }
}
#include "stdio.h"
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <thread>
#include <chrono>

#include "./include/Connection.h"
#include "./include/Channel.h"
#include "./include/InetAddress.h"
#include "./include/Socket.h"
#include "./include/Buffer.h"

using namespace simplemuduo;

Connection::Connection(EventLoop& eventloop, Socket& sock):
    m_eventloop(eventloop),
    m_socket(sock),
    m_channel(nullptr),
    m_buffer(nullptr)
{
    InetAddress tmp;
    int connfd = m_socket.accept(tmp);
    if (connfd == -1) return;

    m_socket.setFdNoBlock(connfd);
    
    m_channel = new Channel(m_eventloop, connfd);
    m_channel->setReadEvents();
    m_channel->setHandleReadEvent(std::bind(&Connection::handleReadEvent, this, connfd));

    m_buffer = new Buffer;
}

Connection::~Connection()
{
    if (m_channel != nullptr)
    {
        ::close(m_channel->getFd()); //关闭socket会自动将文件描述符从epoll树上移除
        delete m_channel;
    }
    if (m_buffer != nullptr)
    {
        delete m_buffer;
    }
}

Channel* Connection::getChannel()
{
    return m_channel;
}

void Connection::setReleaseCall(std::function<void(Connection* connection)> call)
{
    m_release_call = call;
}

void Connection::handleReadEvent(const int& fd)
{
    char buf[1024] = { 0 };
    for (;;)
    {
        int recv_cnt = ::recv(fd, buf, sizeof(buf), 0);
        if (recv_cnt > 0)
        {
            m_buffer->append(buf, recv_cnt);
        }
        else if(recv_cnt == -1 && errno == EINTR) //客户端正常中断、继续读取
        {  
            continue;
        }
        else if(recv_cnt == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) //非阻塞IO，socket缓冲区读完了
        {
            printf("recv size=%d buf=%s\n", m_buffer->getSize(), m_buffer->getBuf());
            handleWriteEvent(fd);
            break;
        }
        else if(recv_cnt == 0) //EOF，客户端断开连接
        {
            printf("close fd=%d\n", fd);
            if (m_release_call)
            {
                m_release_call(this);
            }
            break;
        }
    }
}

void Connection::handleWriteEvent(const int& fd)
{
    int send_left = m_buffer->getSize();
    for (;;)
    {
        int send_cnt = ::send(fd, m_buffer->getBuf() + m_buffer->getSize() - send_left, send_left, 0);
        if (send_cnt > 0)
        {
            send_left -= send_cnt;
            if (send_left <= 0)
            {
                printf("send size=%d buf=%s\n", m_buffer->getSize(), m_buffer->getBuf());
                break;
            }
        }
        else if (send_cnt == -1 && errno == EINTR)
        {
            break;
        }
        else if (send_cnt == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) // 非阻塞IO，socket缓冲区满了
        {
            printf("send socket buf full\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    m_buffer->clear();
}
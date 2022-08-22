#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>

#include "./include/Socket.h"

using namespace simplemuduo;

Socket::Socket():
    m_socketfd(-1)
{
    if ((m_socketfd = ::socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, IPPROTO_TCP)) == -1)
    {
        printf("create err:%s(errno=%d)\n", strerror(errno), errno);
    }
    else
    {
        setReuseAddr(true);
        setReusePort(true);
    }
}

Socket::~Socket()
{
    if (m_socketfd != -1)
        ::close(m_socketfd);
}

void Socket::setTcpNoDelay(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(m_socketfd, IPPROTO_TCP, TCP_NODELAY,
        &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReuseAddr(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(m_socketfd, SOL_SOCKET, SO_REUSEADDR,
        &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReusePort(bool on)
{
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(m_socketfd, SOL_SOCKET, SO_REUSEPORT,
        &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setKeepAlive(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(m_socketfd, SOL_SOCKET, SO_KEEPALIVE,
        &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setFdNoBlock(int fd)
{
    int old_flag = ::fcntl(fd, F_GETFL, 0);
    int new_flag = old_flag | O_NONBLOCK;
    ::fcntl(fd, F_SETFL, new_flag);
}

void Socket::bind(InetAddress& inetaddr)
{
    if (::bind(m_socketfd, inetaddr.getAddrPtr(), inetaddr.getAddrLen()) == -1)
        printf("bind err:%s(errno=%d)\n", strerror(errno), errno);
}

void Socket::listen()
{
    if (::listen(m_socketfd, SOMAXCONN) == -1)
        printf("listen err:%s(errno=%d)\n", strerror(errno), errno);
    else
        printf("listen success socketfd=%d\n", m_socketfd);
}

int Socket::accept(InetAddress& inetaddr)
{
    int connfd = -1;
    if ((connfd = ::accept(m_socketfd, inetaddr.getAddrPtr(), inetaddr.getAddrLenPtr())) == -1)
    {
        printf("accept err:%s(errno=%d)\n", strerror(errno), errno);
        return -1;
    }
    else
    {
        printf("accept success connfd=%d\n", connfd);
    }
    return connfd;
}

int Socket::connect(InetAddress& inetaddr)
{
    if (::connect(m_socketfd, inetaddr.getAddrPtr(), inetaddr.getAddrLen()) == -1)
    {
        printf("connect err:%s(errno=%d)\n", strerror(errno), errno);
        return -1;
    }
    else
    {
        printf("connect success socketfd=%d\n", m_socketfd);
    }
    return 0;
}
#pragma once

#include "InetAddress.h"

namespace simplemuduo
{
    class Socket
    {
    private:
        int m_socketfd;

    public:
        Socket();
        ~Socket();
        int getSocketFd() { return m_socketfd; };
        void setTcpNoDelay(bool on);
        void setReuseAddr(bool on);
        void setReusePort(bool on);
        void setKeepAlive(bool on);
        void setFdNoBlock(int fd);
        void bind(InetAddress& inetaddr);
        void listen();
        int accept(InetAddress& inetaddr);
        int connect(InetAddress& inetaddr);
    };
}
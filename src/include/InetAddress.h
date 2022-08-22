#pragma once

#include <netinet/in.h>

namespace simplemuduo
{
    class InetAddress
    {
    private:
        struct sockaddr_in m_addr;
        socklen_t m_addr_len;
    
    public:
        InetAddress();
        InetAddress(const char* ip, uint16_t port);
        ~InetAddress();
        sockaddr* getAddrPtr();
        socklen_t getAddrLen();
        socklen_t* getAddrLenPtr();
    };
}
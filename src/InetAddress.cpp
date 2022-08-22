#include <arpa/inet.h>
#include <string.h>

#include "./include/InetAddress.h"

using namespace simplemuduo;

InetAddress::InetAddress():
    m_addr_len(sizeof(sockaddr_in))
{
    bzero(&m_addr, sizeof(m_addr));
}

InetAddress::InetAddress(const char* ip, uint16_t port):
    m_addr_len(sizeof(sockaddr_in))
{
    bzero(&m_addr, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = inet_addr(ip);
    m_addr.sin_port = htons(port);
}

InetAddress::~InetAddress()
{
}

sockaddr* InetAddress::getAddrPtr()
{
    return (sockaddr*)&m_addr;
}

socklen_t InetAddress::getAddrLen()
{
    return m_addr_len;
}

socklen_t* InetAddress::getAddrLenPtr()
{
    return &m_addr_len;
}
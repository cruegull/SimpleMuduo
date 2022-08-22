#pragma once

#include <memory>
#include <functional>

namespace simplemuduo
{
    class EventLoop;
    class Socket;
    class Channel;

    class Acceptor
    {
    private:
        EventLoop&                          m_eventloop;
        Socket*                             m_socket;
        Channel*                            m_accept_ch;
        std::function<void(Socket& sock)>   m_accept_callback;

    public:
        Acceptor(EventLoop& eventloop, const char* ip, const int port);
        ~Acceptor();
        void setHandleAccept(std::function<void(Socket& sock)> call);
        void handleAccept();
    };
}
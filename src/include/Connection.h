#pragma once

#include <functional>

namespace simplemuduo
{
    class EventLoop;
    class Socket;
    class Channel;
    class Buffer;

    class Connection
    {
    private:
        EventLoop&                                  m_eventloop;
        Socket&                                     m_socket;
        Channel*                                    m_channel;
        Buffer*                                     m_buffer;
        std::function<void(Connection* connection)> m_release_call;
        
    public:
        Connection(EventLoop& eventloop, Socket& sock);
        ~Connection();
        Channel* getChannel();
        void setReleaseCall(std::function<void(Connection* connection)> call);
        void handleReadEvent(const int& fd);
        void handleWriteEvent(const int& fd);
    };
}
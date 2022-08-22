#pragma once

#include <vector>

namespace simplemuduo
{
    class Channel;
    
    class Epoll
    {
    private:
        int m_epollfd;
        std::vector<struct epoll_event> m_events;

    public:
        Epoll();
        ~Epoll();

        int getEpollFd() { return m_epollfd; };
        void epollDel(int fd);
        void epollMod(struct epoll_event& ev);
        void updateChannel(Channel* channel);
        std::vector<Channel*> epoll(int timeout = -1);
    };
}
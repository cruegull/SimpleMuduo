#include <stdio.h>
#include <signal.h>
#include <string>
#include <functional>
#include <thread>
#include <chrono>

#include "../src/include/ThreadPool.h"
#include "../src/include/Socket.h"
#include "../src/include/InetAddress.h"
#include "../src/include/Buffer.h"

using namespace simplemuduo;

void connectFunc(int msg_num, int wait_seconds)
{
    Socket sock;
    // InetAddress bind_addr("192.168.37.129", 7000);
    // sock.bind(bind_addr);

    InetAddress connect_addr("192.168.37.129", 8080);
    if (sock.connect(connect_addr) == -1)
        return;

    std::this_thread::sleep_for(std::chrono::seconds(wait_seconds));

    Buffer sendbuf, recvbuf;
    for (int count = 0; count < msg_num;)
    {
        sendbuf.setBuf("测试消息");
        int send_left = sendbuf.getSize();
        for (;;)
        {
            int send_cnt = ::send(sock.getSocketFd(), sendbuf.getBuf() + sendbuf.getSize() - send_left, send_left, MSG_DONTWAIT);
            if (send_cnt > 0)
            {
                send_left -= send_cnt;
                if (send_left <= 0)
                {
                    ++count;
                    break;
                }
            }
            else if (send_cnt == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            else if (send_cnt == -1)
            {
                printf("disconnected send err\n");
                return;
            }
        }

        int recved_cnt = 0;
        char buf[sendbuf.getSize()] = { 0 };
        for (;;)
        {
            int recv_cnt = ::recv(sock.getSocketFd(), buf, sizeof(buf), MSG_DONTWAIT);
            if (recv_cnt > 0)
            {
                recvbuf.append(buf, recv_cnt);
                recved_cnt += recv_cnt;
                if (recved_cnt >= sendbuf.getSize())
                {
                    printf("recv count=%d size=%d buf=%s\n", count, recvbuf.getSize(), recvbuf.getBuf());
                    break;
                }
            }
            else if (recv_cnt == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
            {
                continue;
            }
            else if (recv_cnt == 0)
            {
                printf("disconnected recv err");
                return;
            }
        }
        recvbuf.clear();
    }
}

int main(int argc, char *argv[])
{
    int thread_num = 1, msg_num = 0, wait_seconds = 0;
    int opt = 0;
    while ((opt = getopt(argc, argv, "t:m:w::")) != -1)
    {
        switch (opt)
        {
        case 't':
            thread_num = atoi(optarg);
            break;
        case 'm':
            msg_num = atoi(optarg);
            break;
        case 'w':
            wait_seconds = atoi(optarg);
            break;
        default:
            printf("error opterr: %d\n", opterr);
            break;
        }
    }
    printf("t=%d, m=%d, w=%d\n", thread_num, msg_num, wait_seconds);

    ThreadPool thread_pool(thread_num);
    std::function<void()> func = std::bind(connectFunc, msg_num, wait_seconds);
    for (int i = 0; i < thread_pool.getSize(); ++i)
    {   
        thread_pool.pushTask(func);
    }
    //int a = getchar();
    return 0;
}
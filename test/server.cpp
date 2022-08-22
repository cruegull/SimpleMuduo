#include <signal.h>
#include <assert.h>
#include <string>

#include "../src/include/EventLoop.h"
#include "../src/include/Server.h"

using namespace simplemuduo;

int main(int argc, char* argv[])
{
    int opt = 0;
    std::string ip = "0";
    short port = 0;
    while ((opt = getopt(argc, argv, "i:p:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            ip = optarg;
            break;
        case 'p':
            port = atoi(optarg);
            break;
        default:
            printf("options error\n");
            exit(-1);
            break;
        }
    }
    assert(ip != "0");
    assert(port != 0);
    printf("listen ip = %s port = %d\n", ip.c_str(), port);

    EventLoop eventloop;
    Server server(eventloop, ip.c_str(), port);
    eventloop.loop();

    return 0;
}
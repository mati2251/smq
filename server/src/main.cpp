#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "event-loop/event-loop.h"
#include <csignal>
#include "configure/configure.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <port> <package-lifetime-in-seconds>" << std::endl;
        return 1;
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        std::cout << "socket failed" << std::endl;
    sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[1])),
        .sin_addr = {htonl(INADDR_ANY)},
        .sin_zero = {0}
    };
    const int one = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    int err = bind(sock, (sockaddr *)&addr, sizeof(addr));
    if (err < 0)
    {
        std::cout << "Error binding socket" << std::endl;
        return 1;
    }
    err = listen(sock, 5);
    if (err < 0)
    {
        std::cout << "Error listening on socket" << std::endl;
        return 1;
    }
    std::cout << "Listening on port " << argv[1] << std::endl;
    set_conf(argv);
    EventLoop *loop = new EventLoop(sock);
    loop->run();
    return 0;
}

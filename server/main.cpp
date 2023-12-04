#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <csignal>
#include "src/event-loop.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
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
    EventLoop *loop = new EventLoop(sock);
    loop->Run();
    delete loop;
    close(sock);
    return 0;
}
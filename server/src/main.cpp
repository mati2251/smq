#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "event-loop/event-loop.h"
#include <csignal>
#include "configure/configure.h"

int main(int argc, char **argv) {
    if (argc != 2 && argc != 4) {
        std::cout << "Usage: " << argv[0] << " <port> <buffer-size> <package-lifetime>" << std::endl;
        return 1;
    }
    if (argc == 4) {
        int buffer_size = static_cast<int>(strtod(argv[2], nullptr));
        float package_lifetime = static_cast<float>(strtod(argv[3], nullptr));
        if (buffer_size == 0 || package_lifetime == 0) {
            std::cout << "Warnning: package lifetime or buffer set to 0" << std::endl;
        }
        set_buffer_size_conf(buffer_size);
        set_package_lifetime_conf(package_lifetime);
    } else {
        set_buffer_size_conf(-1);
        set_package_lifetime_conf(-1);
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
    int err = bind(sock, (sockaddr *) &addr, sizeof(addr));
    if (err < 0) {
        std::cout << "Error binding socket" << std::endl;
        return 1;
    }
    err = listen(sock, 5);
    if (err < 0) {
        std::cout << "Error listening on socket" << std::endl;
        return 1;
    }
    std::cout << "Listening on port " << argv[1] << std::endl;
    EventLoop *loop = new EventLoop(sock);
    loop->run();
    return 0;
}

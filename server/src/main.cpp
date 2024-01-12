#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "event-loop/event-loop.h"
#include <csignal>
#include "configure/configure.h"

EventLoop *loop;

int main(const int argc, char **argv) {
    if (argc != 2 && argc != 4) {
        std::cout << "Usage: " << argv[0] << " <port> <buffer-size> <package-lifetime>" << std::endl;
        return 1;
    }
    if (argc == 4) {
        char *endptr;
        const auto buffer_size = static_cast<unsigned int>(strtoul(argv[2], &endptr, 10));
        if (errno != 0 || *endptr != '\0') {
            std::cout << "Usage: " << argv[0] << " <port> <buffer-size> <package-lifetime>" << std::endl;
            return 1;
        }
        const float package_lifetime = strtof(argv[3], &endptr);
        if (errno != 0 || *endptr != '\0') {
            std::cout << "Usage: " << argv[0] << " <port> <buffer-size> <package-lifetime>" << std::endl;
            return 1;
        }
        set_buffer_size_conf(buffer_size);
        set_package_lifetime_conf(package_lifetime);
    } else {
        set_buffer_size_conf(0);
        set_package_lifetime_conf(0);
    }
    const int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        std::cout << "socket failed" << std::endl;
    sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[1])),
        .sin_addr = {htonl(INADDR_ANY)},
        .sin_zero = {0}
    };
    constexpr int one = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    signal(SIGPIPE, SIG_IGN);
    int err = bind(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
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
    signal(SIGINT, [](int) {
        loop->stop();
        std::cout << "Server stopped" << std::endl;
        exit(0);
    });
    loop = new EventLoop(sock);
    loop->run();
    return 0;
}

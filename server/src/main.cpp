#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "event-loop/event-loop.h"
#include <csignal>
#include "configure/configure.h"
#include <fstream>
#include <sstream>

int main(int argc, char **argv)
{
    char **args;
    if (argc != 2 && argc != 4)
    {
        std::cout << "Usage: " << argv[0] << " [-f] <port> <package-lifetime-in-seconds> <buffer-size>" << std::endl;
        return 1;
    } else if (!strcmp(argv[1], "-f"))
    {
        std::ifstream inputFile("../configuration_file.txt");
        if (!inputFile.is_open()) {
            std::cerr << "Error opening file" << std::endl;
            return 1;
        }

        std::string line;
        std::getline(inputFile, line);
        std::getline(inputFile, line);
        std::getline(inputFile, line);
        line = line + " ";

        std::vector<std::string> arguments;
        long unsigned int i = 0;
        while (i < line.length()) {
            if (line[i] == ' ') {
            arguments.push_back(line.substr(0, i));
            line.erase(0, i + 1);
            i = 0;
            } else {
            i++;
            }
        }

        // Convert the arguments to argv
        int size = arguments.size() + 1;
        args = new char*[4];
        for (int i = 1; i < size; i++) {
            args[i] = (char*)arguments[i-1].c_str();
            std::cout << args[i] << std::endl;
        }
        inputFile.close();
    } else {
        args = argv;
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        std::cout << "socket failed" << std::endl;
    sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(atoi(args[1])),
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
    std::cout << "Listening on port " << args[1] << std::endl;
    set_conf(args);
    EventLoop *loop = new EventLoop(sock);
    loop->run();
    return 0;
}

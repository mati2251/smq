class EventLoop
{
public:
    EventLoop(int &sock);
    ~EventLoop();

    void Run();
    void Stop();

private:
    int sock;
    int epoll_fd;
};
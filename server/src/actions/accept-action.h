#include "event-action.h"
#include <sys/socket.h>

class AcceptAction : public EventAction
{
public:
    AcceptAction(int fd, int &epoll_fd);
    void action();
    epoll_event& getEpollEvent();
private:
    int epoll_fd;
};

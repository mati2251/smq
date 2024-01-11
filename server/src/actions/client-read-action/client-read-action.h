#pragma once
#define BUFFER_SIZE 1024

#include "../event-action.h"
#include "../../structs/request.h"



class ClientReadAction : public EventAction
{
    public:
    ClientReadAction(int fd, int efd);

    ~ClientReadAction() override;
    void action() override;
private:
    std::string buffer;
    void readRequest();
    bool checkEndOfRequest() const;
    void closeConnection() const;
};

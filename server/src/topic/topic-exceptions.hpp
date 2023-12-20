#include <string>
#include <exception>

class TopicNotFoundException : public std::exception
{
public:
    TopicNotFoundException(const std::string &topic) : topic(topic) {}
    const char *what() const noexcept override
    {
        return ("Topic " + topic + " not found").c_str();
    }
private:
    std::string topic;
};

class ClientNotPublisherException : public std::exception
{
public:
    ClientNotPublisherException(const int &client, const std::string &topic) : client(client), topic(topic) {}
    const char *what() const noexcept override
    {
        return ("Client " + std::to_string(client) + " tried to send to not exist topic or client isn't register as publisher " + topic).c_str();
    }
private:
    int client;
    std::string topic;
};
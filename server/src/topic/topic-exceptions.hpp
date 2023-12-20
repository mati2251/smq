#pragma once
#include <string>
#include <exception>

class TopicNotFoundException : public std::exception
{
public:
    TopicNotFoundException(const std::string &topic) : topic(topic) {}
    const char *what() const noexcept override
    {
        std::string msg = "Topic " + topic + " not found";
        return msg.c_str();
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
        std::string  msg = "Client " + std::to_string(client) + " tried to send to not exist topic or client isn't register as publisher " + topic;
        return msg.c_str();
    }
private:
    int client;
    std::string topic;
};

class ClientNotSubscriberException : public std::exception
{
public:
    ClientNotSubscriberException(const int &client, const std::string &topic) : client(client), topic(topic) {}
    const char *what() const noexcept override
    {
        std::string msg = "Client " + std::to_string(client) + " tried to read from not exist topic or client isn't register as subscriber " + topic;
        return msg.c_str();
    }
private:
    int client;
    std::string topic;
};

class ClientAlreadySubscriberException : public std::exception
{
public:
    ClientAlreadySubscriberException(const int &client, const std::string &topic) : client(client), topic(topic) {}
    const char *what() const noexcept override
    {
        std::string msg = "Client " + std::to_string(client) + " already subscribed to " + topic;
        return msg.c_str();
    }
private:
    int client;
    std::string topic;
};

class ClientAlreadyPublisherException : public std::exception
{
public:
    ClientAlreadyPublisherException(const int &client, const std::string &topic) : client(client), topic(topic) {}
    const char *what() const noexcept override
    {
        std::string msg = "Client " + std::to_string(client) + " already publish to " + topic;
        return msg.c_str();
    }
private:
    int client;
    std::string topic;
};

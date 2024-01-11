#pragma once
#include <string>
#include <exception>

class TopicNotFoundException : public std::exception
{
public:
    TopicNotFoundException(const std::string &topic) {
        this->msg = "Topic " + topic + " not found";
    }
    const char *what() const noexcept override
    {
        return this->msg.c_str();
    }
private:
    std::string msg;
};

class ClientNotPublisherException : public std::exception
{
public:
    ClientNotPublisherException(const int &client, const std::string &topic) {
        this->msg = "Client " + std::to_string(client) + " tried to publish to not exist topic or client isn't register as publisher " + topic;
    }
    const char *what() const noexcept override
    {
        return this->msg.c_str();
    }
private:
    std::string msg;
};

class ClientNotSubscriberException : public std::exception
{
public:
    ClientNotSubscriberException(const int &client, const std::string &topic) {
        this->msg = "Client " + std::to_string(client) + " tried to subscribe to not exist topic or client isn't register as subscriber " + topic;
    }
    const char *what() const noexcept override
    {
        return this->msg.c_str();
    }
private:
    std::string msg;
};

class ClientAlreadySubscriberException : public std::exception
{
public:
    ClientAlreadySubscriberException(const int &client, const std::string &topic) {
        this->msg = "Client " + std::to_string(client) + " already subscribed to " + topic;
    }
    const char *what() const noexcept override
    {
        return this->msg.c_str();
    }
private:
    std::string msg;
};

class ClientAlreadyPublisherException : public std::exception
{
public:
    ClientAlreadyPublisherException(const int &client, const std::string &topic) {
        this->msg = "Client " + std::to_string(client) + " already publish to " + topic;
    }
    const char *what() const noexcept override
    {
        return this->msg.c_str();
    }
private:
    std::string msg;
};

class NoSubribersException : public std::exception {
public:
    const char *what() const noexcept override
    {
        return "No subscribers to topic, message not sent";
    }
};

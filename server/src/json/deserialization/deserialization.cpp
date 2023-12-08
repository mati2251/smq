#include "deserialization.h"

const char whiteChars[] = " \t\n\r\f\v";

bool isWhiteChar(char c)
{
    for (int i = 0; i < 6; i++)
    {
        if (c == whiteChars[i])
        {
            return true;
        }
    }
    return false;
}

void findChar(const std::string &json, size_t &pos, char character)
{
    while (json[pos] != character)
    {
        if (!isWhiteChar(json[pos]))
        {
            throw DeserializationException("Unexpected character " + json[pos]);
        }
        pos++;
    }
    pos++;
}

std::string findString(const std::string &json, size_t &pos)
{
    std::string result = "";
    while (json[pos] != '"')
    {
        if (!isWhiteChar(json[pos]))
        {
            throw DeserializationException("Unexpected character " + json[pos]);
        }
        pos++;
    }
    pos++;
    while (json[pos] != '"')
    {
        result += json[pos];
        pos++;
    }
    pos++;
    return result;
}

void keyValToMessage(const std::string &key, const std::string &val, message &msg)
{
    if (key == "topic")
    {
        msg.topic = val;
    }
    else if (key == "content")
    {
        msg.content = val;
    }
    else
    {
        throw DeserializationException("Unexpected key" + key);
    }
}

message deserializeMessage(const std::string &json)
{
    size_t pos = 0;
    findChar(json, pos, '{');
    std::string key = findString(json, pos);
    findChar(json, pos, ':');
    std::string val = findString(json, pos);
    findChar(json, pos, ',');
    std::string key2 = findString(json, pos);
    findChar(json, pos, ':');
    std::string val2 = findString(json, pos);
    findChar(json, pos, '}');
    message msg;
    keyValToMessage(key, val, msg);
    keyValToMessage(key2, val2, msg);
    return msg;
}

void keyValToActionTopic(const std::string &key, const std::string &val, action_topic &act)
{
    if (key == "topic")
    {
        act.topic = val;
    }
    else if (key == "action")
    {
        act.action = val;
    }
    else
    {
        throw DeserializationException("Unexpected key " + key);
    }
}

action_topic deserializeAction(const std::string &json)
{
    size_t pos = 0;
    findChar(json, pos, '{');
    std::string key = findString(json, pos);
    findChar(json, pos, ':');
    std::string val = findString(json, pos);
    findChar(json, pos, ',');
    std::string key2 = findString(json, pos);
    findChar(json, pos, ':');
    std::string val2 = findString(json, pos);
    findChar(json, pos, '}');
    action_topic act;
    keyValToActionTopic(key, val, act);
    keyValToActionTopic(key2, val2, act);
    return act;
}
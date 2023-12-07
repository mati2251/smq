#include "request-util.h"

request_type getRequestType(std::string request_type_str)
{
    for (int i = 0; i < (int)request_stype->size(); i++)
    {
        if (request_stype[i] == request_type_str)
        {
            return (request_type)i;
        }
    }
    return (request_type)-1;
}
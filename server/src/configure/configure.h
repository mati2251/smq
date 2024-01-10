#pragma once

#include <iostream>

struct configure{
    int port;
    float package_lifetime;
};

void set_conf(char** argv);

float get_package_lifetime_conf();
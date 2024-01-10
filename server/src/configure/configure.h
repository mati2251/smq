#pragma once

#include <iostream>

struct configure{
    int port;
    float package_lifetime;
    long unsigned int buffer_size;
};

void set_conf(char** argv);

float get_package_lifetime_conf();

long unsigned int get_buffer_size_conf();
#pragma once


struct configure{
    int port;
    float package_lifetime;
    unsigned int buffer_size;
};

void set_port_conf(int port);
void set_package_lifetime_conf(float package_lifetime);
void set_buffer_size_conf(unsigned int buffer_size);

float get_package_lifetime_conf();
int get_port_conf();
unsigned int get_buffer_size_conf();

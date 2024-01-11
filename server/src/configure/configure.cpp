#include "configure.h"

struct configure conf {.port = 3000, .package_lifetime = -1.0, .buffer_size = -1};

void set_port_conf(int port){
    conf.port = port;
}

void set_package_lifetime_conf(float package_lifetime){
    conf.package_lifetime = package_lifetime;
}

void set_buffer_size_conf(int buffer_size){
    conf.buffer_size = buffer_size;
}

float get_package_lifetime_conf(){
    return conf.package_lifetime;
}

float get_port_conf(){
    return conf.port;
}

float get_buffer_size_conf(){
    return conf.buffer_size;
}
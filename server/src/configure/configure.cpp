#include "configure.h"

struct configure conf {.port = 3000, 
                        .package_lifetime = std::numeric_limits<float>::max(), 
                        .buffer_size = std::numeric_limits<unsigned int>::max()};

void set_port_conf(int port){
    conf.port = port;
}

void set_package_lifetime_conf(float package_lifetime){
    if(package_lifetime >= 0)
        conf.package_lifetime = package_lifetime;
}

void set_buffer_size_conf(unsigned int buffer_size){
    conf.buffer_size = buffer_size;
}

float get_package_lifetime_conf(){
    return conf.package_lifetime;
}

int get_port_conf(){
    return conf.port;
}

unsigned int get_buffer_size_conf(){
    return conf.buffer_size;
}
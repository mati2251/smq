#include "configure.h"

struct configure conf;

void set_conf(char** argv){
    conf.package_lifetime = atof(argv[2]);
    conf.port = atoi(argv[1]);
    conf.buffer_size = atoi(argv[3]);
}

float get_package_lifetime_conf(){
    return conf.package_lifetime;
}

long unsigned int get_buffer_size_conf(){
    return conf.buffer_size;
}
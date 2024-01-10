#include "configure.h"

struct configure conf;

void set_conf(char** argv){
    conf.package_lifetime = atof(argv[2]);
    conf.port = atoi(argv[1]);
}

float get_package_lifetime_conf(){
    return conf.package_lifetime;
}
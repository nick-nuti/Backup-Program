#ifndef LINUXCONFIG
#define LINUXCONFIG

#include <filesystem>
#include <unistd.h>
#include <vector>

const std::string config_location = "/var/lib/backup_program";
const std::string config = config_location + '/' + "config.json";
const std::string debuglog = config_location + '/' + "DEBUG.log";

bool checkadmin();

#endif

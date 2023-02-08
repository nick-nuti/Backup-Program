#ifndef WINDOWSCONFIG
#define WINDOWSCONFIG

#include <Windows.h>
#include <shlobj.h>
#include <string>

const std::string config_location = std::string(std::getenv("LOCALAPPDATA")) + "\\backup_program";
const std::string config = config_location + '\\' + "config.json";
const std::string debuglog = config_location + '\\' + "DEBUG.log";

bool checkadmin();

#endif

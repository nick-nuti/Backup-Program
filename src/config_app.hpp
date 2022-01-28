#ifndef CONFIGAPP
#define CONFIGAPP

#include <iostream>
#include <filesystem>
#include <stdio.h>

#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "rapidjson/filereadstream.h"

namespace config_app
{
    void os_config();

    std::string config_digest();

    void update_config(const std::string pathtobackup, const int backupfreq);
}
#endif
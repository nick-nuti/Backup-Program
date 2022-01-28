#ifndef FILECOPY
#define FILECOPY

#include <filesystem>
#include <stdio.h>
#include <iostream>

#include "tree.hpp" //for the BackupParams struct

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

void filecopy(BackupParams backup_location, std::string json_location, std::string backupDir_fullpath);

#endif
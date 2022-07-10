#include <filesystem>
#include <iostream>
#include <chrono>
#include <fstream>
#include <stdio.h>

#include "tree.hpp"
#include "filecopy.hpp"
#include "sha256_hash.hpp"
#include "config_app.hpp"

#include "debug_logger.hpp"

namespace fs = std::filesystem;

int main(int argc, char **argv) 
{  
    if(debug_logger::debug_thread == 1) std::cout << "DEBUG THREAD ACTIVE\n";

    auto start_time = std::chrono::steady_clock::now();

//create config dir + file if they don't exist
    config_app::os_config(); 
    std::string inputJSONPath;

    if(argv[1] != NULL)
    {
        inputJSONPath = argv[1];
        debug_logger::print("Building digest from: %v\n",inputJSONPath);
    }

//if run without input arg, check available backups from config
    else inputJSONPath = config_app::config_digest();

    if(inputJSONPath.empty())
    {
        debug_logger::print("No backup path selected. Exiting...\n");
        return 0;
    }

    BackupParams backupParams = generateBackupParams(inputJSONPath);

// create named backup parent directory (holds invidual backups) 
    if(!fs::exists(backupParams.outputPath + backupParams.backupLabel))    
    {
        fs::create_directories(backupParams.outputPath + backupParams.backupLabel);
    }

    std::string backupDigestLocation, backupDir;
    int backupFreq;

    buildDigest(backupParams, backupDigestLocation, backupDir);

// update config should be right here
    config_app::update_config(backupDir, backupParams.backupFreq);

    //filecopy(backupParams, backupDigestLocation, backupDir);

    auto end_time = std::chrono::steady_clock::now();

    debug_logger::print("Time Elapsed: %v ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time).count());
    return 0;
}
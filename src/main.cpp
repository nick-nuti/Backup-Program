// clang++ -std=c++17 ./*.cpp -o ./main.out -lstdc++fs

#include <filesystem>
#include <iostream>
#include <chrono>
#include <fstream>
#include <stdio.h>

#include "tree.hpp"
#include "filecopy.hpp"
#include "sha256_hash.hpp"
#include "config_app.hpp"

namespace fs = std::filesystem;

int main(int argc, char **argv) 
{  
    auto start_time = std::chrono::steady_clock::now();

    config_app::os_config(); //create config dir + file if they don't exist
    std::string inputJSONPath;

    if(argv[1] != NULL)
    {
        inputJSONPath = argv[1];
        std::cout << "Building digest from: " << inputJSONPath << std::endl;
    }

    //if run without input arg, check available backups from config
    else inputJSONPath = config_app::config_digest();

    if(inputJSONPath.empty())
    {
        std::cout << "No backup path selected. Exiting..." << std::endl;
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

    std::cout << "Time Elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time).count() << " ms" << std::endl;

    return 0;
}
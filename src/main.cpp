// clang++ -std=c++17 ./*.cpp -o ./main.out -lstdc++fs

#include <filesystem>
#include <iostream>
#include <chrono>
#include <fstream>
#include <string>

#include "tree.hpp"

int main(int argc, char **argv) 
{  
    std::string inputJSONPath = argv[1];
    std::cout << "Building digest from: " << argv[1] << std::endl << std::endl;

    BackupParams backupParams = generateBackupParams(inputJSONPath);
    
    buildDigest(backupParams);
    
    return 0;

}
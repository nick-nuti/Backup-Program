<<<<<<< HEAD
// clang++ -std=c++17 ./*.cpp -o ./main.out -lstdc++fs

#include <filesystem>
#include <iostream>
#include <chrono>
#include <fstream>
#include <stdio.h>

#include "tree.hpp"
#include "filecopy.hpp"
#include "sha256_hash.hpp"

using namespace std::filesystem;

int main(int argc, char **argv) 
{  
    auto start_time = chrono::steady_clock::now();

    std::string inputJSONPath = argv[1];
    std::cout << "Building digest from: " << argv[1] << std::endl << std::endl;

    BackupParams backupParams = generateBackupParams(inputJSONPath);

    // create named backup parent directory (holds invidual backups) 
    if(!exists(backupParams.outputPath + backupParams.backupLabel))    
    {
        create_directories(backupParams.outputPath + backupParams.backupLabel);
    }

    std::string backupDigestLocation;

    buildDigest(backupParams, backupDigestLocation);

    cout << "backupDigestLocation: " << backupDigestLocation << endl;

    // filecopy(backupDigestLocation);

    auto end_time = chrono::steady_clock::now();

    std::cout << "Time Elapsed: " << chrono::duration_cast<chrono::milliseconds>(end_time-start_time).count() << " ms" << endl;

    return 0;
=======
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

>>>>>>> ee33bb0c3328ae86ea9367ff5110360d210a1207
}
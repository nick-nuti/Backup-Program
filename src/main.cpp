// clang++ -std=c++17 ./*.cpp -o ./main.out -lstdc++fs

#include <filesystem>
#include <iostream>
#include <chrono>
#include <fstream>

#include "json.hpp"

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char **argv) 
{  
    /*
        Read input.json file and generate backupParameters
    */
    backupParameters params;
    generateInputParameters(&params, fs::absolute(argv[1]));
    uintmax_t space_required_acc = 0;

    /*
        Generate a JSON representation of the backup directory
    */
    nlohmann::json outputTree;
    generateOutputTree(&params, &outputTree, &space_required_acc);

    /*
        Check space required for backup
    */
    int err_check;
    space_check(get<0>(params.output), space_required_acc, err_check);


    /*
        Write JSON tree to file in backup location 
    */
    string outputDataWritePath = get<0>(params.output) + get<1>(params.output) + ".json";
    cout << "Writing json backup to: " << outputDataWritePath << endl;
    writeJSON(&outputTree, &outputDataWritePath);

    return 0;

}
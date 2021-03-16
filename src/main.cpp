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
    if(argv[1] != NULL)
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
        bool err_check = true;
        space_check(get<0>(params.output), space_required_acc, err_check);

        if(err_check == true)
        {
            /*
                Write JSON tree to file in backup location 
            */
            string outputDataWritePath;
            
            if(params.output.first[params.output.first.length()-1] == 47) outputDataWritePath = params.output.first + params.output.second + ".json";
            else outputDataWritePath = params.output.first + "/" + params.output.second + ".json";

            cout << "Writing json backup to: " << outputDataWritePath << endl;

            writeJSON(outputTree, outputDataWritePath);
        }
    }

    else cout << "\nPlease specify input.json file as an argument.\n\n";

    return 0;

}
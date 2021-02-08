// clang++ -std=c++17 ./*.cpp -o ./main.out -lstdc++fs

#include <filesystem>
#include <iostream>
#include <chrono>

#include "json.hpp"

using namespace std;

int main(int argc, char **argv, char *envp[])
{
    char *sourceDir = argv[1];
    char *destinationDir = argv[2];

    printf("Using source directory: %s\n\n", sourceDir);

    backupParameters params;
    generateInputParameters(&params);

    cout << get<0>(params.output) << endl;

    vector<Node> roots;
    nlohmann::json::array_t container;

    for(int i = 0; i < params.input.size(); i++) {
        string path = get<0>(params.input.at(i));
        string flag = get<1>(params.input.at(i));

        Node root = { path, 0 };
        populate(root.path.c_str(), &root, flag);

        
        generateOutputTree(&root, &container);
    }

    nlohmann::json::object_t outputData = nlohmann::json::object({
        { "path", get<0>(params.output) },
        { "name",  get<1>(params.output) },
        { "files", container }
    });

    string outputDataWritePath = "./output.json";
    writeJSON(&outputData, &outputDataWritePath);

    return 0;
}
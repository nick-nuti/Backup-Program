#ifndef JSON_HEADER
#define JSON_HEADER

#include <string>
//#include <vector> //included in std library
#include <nlohmann/json.hpp>

#include "directoryTree.hpp"

struct backupParameters
{
    std::pair<std::string, std::string> output; // pair: output -> path, output -> name
    std::vector<std::pair<std::string, std::string>> input; // vector: <(inputfile -> path, inputfile -> type),...>
};

void writeJSON(nlohmann::json &obj, std::string fileout);

void generateOutputTree(backupParameters *params, nlohmann::json *tree, uintmax_t *space_required_acc);

void generateOutputTree(Node *node, nlohmann::json::array_t *container, nlohmann::json::array_t *branch = {}, bool root = true);

void generateInputParameters(backupParameters *params, std::string out_path);

#endif
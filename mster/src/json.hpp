#ifndef JSON_HEADER
#define JSON_HEADER

#include <vector>
#include <nlohmann/json.hpp>
#include "directoryTree.hpp"

struct backupParameters
{
    std::pair<std::string, std::string> output;
    std::vector<std::pair<std::string, std::string>> input;
};

void writeJSON(nlohmann::json::object_t *obj, std::string *writeLocation);

void generateOutputTree(Node *node, nlohmann::json::array_t *container, nlohmann::json::array_t *branch = {}, bool root = true);

void generateInputParameters(backupParameters *params);

#endif
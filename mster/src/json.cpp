#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

#include "directoryTree.hpp"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

void writeJSON(json::object_t *obj, string *writeLocation)
{
    ofstream jsonFile("./output.json");
    jsonFile << *obj;
    jsonFile.close();
}

void generateOutputTree(Node *node, json::array_t *container, json::array_t *pArr, bool root) {

    json::object_t metadata = json::object({
        { "path", node->path },
        { "size", node->size },
        { "timeLastWritten", node->timeLastWritten }
    });

    // Directories do not have a filename, nor do they have permissions.
    if (node->filename != "") metadata["filename"] = node->filename;
    if (node->perms != "") metadata["perms"] = node->perms;

    if (node->children.size() > 0) 
    {
        json::array_t children = json::array({});

        for(int i = 0; i < node->children.size(); i++) {
            Node child_ptr = node->children.at(i);
            generateOutputTree(&child_ptr, nullptr, &children, false);
        }

        metadata["children"] = children;
    }

    if (root == false) {
        pArr->push_back(metadata);
    } else {
        container->push_back(metadata);
    }
}

// read from json input file
// determine which trees need to be built
// assign appropriate flags (f = file only, sd = surface directory (no children), fd = file directory (a.k.a. normal dir tree))
// pass this info

void generateInputParameters(backupParameters *params)
{
    string inputFileLocation = "./input.json";

    ifstream rawJSONFile(inputFileLocation);
    json parsed = json::parse(rawJSONFile);

    params->output = make_pair(parsed["output"]["path"], parsed["output"]["name"]);

    for(int i = 0; i < parsed["input"].size(); i++) {
        params->input.push_back(make_pair(parsed["input"].at(i)["path"], parsed["input"].at(i)["type"]));
    }
}

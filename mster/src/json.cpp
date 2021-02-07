#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

#include "directoryTree.h"

using namespace std;
using json = nlohmann::json;

void writeJSON(json::object_t *obj, string *writeLocation)
{
    ofstream jsonFile("out.json");
    jsonFile << *obj;
    jsonFile.close();
}

void generateOutputTree(Node *node, json::array_t *pArr, bool root) {

    json::object_t metadata = json::object({
        { "path", node->path },
        { "size", node->size },
        { "timeLastWritten", node->timeLastWritten }
    });

    if (node->filename != "") metadata["filename"] = node->filename;
    if (node->perms != "") metadata["perms"] = node->perms;

    if (node->children.size() > 0) 
    {
        json::array_t children = json::array({});

        for(int i = 0; i < node->children.size(); i++) {
            Node child_ptr = node->children.at(i);
            generateOutputTree(&child_ptr, &children, false);
        }

        metadata["children"] = children;
    }

    if (root == false) {
        pArr->push_back(metadata);
    } else {
        string writeLoc = "./";
        writeJSON(&metadata, &writeLoc);
    }
}

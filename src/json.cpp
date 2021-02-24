#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

#include "directoryTree.hpp"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

void writeJSON(json *obj, string *writeLocation)
{
    ofstream jsonFile(writeLocation->c_str());
    jsonFile << *obj;
    jsonFile.close();
}

void generateOutputTree(backupParameters *params, json *tree, uintmax_t *space_required_acc) 
{
    json::array_t container;
    vector<Node> roots;
    
    string path;
    int8_t flag;
    uintmax_t s_lw [2] = {0,0};
    map<string, int> flag_to_int {{"f", 2},{"sd", 1},{"fd", 0}};

    for(int i = 0; i < params->input.size(); ++i) 
    {
        path = get<0>(params->input.at(i));
        flag = flag_to_int[get<1>(params->input.at(i))];

        size_lastwrite(path, s_lw[0], s_lw[1]);

        roots.push_back(Node());

        roots[i] = (Node) { .path = path,
                            .perms = read_perms(path),
                            .size = s_lw[0],
                            .timeLastWritten = s_lw[1]
                            };

        if(flag < 2)
        {
            populate(path, flag, &roots[i]);
        }

        *space_required_acc += roots[i].size;

        generateOutputTree(&roots[i], &container);
    }
    
    *tree = json::object({
        { "path", get<0>(params->output) },
        { "name",  get<1>(params->output) },
        { "files", container }
    });
}

void generateOutputTree(Node *node, json::array_t *container, json::array_t *pArr, bool root) 
{

    json::object_t metadata = json::object({
        { "path", node->path },
        { "size", node->size },
        { "timeLastWritten", node->timeLastWritten }
    });

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

void generateInputParameters(backupParameters *params, std::string out_path)
{
    ifstream rawJSONFile(out_path);
    json parsed = json::parse(rawJSONFile);

    params->output = make_pair(parsed["output"]["path"], parsed["output"]["name"]);

    for(int i = 0; i < parsed["input"].size(); ++i)
    {
        params->input.push_back(make_pair(parsed["input"].at(i)["path"], parsed["input"].at(i)["type"]));
    }
}

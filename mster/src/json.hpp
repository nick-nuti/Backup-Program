#include <nlohmann/json.hpp>
#include "directoryTree.hpp"

void generateOutputTree(Node *node, nlohmann::json::array_t *branch = {}, bool root = true);

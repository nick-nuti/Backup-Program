#include <nlohmann/json.hpp>
#include "directoryTree.h"

void generateOutputTree(Node *node, nlohmann::json::array_t *branch = {}, bool root = true);

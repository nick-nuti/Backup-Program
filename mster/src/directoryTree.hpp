#ifndef DIR_TREE_HEADER
#define DIR_TREE_HEADER

#include <vector>

struct Node
{
    Node(std::string p, int s = 0) : path(p), size(s){};

    std::string path;
    std::string filename;
    std::string perms;
    int size;
    int timeLastWritten;
    std::vector<Node> children;
};

void populate(const char *dir, Node *parent, std::string flag = "");

void printTree(Node node, int padding = 0);

#endif
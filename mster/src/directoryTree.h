

typedef struct Node
{
    Node(std::string p, int s = 0) : path(p), size(s){};

    std::string path;
    std::string perms;
    int size;
    int timeLastWritten;
    std::vector<Node> children;
} Node;

void populate(const char *dir, Node *parent);

void printTree(Node node, int padding = 0);
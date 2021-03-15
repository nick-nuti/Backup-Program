#ifndef DIR_TREE_HEADER
#define DIR_TREE_HEADER

#include <string>
#include <vector>
#include <sys/stat.h>

#ifndef WIN32
#include <unistd.h>
#endif

#ifdef WIN32
#define stat _stat
#endif

struct Node
{
    std::string path;
    std::string perms;
    uintmax_t size = 0;
    uintmax_t timeLastWritten = 0;
    std::vector<Node> children;
};

void size_lastwrite(std::string path, uintmax_t &size, uintmax_t &lastwrite);

std::string read_perms(std::string path);

void populate(const std::string path, const int flag, Node *parent);

void printTree(Node node, int padding = 0);

void space_check(std::string inputpath, uintmax_t space_required_acc, bool &err_check);

#endif
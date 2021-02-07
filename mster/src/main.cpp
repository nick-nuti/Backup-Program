// g++-8 -std=c++17 ./*.cpp -o ./main.out -lstdc++fs

#include <filesystem>
#include <iostream>
#include <chrono>

#include "directoryTree.h"

int main(int argc, char **argv, char *envp[])
{
    char *sourceDir = argv[1];
    char *destinationDir = argv[2];

    printf("Using source directory: %s\n\n", sourceDir);

    Node root = { sourceDir, 0 };
    populate(root.path.c_str(), &root);

    printTree(root, 0);

    return 0;
}
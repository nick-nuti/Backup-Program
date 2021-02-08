#include <filesystem>
#include <iostream>
#include <chrono>
#include <vector>

#include "directoryTree.hpp"

using namespace std;
namespace fs = filesystem;

/*
    An underscore as the first character in an ID is often used to indicate an internal 
    implementation that is not considered part of the API and should not be called by
    code outside that implementation.

    https://en.wikipedia.org/wiki/Underscore
*/

// maybe make void fn
int _timeLastWritten(string path) 
{
   return 1;
}

void _filePermissions(Node *file) 
{
    fs::perms filePerms = fs::status(file->path).permissions();
    file->perms = string((filePerms & fs::perms::owner_read) != fs::perms::none ? "r" : "-")
    + ((filePerms & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
    + ((filePerms & fs::perms::owner_exec) != fs::perms::none ? "x" : "-")
    + ((filePerms & fs::perms::group_read) != fs::perms::none ? "r" : "-")
    + ((filePerms & fs::perms::group_write) != fs::perms::none ? "w" : "-")
    + ((filePerms & fs::perms::group_exec) != fs::perms::none ? "x" : "-")
    + ((filePerms & fs::perms::others_read) != fs::perms::none ? "r" : "-")
    + ((filePerms & fs::perms::others_write) != fs::perms::none ? "w" : "-")
    + ((filePerms & fs::perms::others_exec) != fs::perms::none ? "x" : "-");
}

void populate(const char *dir, Node *parent, string flag)
{
    // file: file only
    if (flag == "f") {
        parent->timeLastWritten = _timeLastWritten(""); // placeholder
        parent->path = fs::path(parent->path).filename();
        parent->size = fs::file_size(parent->path);
        _filePermissions(parent);
    }

    // file directory or surface directory
    if (flag == "fd" || flag == "sd") 
    {
        for (fs::directory_entry entry : fs::directory_iterator(dir)) 
        {
            if (entry.is_directory() == true && flag == "fd") // only recurse on fd
            {
                string path = entry.path();

                struct Node child = { path, 0 };

                populate(path.c_str(), &child, flag);

                parent->children.push_back(child);
                parent->size += child.size;
            } 
            else if (entry.is_regular_file() == true) 
            {
                int size = fs::file_size(entry.path());
                parent->size += size;

                struct Node file = { entry.path(), size };

                file.timeLastWritten = _timeLastWritten(""); // placeholder
                _filePermissions(&file);
                file.filename = entry.path().filename();

                parent->children.push_back(file);
            };
        }
    }
}

void printTree(Node node, int padding)
{
    cout << string(padding, ' ') << node.path << ((padding == 0) ? " (root) " : " ") << "<" << node.size << "> " << node.timeLastWritten << endl;
    cout << string(padding, ' ') << node.perms << endl;

    if (node.children.size() > 0)
    {
        for (int i = 0; i < node.children.size(); i++)
        {
            printTree(node.children.at(i), padding + 4);
        };
    };
}

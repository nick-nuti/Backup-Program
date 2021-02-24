#include <filesystem>
#include <iostream>
//#include <chrono>
//#include <vector> //this should be in standard library
#include <sys/stat.h>
#include <ctime>
#include <string>

#include "directoryTree.hpp"

using namespace std;
namespace fs = std::filesystem;

/*
    An underscore as the first character in an ID is often used to indicate an internal 
    implementation that is not considered part of the API and should not be called by
    code outside that implementation.

    https://en.wikipedia.org/wiki/Underscore
*/

void size_lastwrite(string path, uintmax_t &size, uintmax_t &lastwrite)
{
    struct stat path_to_check;

    stat(path.c_str(), &path_to_check);
    //std::cout <<"PATH = " << path.c_str() << "\n";

    size = (fs::is_directory(path) == 0) ? path_to_check.st_size : 0;

    struct tm datemod;
    localtime_r(&path_to_check.st_mtime, &datemod);
    lastwrite = (((1900 + datemod.tm_year) * 10000000000)
                        +((1 + datemod.tm_mon) * 100000000)
                        +((datemod.tm_mday) * 1000000)
                        +((datemod.tm_hour) * 10000)
                        +((datemod.tm_min) * 100)
                        + datemod.tm_sec);
}

string read_perms(string path)
{
    fs::perms path_perms; 
    path_perms = fs::status(path).permissions();
    string permissions;
    permissions=((path_perms & fs::perms::owner_read) != fs::perms::none ? 'r' : '-');
    permissions+=((path_perms & fs::perms::owner_write) != fs::perms::none ? 'w' : '-');
    permissions+=((path_perms & fs::perms::owner_exec) != fs::perms::none ? 'x' : '-');
    permissions+=((path_perms & fs::perms::group_read) != fs::perms::none ? 'r' : '-');
    permissions+=((path_perms & fs::perms::group_write) != fs::perms::none ? 'w' : '-');
    permissions+=((path_perms & fs::perms::group_exec) != fs::perms::none ? 'x' : '-');
    permissions+=((path_perms & fs::perms::others_read) != fs::perms::none ? 'r' : '-');
    permissions+=((path_perms & fs::perms::others_write) != fs::perms::none ? 'w' : '-');
    permissions+=((path_perms & fs::perms::others_exec) != fs::perms::none ? 'x' : '-');

    return permissions;
}

//void populate(const char *dir, Node *parent, string flag)
void populate(const string path, const int flag, Node *parent)
{
    struct Node child;
    uintmax_t s_lw [2] = {0,0};

    for (fs::directory_entry entry : fs::directory_iterator(path)) 
    {
        switch(flag)
        {
            case 0:
                size_lastwrite(entry.path().c_str(), s_lw[0], s_lw[1]);

                child = (Node) {.path = entry.path().filename(),
                                .perms = read_perms(entry.path().c_str()),
                                .size = s_lw[0],
                                .timeLastWritten = s_lw[1]
                                };

                if (entry.is_directory() == true) populate(entry.path().c_str(), flag, &child);

                // when a dir is finished and the recursive function returns to right below root, 
                // the overall size of the sub directory is added to the root dir size
                parent->children.push_back(child);
                parent->size += child.size;
                
            break;

            case 1:

                if (entry.is_regular_file() == true) 
                {
                    size_lastwrite(entry.path().c_str(), s_lw[0], s_lw[1]);

                    child = (Node) {.path = entry.path().filename(),
                                    .perms = read_perms(entry.path().c_str()),
                                    .size = s_lw[0],
                                    .timeLastWritten = s_lw[1]
                                    };

                    parent->children.push_back(child);
                    parent->size += child.size;  
                }
            break;

            default:
                //problemo
                // maybe we error/exit here? This would occur because of user error
                // i.e. Not using "f", "fd", "sd", flags which will map to NULL
            break;
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

void space_check(string inputpath, uintmax_t space_required_acc, int &err_check)
{
    string inputpath_substr;
    uintmax_t space_available;

    for (int path_index = inputpath.length() - 1; path_index >= 0; --path_index)
    {
        // 47 should be "/"
        if (inputpath[path_index] == 47)
        {
            inputpath_substr = inputpath.substr(0, path_index);

            //std::cout << "inputpath_substr = " << inputpath_substr << "\n";

            if (fs::exists(inputpath_substr))
            {
                fs::space_info dev_space = fs::space(inputpath_substr);
                //std::cout << "Space available for current path = " << dev_space.free << "\n";
                space_available = dev_space.free;
                //std::cout << "Space available for current path = " << space_available << "\n";
                break;
            }
        }
    }

    if(space_available > space_required_acc)
    {
        cout << "There will be " << space_available-space_required_acc << " Bytes remaining after backup.\nProceed? [Y/n]: ";

        // Typically, prompts use a `Y/n` syntax which means YES is default.
        // `y/N` would mean NO is default.
        // allow == '\n' supports the enter key

        char allow = cin.get();
        if (!(allow == 'Y' || allow == 'y' || allow == '\n'))
        {
            //should exit + result in some sort of an error
            //err_check = an error
            cout << "Exiting...\n";
        } 
        else
        {
            cout << "Continuing...\n";
            fs::create_directories(inputpath);
        }
    }

    else
    {
        //should exit + result in some sort of an error
        //err_check = an error
        cout << "There wasn't enough space in " << inputpath << " to proceed.\n";
        cout << "Space available: " << space_available << " bytes, Space required: " << space_required_acc << " bytes\n" ;
    }
}
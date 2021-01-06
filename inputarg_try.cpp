#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

#include "backup_errors.hpp"

using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;

void err_num(int index)
{
    exception_class error_exception;
    error_exception.error_numb = index;
    throw error_exception;
}


std::string create_initial_JSON(std::string input_txt)
{
    char inout = 0b000; 
    std::string importantJSON;
    std::string importantJSON_direc = NULL;

    ifstream txt_file(input_txt);

    for(std::string current_line; getline (txt_file, current_line);)
    {
        
        std::cout << current_line << "\n";

        switch(inout)
        {
            case 0: 
            {
                if(current_line.find("output") != std::string::npos) inout = 0b001;
                break;
            }

            case 1:
            { 
                if(current_line.find("PATH:") != std::string::npos)
                {
                    if(current_line.find("...") != std::string::npos)
                    {
                        int pos2 = (current_line.find("..."))-5;
                        
                        importantJSON_direc = current_line.substr(5,pos2);

                        std::cout << importantJSON_direc << "\n";

                        if(!fs::exists(importantJSON_direc)) 
                        {
                            std::cout << "It not dere\n";
                            fs::create_directories(importantJSON_direc); // I could make this in C but it would need to be a seperate file
                        
                            // if system cannot create the directories then exit
                        }

                        fs::current_path(importantJSON_direc);
                        
                        // if system cannot access current path then provide an error and exit
                    }
                }
                
                if(current_line.find("NAME:") != std::string::npos)
                {
                    if(fs::current_path() == importantJSON_direc)
                    {
                        // need to check for those 
                        //while (pch!=NULL)
                        //{
                        //    printf ("found at %d\n",pch-str+1);
                        //    pch=strchr(pch+1,'s');
                        //}
                        //

                        fs::create_directory("::::                   "); //directory name cant have \ / * : ? " < > |
                    }

                    //else there should be an error....
                }

                break;
            }
        }
    }
    return importantJSON;
}


int main(int argc, char **argv)
{
    std::string importantJSON;
    std::string inputarg;

    try
    {
        if(argc > 1)
        {
            if((fs::exists(argv[1]))&&(strcmp(fs::path(argv[1]).extension().c_str(),".txt") == 0)) //if a file is specified and it exists in the specific path then grab the path and store it
            {
                std::cout << "The specified file exists" << fs::absolute(argv[1]) << '\n';
                importantJSON = create_initial_JSON(fs::absolute(argv[1]));
            }

            else err_num(2);
        }

        else err_num(1);
    }

    catch (exception_class& err)
    {
        std::cout << err.backup_error() << "\n";
        return 0;
    }
}

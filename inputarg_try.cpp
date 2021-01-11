#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <ctime>
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

std::string current_time_date()
{
    std::string time_date;

    time_t timey = time(0);
    tm* timestruct = localtime(&timey);
    
    time_date= ('_') + std::to_string(1900 + timestruct->tm_year)+('_')+std::to_string(1 + timestruct->tm_mon)+("__")+std::to_string(timestruct->tm_mday)+('_')+std::to_string(timestruct->tm_hour)+('_')+std::to_string(timestruct->tm_min)+('_')+std::to_string(timestruct->tm_sec);
    std::cout << time_date << "\n";
    return time_date;
}

std::string illegalcharcheck(std::string stringtocheck, bool ispath)
{
    char space_check = 0;
    std::string checkedpath;
    int pos2;

    if(ispath == true) pos2 = (stringtocheck.find("..."));
    else pos2 = (stringtocheck.find('\n'));

    // trying to get rid of spaces before the path in the input file
    for(int ws_index=5; ws_index<stringtocheck.length()-1; ++ws_index)
    {
        std::cout << "Path = " << stringtocheck[ws_index] << "\n";

        //this is to get rid of leading whitespace
        if(space_check == 0)
        {
            if(int(stringtocheck[ws_index]) != 0 && int(stringtocheck[ws_index]) != 32)
            {
                std::cout << "pangus\n";
                checkedpath = stringtocheck.substr(ws_index,(pos2-ws_index));
                space_check = 1;
            }
        }

        switch(int(stringtocheck[ws_index]))
        {
            case 92: 
            {
                if(ispath == false) err_num(3); // '\'
                break;
            }
            case 47: 
            {
                if(ispath == false) err_num(3); // '/'
                break;
            }
            case 42: err_num(3);
            case 58: err_num(3);
            case 63: err_num(3);
            case 34: err_num(3);
            case 60: err_num(3);
            case 62: err_num(3);
            case 124: err_num(3);
        }
    }

    return checkedpath;
}

void create_initial_JSON(std::string input_txt, std::string importantJSON, std::string importantJSON_direc)
{
    char inout = 0b000;
    char output_info = 0; 
    //std::string importantJSON;
    //std::string importantJSON_direc;
    std::string datetime = current_time_date();

    std::cout << datetime << "\n";

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
                        importantJSON_direc = illegalcharcheck(current_line, true);

                        importantJSON_direc.append(datetime);
                        std::cout << importantJSON_direc << "\n";

                        if(!fs::exists(importantJSON_direc)) 
                        {
                            std::cout << "It not dere\n";
                            fs::create_directories(importantJSON_direc); // I could make this in C but it would need to be a seperate file

                            // if system cannot create the directories then provide an error and exit
                                // important*** do not need to check for illegal characters due to the fact that if the dirs cant be created then an error will result
                        }

                        fs::current_path(importantJSON_direc);
                        std::cout << "Current path = " << fs::current_path() << "\n";
                        
                        // if system cannot access current path then provide an error and exit
                        output_info = 1;
                    }

                    // else the file is improperly formatted raise an error

                    
                }

                else if((current_line.find("NAME:") != std::string::npos)&&(output_info == 1))
                {
                    std::cout << "File name = " << current_line << "\n";
                    importantJSON = illegalcharcheck(current_line, false);

                    importantJSON.append(datetime);
                    importantJSON.append(".json");
                    std::ofstream JSONFILE(importantJSON);
                    inout = 0b010;
                }

                // else the file is improperly formatted raise an error

                break;
            }

            case 2:
            {
                if(current_line.find("input") != std::string::npos) inout = 0b011;
                break;
            }

            case 3:
            {

            }

            default:
            {
                // need an error here
                break;
            }
        }
    }
}


int main(int argc, char **argv)
{
    std::string importantJSON;
    std::string importantJSON_direc;
    std::string inputarg;

    try
    {
        if(argc > 1)
        {
            if((fs::exists(argv[1]))&&(strcmp(fs::path(argv[1]).extension().c_str(),".txt") == 0)) //if a file is specified and it exists in the specific path then grab the path and store it
            {
                std::cout << "The specified file exists" << fs::absolute(argv[1]) << '\n';
                create_initial_JSON(fs::absolute(argv[1]), importantJSON, importantJSON_direc);
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

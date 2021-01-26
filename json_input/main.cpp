#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;

std::string date_time;

namespace json_cmd
{
    struct infile_out_dir
    {
        std::string path;
        std::string name;
    };

    void from_json(const json &j, infile_out_dir &out)
    {
        j.at("path").get_to(out.path);
        j.at("name").get_to(out.name);
    }

    struct infile_in_data
    {
        std::string type;
        std::string path;
    };

    void from_json(const json &j, infile_in_data &in)
    {
        j.at("type").get_to(in.type);
        j.at("path").get_to(in.path);
    }

    struct outfile_data
    {
        std::string path;
        std::string metadata; // I have no idea of how to get metadata....
        std::string datetime;
        std::vector<json_cmd::outfile_data> inside;
    };
}

void space_avaliable(std::string inputpath, uintmax_t &space)
{
    std::string inputpath_substr;

    for (int path_index = inputpath.length() - 1; path_index >= 0; --path_index)
    {
        // 47 should be "/"
        if (inputpath[path_index] == 47)
        {
            inputpath_substr = inputpath.substr(0, path_index);
            std::cout << "inputpath_substr = " << inputpath_substr << endl;
            if (fs::exists(inputpath_substr))
            {
                fs::space_info dev_space = fs::space(inputpath_substr);
                //std::cout << "Space available for current path = " << dev_space.free << "\n";
                space = dev_space.free;
                std::cout << "Space available for current path = " << space << "\n";
                break;
            }
        }
    }
}

std::string current_time_date()
{
    std::string time_date;

    time_t timey = time(0);
    tm* timestruct = localtime(&timey);
    
    time_date=std::to_string(1900 + timestruct->tm_year)+('_')+std::to_string(1 + timestruct->tm_mon)+('_')+std::to_string(timestruct->tm_mday)+("__")+std::to_string(timestruct->tm_hour)+('_')+std::to_string(timestruct->tm_min)+('_')+std::to_string(timestruct->tm_sec);
    std::cout << time_date << "\n";
    return time_date;
}

void JSON_substructure(json_cmd::outfile_data &sub_structure)
{

}

json_cmd::outfile_data createJSON_structure(json_cmd::infile_in_data in_data)
{
    json_cmd::outfile_data out_data;

    if(fs::exists(in_data.path))
    {
        out_data.path = in_data.path;
        //out_data.metadata = ?
        out_data.datetime = date_time;

        if(fs::is_directory(in_data.path))
        {
            if(strcmp(in_data.type.c_str(),"fd")==0)
            {
                for(auto p: fs::recursive_directory_iterator(in_data.path))
                {
                    out_data.inside.push_back(json_cmd::outfile_data());

                    if(fs::is_directory(in_data.path))
                    {
                        
                    }

                    else
                    {
                        
                    }
                    
                    std::cout << p.path() << '\n';
                }
            }

            else if(strcmp(in_data.type.c_str(),"sd")==0)
            {
                for(auto p: fs::directory_iterator(in_data.path))
                {
                    out_data.inside.push_back(json_cmd::outfile_data());

                    std::cout << p.path() << '\n';
                }
            }
        }

        else
        {
            
            std::cout << in_data.path << '\n';
        }
    
        return out_data;
    }

    else
    {
        return -1; // problem?
    }
}

int main(int argc, char **argv)
{
    if (argc > 0)
    {
        date_time = current_time_date();

        std::string originaljson = fs::absolute(argv[1]);

        std::ifstream inputjson(fs::absolute(argv[1]));

        json jsonobject;
        inputjson >> jsonobject;

        json_cmd::infile_out_dir infile_out_dir = jsonobject["output"].get<json_cmd::infile_out_dir>();

        json_cmd::infile_in_data infile_in;
        std::vector<json_cmd::outfile_data> JSON_structures;
        //Alternative to a linked list

        for(int index=0; index<jsonobject["input"].size(); ++index)
        {
            infile_in = jsonobject["input"][index].get<json_cmd::infile_in_data>();
            //std::cout << "infile_in[index] = " << infile_in[index].path << "\n";

            JSON_structures.push_back(json_cmd::outfile_data());
            JSON_structures[index] = createJSON_structure(infile_in);
            //ref:https://stackoverflow.com/questions/8067338/vector-of-structs-initialization/8067443
        }

        std::cout << "output_cls = " << infile_out_dir.path << "\n";
        //std::cout << "input_cls = " << infile_in[0].path << "\n";

        uintmax_t space_ready;
        space_avaliable(infile_out_dir.path, space_ready);

        std::cout << "space avaliable = " << space_ready << endl;

        //uintmax_t space_needed;
        //space_required(std::string inputpath, uintmax_t &space) ;
    }
}
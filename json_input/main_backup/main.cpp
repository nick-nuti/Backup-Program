#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;

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
        std::string metadata;
        std::string datetime;
        std::vector<std::string> inside;
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

void space_required(std::string inputpath, uintmax_t &space)
{
}

int main(int argc, char **argv)
{
    if (argc > 0)
    {
        std::string originaljson = fs::absolute(argv[1]);

        std::ifstream inputjson(fs::absolute(argv[1]));

        json jsonobject;
        inputjson >> jsonobject;

        json_cmd::infile_out_dir infile_out_dir = jsonobject["output"].get<json_cmd::infile_out_dir>();

        std::vector<json_cmd::infile_in_data> infile_in;
        //Alternative to a linked list
        for(int index=0; index<jsonobject["input"].size(); ++index)
        {
            infile_in.push_back(json_cmd::infile_in_data());
            infile_in[index] = jsonobject["input"][index].get<json_cmd::infile_in_data>();
            //ref:https://stackoverflow.com/questions/8067338/vector-of-structs-initialization/8067443
            std::cout << "infile_in[index] = " << infile_in[index] << "\n";
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
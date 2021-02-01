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
        uintmax_t filesize;
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

void JSON_substructure(std::string dir_path, json_cmd::outfile_data *sub_structure, bool recursive, uintmax_t &dir_size_acc)
{
    json_cmd::outfile_data struct_holder;
    uintmax_t dir_size = dir_size_acc;
    std::cout << "SPACE REQUIRED= " << dir_size_acc << "\n";

    if(recursive == true)
    {

        for(auto pathy: fs::directory_iterator(dir_path))
        {
            if(fs::is_directory(pathy))
            {
                //std::cout << std::string(pathy.path()) << "\n";
                struct_holder = (json_cmd::outfile_data){.path = pathy.path()};

                JSON_substructure(std::string(pathy.path()), &struct_holder, true, dir_size);

                sub_structure->inside.push_back(struct_holder);
            }

            else
            {
                struct_holder = (json_cmd::outfile_data){.path = pathy.path().filename(), .filesize = fs::file_size(pathy.path())};
                
                dir_size += fs::file_size(pathy.path());
                sub_structure->inside.push_back(struct_holder);
            }
        }
    }

    else
    {
        for(auto pathy: fs::directory_iterator(dir_path))
        {
            if(!(fs::is_directory(pathy)))
            {
                struct_holder = (json_cmd::outfile_data){.path = pathy.path().filename(), .filesize = fs::file_size(pathy.path())};

                sub_structure->inside.push_back(struct_holder);
            }
            
            //std::cout << pathy.path() << '\n';
        }
    }
}

json_cmd::outfile_data createJSON_structure(json_cmd::infile_in_data in_data, uintmax_t &dir_size_acc)
{
    json_cmd::outfile_data out_data;
    bool recursive;

    //std::cout << fs::path(in_data.path) << '\n';
    std::string checkpath = fs::path(in_data.path);

    if(fs::exists(checkpath))
    {
        out_data = (json_cmd::outfile_data){.path = fs::path(in_data.path)};

        if(fs::is_directory(checkpath))
        {
            if(strcmp(in_data.type.c_str(),"fd")==0) recursive = true;
            else if(strcmp(in_data.type.c_str(),"sd")==0) recursive = false;
            //else raise an error

            JSON_substructure(in_data.path, &out_data, recursive, dir_size_acc);
        }

        //else
        //{
            //out_data = (json_cmd::outfile_data){.path = fs::path(in_data.path).filename()}; // doesnt make much sense to only include the filename... need the high level path
        //    out_data = (json_cmd::outfile_data){.path = fs::path(in_data.path)};
            //std::cout << in_data.path << '\n';
        //}
    
        return out_data;
    }

    else
    {
        std::cout << "apparently there is no dir?....";
        return out_data; // return problem
    }
}

void printtree(json_cmd::outfile_data *JSONTREE, bool firsttime, int padding = 0)
{
    std::string keep_path;

    if(firsttime) std::cout << JSONTREE->path << '\n';

    for(int index = 0; index < JSONTREE->inside.size(); ++index)
    {
        //std::cout << JSONTREE->inside[index].path << '\n';

        if(JSONTREE->inside[index].inside.size()>0)
        {
            std::cout << std::string(padding, ' ') << JSONTREE->inside[index].path << '\n';
            printtree(&JSONTREE->inside[index],false, ++padding);
            --padding;
        }

        else
        {
            std::cout << std::string(padding, ' ') << JSONTREE->inside[index].path << '\n';
        } 
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

        uintmax_t space_required;
        
        //Alternative to a linked list

        for(int index=0; index<jsonobject["input"].size(); ++index)
        {
            infile_in = jsonobject["input"][index].get<json_cmd::infile_in_data>();
            //std::cout << "infile_in[index] = " << infile_in[index].path << "\n";

            JSON_structures.push_back(json_cmd::outfile_data());
            JSON_structures[index] = createJSON_structure(infile_in, space_required);
            //ref:https://stackoverflow.com/questions/8067338/vector-of-structs-initialization/8067443

            //std::cout << infile_in.path << '\n';
        }

        //std::cout << "SPACE REQUIRED= " << space_required << "\n";

        for(int index = 0; index < JSON_structures.size(); ++index)
        {
            std::cout << "\n\n";
            printtree(&JSON_structures[index], true);
        }
    
        std::cout << "output_cls = " << infile_out_dir.path << "\n";
        //std::cout << "input_cls = " << infile_in[0].path << "\n";

        uintmax_t space_ready;
        space_avaliable(infile_out_dir.path, space_ready);

        std::cout << "space avaliable = " << space_ready << endl;

    }
}
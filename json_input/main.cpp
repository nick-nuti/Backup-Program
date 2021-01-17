#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;

class json_cmd
{
    public:
        struct output_json
        {
            std::string path;
            std::string name;
        };

        void from_json(const json& j, output_json& o)
        {
            j.at("path").get_to(o.path);
            j.at("name").get_to(o.name);
        }

        /*void read_json_in_info()
        {

        }*/
};

void space_avaliable(std::string inputpath, uintmax_t &space) 
{
    std::string inputpath_substr;

    for(int path_index=inputpath.length()-1; path_index>=0; --path_index)
    {
        // 47 should be "/"
        if(inputpath[path_index] == 47) 
        {
            inputpath_substr = inputpath.substr(0,path_index);
            std::cout << "inputpath_substr = " << inputpath_substr << endl;
            if(fs::exists(inputpath_substr))
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

int main(int argc, char** argv)
{
    /*
    if(argc > 0)
    {
        std::string originaljson = fs::absolute(argv[1]);
        std::string backupcopy_json = originaljson.insert(originaljson.length()-6,"_backup");
        //std::ofstream inputjson(backupcopy_json);
        std::ifstream inputjson(fs::absolute(argv[1]));

        json ij;
        inputjson >> ij;

        std::cout << "ij = " << ij << endl;

        json_cmd::output_json output_cls = ij["output"].from_json<json_cmd::output_json>();

        //std::cout << "path = " << ij["output"]["path"] << endl;
        //std::cout << "name = " << ij["output"]["name"] << endl;

        uintmax_t space_ready;
        space_avaliable(ij["output"]["path"],space_ready);

        std::cout << "space avaliable = " << space_ready << endl;

        //uintmax_t space_needed;
        //space_required(std::string inputpath, uintmax_t &space) ;
    }
    */

   // store a string in a JSON value
    json j_string = "this is a string";

    // retrieve the string value
    auto cpp_string = j_string.get<std::string>();
    // retrieve the string value (alternative when an variable already exists)
    std::string cpp_string2;
    j_string.get_to(cpp_string2);

    // retrieve the serialized value (explicit JSON serialization)
    std::string serialized_string = j_string.dump();

    // output of original string
    std::cout << cpp_string << " == " << cpp_string2 << " == " << j_string.get<std::string>() << '\n';
    // output of serialized value
    std::cout << j_string << " == " << serialized_string << std::endl;
}
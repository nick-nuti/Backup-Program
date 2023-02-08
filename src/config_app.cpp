#include "config_app.hpp"

#ifdef PLATFORM_WIN
    #include "win_app.hpp"
#elif PLATFORM_LIN
    #include "lin_app.hpp"
#elif PLATFORM_OSX
    #include "osx_app.hpp"
#endif

namespace fs = std::filesystem;

void config_app::os_config()
{
    if(!fs::exists(config))
    {
        if(checkadmin() == false) std::cout << "Please re-run as root" << std::endl;
        else
        {
            // create directory in /var/lib/ if it doesn't exist
            fs::create_directory(config_location);

            // creates files /var/lib/config.json
            if(FILE * fp = fopen(config.c_str(),"w+"))
            {
                fputs("{\"Backups\":[]}", fp);
                fclose(fp);
            }

            else throw std::runtime_error("Error creating config JSON.");
        }
    }
}

std::string config_app::config_digest() // lets user reconfig logged backup frequency or make new backup right now
{
    std::string selected_backup = "\0";

    //need to build out UI function for selecting backup

    return selected_backup;
}

void config_app::update_config(const std::string pathtobackup, const int backupfreq)
{
    if(!fs::exists(config))
    {
        std::cout << "Cannot find config.json, creating..." << std::endl;
        config_app::os_config();
    }

    if(FILE * configread = fopen(config.c_str(),"r"))
    {
        char readbuff [65536];
        rapidjson::FileReadStream rs(configread, readbuff, sizeof(readbuff));
        rapidjson::Document readconfig_doc;
        readconfig_doc.ParseStream(rs);

        rapidjson::Value& config_arr = readconfig_doc["Backups"];

        fclose(configread);

        rapidjson::Value objValue(rapidjson::kObjectType);

        const std::string pathto_currbackup = pathtobackup.substr(0,pathtobackup.find_last_of('-'));
        //std::cout << readconfig_doc["Backups"][0]["path"].GetString() << std::endl;

        std::string pathtobackup_json, pathtobackup_dir;
        int index = 0;

        for(rapidjson::SizeType i = 0; i < config_arr.Size(); ++i, ++index)
        {
            pathtobackup_json = readconfig_doc["Backups"][i]["path"].GetString();
            pathtobackup_dir = pathtobackup_json.substr(0,pathtobackup_json.find_last_of('-'));

            if(strcmp(pathto_currbackup.c_str(), pathtobackup_dir.c_str()) == 0)
            {
                // found backup, need to modify
                readconfig_doc["Backups"][i]["path"].SetString(pathtobackup.data(), pathtobackup.size(), readconfig_doc.GetAllocator());
                break;
            }
        }

        if(index == config_arr.Size())
        {
            rapidjson::Value ptb(pathtobackup.c_str(), readconfig_doc.GetAllocator());
            objValue.AddMember("path", ptb, readconfig_doc.GetAllocator());
            
            //rapidjson::Value bf(pathtobackup.c_str(), readconfig_doc.GetAllocator());
            objValue.AddMember("freq", backupfreq, readconfig_doc.GetAllocator());

            config_arr.PushBack(objValue, readconfig_doc.GetAllocator());
        }

        FILE* configupdate = fopen(config.c_str(), "w");
        char writebuff [4096];
        rapidjson::FileWriteStream ws(configupdate, writebuff, sizeof(writebuff));
        rapidjson::Writer<rapidjson::FileWriteStream> writer(ws);
        readconfig_doc.Accept(writer);
        fclose(configupdate);
    }
    
    else throw std::runtime_error("Error creating config JSON.");
}
#include "filecopy.hpp"

#include <pthread.h>

namespace fs = std::filesystem;

void recursive_arrcopy(rapidjson::Value::ConstMemberIterator itr1, const int sourceitem_type, const std::string sourceitem_path, const std::string sub_dest_path)
{
    std::string next_path;
    std::string source_path;

    for (rapidjson::Value::ConstValueIterator itr2 = itr1->value.Begin(); itr2 != itr1->value.End(); ++itr2) // arrays
    {
        for (rapidjson::Value::ConstMemberIterator itr3 = itr2->MemberBegin(); itr3 != itr2->MemberEnd(); ++itr3) // objects
        {
            if(strcmp(itr3->name.GetString(),"path") == 0)
            {
                next_path = sub_dest_path + '/' + itr3->value.GetString();
                source_path = sourceitem_path + '/' + itr3->value.GetString();

                if(fs::is_regular_file(source_path)) 
                {
                    fs::copy(source_path, next_path);
                }

                else if((sourceitem_type == 0) && (fs::is_directory(source_path)))
                {
                    fs::create_directory(next_path);
                }
            }

            else if(itr3->value.IsArray() && sourceitem_type != 1)
            {
                recursive_arrcopy(itr3, 0, source_path, next_path);
            }
        }
    }
}

void filecopy(BackupParams backupParams, std::string json_location, std::string backupDir_fullpath)
{
    FILE* fp = fopen(json_location.c_str(), "r");

    char readBuffer[4096];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document document;
    document.ParseStream(is);

    // object -> array -> object -> array -> recursive -> object -> array....
    int srcparam_iter = 0;

    for (rapidjson::Value::ConstValueIterator itr = document["files"].Begin(); itr != document["files"].End(); ++itr, ++srcparam_iter) // arrays
    {
        const std::string sourceitem_path = backupParams.sourceParams[srcparam_iter].path;
        const int sourceitem_type = backupParams.sourceParams[srcparam_iter].flag;

        for (rapidjson::Value::ConstMemberIterator itr1 = itr->MemberBegin(); itr1 != itr->MemberEnd(); ++itr1) // objects
        {
            const std::string full_dest = backupDir_fullpath + '/' + sourceitem_path.substr(sourceitem_path.find_last_of("/")+1);

            if(strcmp(itr1->name.GetString(),"path") == 0)
            {
                if(sourceitem_type == 2) fs::copy(sourceitem_path, full_dest);
                
                else fs::create_directory(full_dest);
            }

            else if(itr1->value.IsArray())
            {
                recursive_arrcopy(itr1, sourceitem_type, sourceitem_path, full_dest);
            }

           else continue;
        }
    }

    fclose(fp);
}

#include <filesystem>
#include <iostream>
#include <stdio.h>

#include "rapidjson\document.h"
#include "rapidjson\filereadstream.h"
 
using namespace std;
using namespace rapidjson;
namespace fs = std::filesystem;

void recursive_arrcopy(Value::ConstMemberIterator itr1)
{
    for (Value::ConstValueIterator itr2 = itr1->value.Begin(); itr2 != itr1->value.End(); ++itr2) // arrays
    {
        for (Value::ConstMemberIterator itr3 = itr2->MemberBegin(); itr3 != itr2->MemberEnd(); ++itr3) // objects
        {
            if(itr3->value.IsString()) printf("STRING Member %s is %s\n", itr3->name.GetString(), itr3->value.GetString());
            else if(itr3->value.IsInt64()) printf("NUMBER Member %s is %d\n", itr3->name.GetString(), itr3->value.GetInt());
            else if(itr3->value.IsBool()) printf("BOOL Member %s is %s\n", itr3->name.GetString(), itr3->value.GetBool() ? "True" : "False");
            else if(itr3->value.IsArray())
            {
                printf("Array Member %s\n", itr1->name.GetString());
                recursive_arrcopy(itr3);
            }
        }
    }
}

void filecopy(string jsonname)
{
    FILE* fp = fopen(jsonname.c_str(), "r");

    char readBuffer[4096];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    Document document;
    document.ParseStream(is);

    fs::create_directory("backup");
    fs::current_path("backup");

    // object -> array -> object -> array -> recursive -> object -> array....

    for (Value::ConstValueIterator itr = document["files"].Begin(); itr != document["files"].End(); ++itr) // arrays
    {
        for (Value::ConstMemberIterator itr1 = itr->MemberBegin(); itr1 != itr->MemberEnd(); ++itr1) // objects
        {
            //if(itr1->value.IsString()) printf("STRING Member %s is %s\n", itr1->name.GetString(), itr1->value.GetString());
            //else if(itr1->value.IsInt64()) printf("NUMBER Member %s is %d\n", itr1->name.GetString(), itr1->value.GetInt());
            //else if(itr1->value.IsBool()) printf("BOOL Member %s is %s\n", itr1->name.GetString(), itr1->value.GetBool() ? "True" : "False");
            /*else if(itr1->value.IsArray())
            {
                printf("Array Member %s\n", itr1->name.GetString());

                recursive_arrcopy(itr1);
            }
            */
           
           //if(itr1->value.IsString())
           if(!strcmp(itr1->name.GetString(),"path"))
           {
                //printf("STRING Member %s is %s\n", itr1->name.GetString(), itr1->value.GetString());
                fs::path path = fs::current_path();
                string backupdirpath = path.string();
                replace(backupdirpath.begin(), backupdirpath.end(), '\\', '/');

                if(backupdirpath.back() != '/') backupdirpath +='/';

                string destination = "";

                string str_store = itr1->value.GetString();

                if(fs::is_regular_file(str_store))
                {
                    destination = fs::path(str_store).filename().string();
                    cout << fs::path(str_store).filename() << endl;
                    
                }

                else if(fs::is_directory(str_store))
                {
                    destination = fs::path(str_store).string();

                    int backslash;
                    (destination.back() == '/') ? backslash=2 : backslash=1;

                    for(int index = destination.size() - backslash; index >= 0; --index)
                    {
                        if(destination[index] == '/')
                        {
                            destination = destination.substr(index);
                            cout << destination << endl;
                            break;
                        }

                        else continue;
                    }
                }
                
                fs::copy(itr1->value.GetString(), backupdirpath+destination,fs::copy_options::recursive);
           }

           else continue;
        }
    }

    fclose(fp);
}

#include <iostream>
#include <filesystem>
#include <map>
#include <fstream>
#include <ctime>
#include <regex>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/prettywriter.h>
#include "rapidjson/filewritestream.h"

#include "tree.hpp"

using namespace rapidjson;
using namespace std;
using namespace filesystem;

map<string, int> flagMap {{"f", 2}, {"sd", 1}, {"fd", 0}};

BackupParams generateBackupParams(string path)
{
    ifstream ifs(path);
    IStreamWrapper isw(ifs);

    Document inputJSON;
    inputJSON.ParseStream(isw);

    vector<SourceParam> sourceParams;
    int i = 0;
    int len = inputJSON["input"].Size();
    for(; i < len; i++)
    {
        string path = inputJSON["input"][i]["path"].GetString();
        string flag = inputJSON["input"][i]["type"].GetString();

        SourceParam sourceParam {
            .path = path,
            .flag = flagMap[flag]
        };
        sourceParams.push_back(sourceParam);
    }

    time_t timestamp = time(nullptr);
    string backupLabel(inputJSON["output"]["name"].GetString());
    string outputPath(inputJSON["output"]["path"].GetString());

    outputPath += ("/" + backupLabel + "-" + to_string(timestamp) + ".json"); 

    BackupParams b {
        .backupLabel = backupLabel,
        .outputPath = outputPath,
        .sourceParams = sourceParams
    };

    return b;
}

int64_t traverseTree(Writer<FileWriteStream> &writer, const SourceParam &s)
{
    struct stat st;
    stat(s.path.c_str(), &st);

    if(!S_ISREG(st.st_mode) && !(S_ISDIR(st.st_mode)))
    {
        // Not a normal file, nor a directory. GTFO!
        return 0;
    }

    writer.StartObject();
    
    // DECORATE
    writer.Key("path");
    writer.String(s.path.c_str());

    writer.Key("timeLastWritten");
    writer.Int64(st.st_mtime);

    uintmax_t sizeAcc = st.st_size ? st.st_size : 0;

    if ((s.flag == flagMap["fd"] || s.flag == flagMap["sd"]) && S_ISDIR(st.st_mode))
    {
        writer.Key("contents");
        writer.StartArray();

        try {
            for (directory_entry entry : directory_iterator(s.path))
            {
                // replaces path back slashes with forward slashes
                string path = regex_replace(entry.path().string(), regex("\\\\+"), "/");

                SourceParam sourceParam {
                    .path = path,
                    .flag = (s.flag == flagMap["fd"] ? flagMap["fd"] : flagMap["f"])
                };

                sizeAcc += traverseTree(writer, sourceParam);
            }
        } catch (exception& e) {
            cout << "Unable to open " << s.path.c_str() << endl;
            cout << e.what() << endl;
            cout << "continuing..." << endl << endl;
        }

        writer.EndArray();
    }

    writer.Key("size");
    writer.Int64(sizeAcc);
    // END-DECORATE
    
    writer.EndObject();

    return sizeAcc;
} 

void buildDigest(BackupParams &backupParams)
{
    cout << "Writing digest to: " << backupParams.outputPath << endl;

    if(FILE* fp = fopen(backupParams.outputPath.c_str(), "w+"))
    {
        char writeBuf[65536];
        FileWriteStream outStream(fp, writeBuf, sizeof(writeBuf));
        Writer<FileWriteStream> fsWriter(outStream);

        fsWriter.StartObject();
        fsWriter.Key("name");
        fsWriter.String(backupParams.backupLabel.c_str());
        fsWriter.Key("files");
        fsWriter.StartArray();

        for (const SourceParam& s : backupParams.sourceParams)
        {
            traverseTree(fsWriter, s);
        }

        fsWriter.EndArray();
        fsWriter.EndObject();
        
    } else {
        throw runtime_error("Error opening digest file.");
    }
}
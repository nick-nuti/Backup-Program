<<<<<<< HEAD
// possibly....
// clang++ -std=c++17 ../src/*.cpp -o main.exe -I"C:\Program Files\OpenSSL-Win64\include" -L"C:\Program Files\OpenSSL-Win64\lib" -lssl -lcrypto -lstdc++fs
=======
>>>>>>> ee33bb0c3328ae86ea9367ff5110360d210a1207
#include <iostream>
#include <filesystem>
#include <map>
#include <fstream>
#include <ctime>
#include <regex>
<<<<<<< HEAD
#include <stdio.h>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/filewritestream.h"

#include "tree.hpp"
#include "sha256_hash.hpp"
=======

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/prettywriter.h>
#include "rapidjson/filewritestream.h"

#include "tree.hpp"
>>>>>>> ee33bb0c3328ae86ea9367ff5110360d210a1207

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
<<<<<<< HEAD
    for(int i = 0; i < inputJSON["input"].Size(); i++)
=======
    int i = 0;
    int len = inputJSON["input"].Size();
    for(; i < len; i++)
>>>>>>> ee33bb0c3328ae86ea9367ff5110360d210a1207
    {
        string path = inputJSON["input"][i]["path"].GetString();
        string flag = inputJSON["input"][i]["type"].GetString();

        SourceParam sourceParam {
            .path = path,
            .flag = flagMap[flag]
        };
        sourceParams.push_back(sourceParam);
    }

<<<<<<< HEAD
    string backupLabel(inputJSON["output"]["name"].GetString());
    string outputPath(inputJSON["output"]["path"].GetString());

    //add a slash (if one wasn't entered) at the end of outpath
    if(outputPath.back() != '/') outputPath += '/'; 

    // timestamp generated here!
    string ts = to_string(time(nullptr));
=======
    time_t timestamp = time(nullptr);
    string backupLabel(inputJSON["output"]["name"].GetString());
    string outputPath(inputJSON["output"]["path"].GetString());

    outputPath += ("/" + backupLabel + "-" + to_string(timestamp) + ".json"); 
>>>>>>> ee33bb0c3328ae86ea9367ff5110360d210a1207

    BackupParams b {
        .backupLabel = backupLabel,
        .outputPath = outputPath,
<<<<<<< HEAD
        .ts = ts,
=======
>>>>>>> ee33bb0c3328ae86ea9367ff5110360d210a1207
        .sourceParams = sourceParams
    };

    return b;
}

<<<<<<< HEAD
uintmax_t traverseTree(Writer<FileWriteStream> &writer, const SourceParam &s, vector<string> &hashes)
{
    // get inode information
=======
int64_t traverseTree(Writer<FileWriteStream> &writer, const SourceParam &s)
{
>>>>>>> ee33bb0c3328ae86ea9367ff5110360d210a1207
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
<<<<<<< HEAD
    size_t lastslash = s.path.find_last_of("/"); 
    writer.String(s.path.substr(lastslash+1).c_str()); 
    //writer.String(s.path.c_str());
=======
    writer.String(s.path.c_str());
>>>>>>> ee33bb0c3328ae86ea9367ff5110360d210a1207

    writer.Key("timeLastWritten");
    writer.Int64(st.st_mtime);

    uintmax_t sizeAcc = st.st_size ? st.st_size : 0;
<<<<<<< HEAD
    char hash[65];
=======
>>>>>>> ee33bb0c3328ae86ea9367ff5110360d210a1207

    if ((s.flag == flagMap["fd"] || s.flag == flagMap["sd"]) && S_ISDIR(st.st_mode))
    {
        writer.Key("contents");
        writer.StartArray();

<<<<<<< HEAD
        vector<string> subdirectory_hashes;

=======
>>>>>>> ee33bb0c3328ae86ea9367ff5110360d210a1207
        try {
            for (directory_entry entry : directory_iterator(s.path))
            {
                // replaces path back slashes with forward slashes
                string path = regex_replace(entry.path().string(), regex("\\\\+"), "/");

                SourceParam sourceParam {
                    .path = path,
                    .flag = (s.flag == flagMap["fd"] ? flagMap["fd"] : flagMap["f"])
                };

<<<<<<< HEAD
                // accumulate size of children
                if((s.flag == flagMap["sd"]) && (!is_directory(entry))) sizeAcc += traverseTree(writer, sourceParam, subdirectory_hashes); 
                else continue; 
=======
                sizeAcc += traverseTree(writer, sourceParam);
>>>>>>> ee33bb0c3328ae86ea9367ff5110360d210a1207
            }
        } catch (exception& e) {
            cout << "Unable to open " << s.path.c_str() << endl;
            cout << e.what() << endl;
            cout << "continuing..." << endl << endl;
        }

        writer.EndArray();
<<<<<<< HEAD
        
        sha256_dir(subdirectory_hashes, false, hash);

        //delete the contents of "subdirectory_hashes"
        subdirectory_hashes.clear();

        //add the hash to the "hashes" vector
        hashes.push_back(hash);

        //create a writer entry for the directory hash
        writer.Key("hash");
        writer.String(hash);
    }

    else // this is a file
    {
        sha256_file(s.path, hash);

        writer.Key("hash");
        writer.String(hash);
        
        hashes.push_back(hash); //for the overall hash
=======
>>>>>>> ee33bb0c3328ae86ea9367ff5110360d210a1207
    }

    writer.Key("size");
    writer.Int64(sizeAcc);
    // END-DECORATE
    
    writer.EndObject();

    return sizeAcc;
} 

<<<<<<< HEAD
void buildDigest(BackupParams &backupParams, string &backupDigestLocation)
{
    // individal backup (IB) directory label
    string backupTimestampedLabel = backupParams.backupLabel + "-" + backupParams.ts;

    // path to IB directory
    string backupDir = backupParams.outputPath + backupParams.backupLabel + '/' + backupTimestampedLabel;

    // make the directory ^
    create_directory(backupDir);

    // path to IB digest file
    backupDigestLocation = backupDir + "/" + backupTimestampedLabel + ".json";
    cout << "Writing digest to: " << backupDigestLocation << endl;

    if(FILE* fp = fopen(backupDigestLocation.c_str(), "w+"))
=======
void buildDigest(BackupParams &backupParams)
{
    cout << "Writing digest to: " << backupParams.outputPath << endl;

    if(FILE* fp = fopen(backupParams.outputPath.c_str(), "w+"))
>>>>>>> ee33bb0c3328ae86ea9367ff5110360d210a1207
    {
        char writeBuf[65536];
        FileWriteStream outStream(fp, writeBuf, sizeof(writeBuf));
        Writer<FileWriteStream> fsWriter(outStream);

        fsWriter.StartObject();
        fsWriter.Key("name");
        fsWriter.String(backupParams.backupLabel.c_str());
        fsWriter.Key("files");
        fsWriter.StartArray();

<<<<<<< HEAD
        vector<string> hashes;
        uintmax_t totalbackupsize = 0;

        for (const SourceParam& s : backupParams.sourceParams)
        {
            totalbackupsize += traverseTree(fsWriter, s, hashes);
        }

        fsWriter.EndArray();
        
        char totalbackuphash [65];
        sha256_dir(hashes, true, totalbackuphash);

        fsWriter.Key("hash");
        fsWriter.String(totalbackuphash);

        fsWriter.Key("size");
        fsWriter.Int64(totalbackupsize);
        
        fsWriter.EndObject();

        fclose(fp);
    }

    else throw runtime_error("Error opening digest file.");
=======
        for (const SourceParam& s : backupParams.sourceParams)
        {
            traverseTree(fsWriter, s);
        }

        fsWriter.EndArray();
        fsWriter.EndObject();
        
    } else {
        throw runtime_error("Error opening digest file.");
    }
>>>>>>> ee33bb0c3328ae86ea9367ff5110360d210a1207
}
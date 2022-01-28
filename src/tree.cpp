// possibly....
// clang++ -std=c++17 ../src/*.cpp -o main.exe -I"C:\Program Files\OpenSSL-Win64\include" -L"C:\Program Files\OpenSSL-Win64\lib" -lssl -lcrypto -lstdc++fs

#include "tree.hpp"

//using namespace rapidjson;
//using namespace std;
namespace fs = std::filesystem;

std::map<std::string, int> flagMap {{"f", 2}, {"sd", 1}, {"fd", 0}};

BackupParams generateBackupParams(std::string path)
{
    std::ifstream ifs(path);
    rapidjson::IStreamWrapper isw(ifs);

    rapidjson::Document inputJSON;
    inputJSON.ParseStream(isw);

    std::vector<SourceParam> sourceParams;
    for(int i = 0; i < inputJSON["input"].Size(); i++)
    {
        std::string path = inputJSON["input"][i]["path"].GetString();
        std::string flag = inputJSON["input"][i]["type"].GetString();

        SourceParam sourceParam {
            .path = path,
            .flag = flagMap[flag]
        };

        // need this for error checking
        if((sourceParam.flag == 2) && (fs::is_regular_file(sourceParam.path) == 0))
        {
            std::cout << "Input JSON Entry " << sourceParam.path << " was declared backup type 'f' and is not a regular file. Exiting..." << std::endl;
            // need to create an error and force exit from here
        }

        else if(((sourceParam.flag == 1) || (sourceParam.flag == 0)) && (fs::is_directory(sourceParam.path) == 0))
        {
            std::cout << "Input JSON Entry " << sourceParam.path << " was declared backup type 'sd' or 'fd' and is not a directory. Exiting..." << std::endl;
            // need to create an error and force exit from here
        }

        sourceParams.push_back(sourceParam);
    }

    std::string backupLabel(inputJSON["output"]["name"].GetString());
    std::string outputPath(inputJSON["output"]["path"].GetString());
    
    std::string backupfrequency = inputJSON["output"]["frequency"].GetString();

    int frequency_converted, freq_days, freq_hours, freq_minutes;

    std::size_t first_colon = backupfrequency.find_first_of(':');
    std::size_t last_colon = backupfrequency.find_last_of(':');

    freq_days = stoi(backupfrequency.substr(0,first_colon))*86400;
    freq_hours = stoi(backupfrequency.substr(first_colon+1,last_colon))*3600;
    freq_minutes = stoi(backupfrequency.substr(last_colon+1))*60;

    frequency_converted = freq_days + freq_hours + freq_minutes;

    //add a slash (if one wasn't entered) at the end of outpath
    if(outputPath.back() != '/') outputPath += '/'; 

    // timestamp generated here!
    std::string ts = std::to_string(time(nullptr)); //std::time()

    BackupParams b {
        .backupLabel = backupLabel,
        .outputPath = outputPath,
        .backupFreq = frequency_converted,
        .ts = ts,
        .sourceParams = sourceParams
    };

    return b;
}

uintmax_t traverseTree(rapidjson::Writer<rapidjson::FileWriteStream> &writer, const SourceParam &s, std::vector<std::string> &hashes)
{
    // get inode information
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
    size_t lastslash = s.path.find_last_of("/"); 
    writer.String(s.path.substr(lastslash+1).c_str()); 
    //writer.String(s.path.c_str());

    writer.Key("timeLastWritten");
    writer.Int64(st.st_mtime);

    uintmax_t sizeAcc = st.st_size ? st.st_size : 0;
    char hash[65];

    if ((s.flag == flagMap["fd"] || s.flag == flagMap["sd"]) && S_ISDIR(st.st_mode))
    {
        writer.Key("contents");
        writer.StartArray();

        std::vector<std::string> subdirectory_hashes;

        try {
            for (fs::directory_entry entry : fs::directory_iterator(s.path))
            {
                if((s.flag == flagMap["sd"]) && (is_directory(entry))) continue; // sd skips directories in general
                
                // replaces path back slashes with forward slashes
                std::string path = regex_replace(entry.path().string(), std::regex("\\\\+"), "/");

                SourceParam sourceParam {
                    .path = path,
                    .flag = (s.flag == flagMap["fd"] ? flagMap["fd"] : flagMap["f"])
                };

                // accumulate size of children
                
                sizeAcc += traverseTree(writer, sourceParam, subdirectory_hashes);
            }
        } catch (std::exception& e) {
            std::cout << "Unable to open " << s.path.c_str() << std::endl;
            std::cout << e.what() << std::endl;
            std::cout << "continuing..." << std::endl << std::endl;
        }

        writer.EndArray();
        
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
    }

    writer.Key("size");
    writer.Int64(sizeAcc);
    // END-DECORATE
    
    writer.EndObject();

    return sizeAcc;
} 

void buildDigest(BackupParams &backupParams, std::string &backupDigestLocation, std::string &backupDir)
{
    // individal backup (IB) directory label
    std::string backupTimestampedLabel = backupParams.backupLabel + "-" + backupParams.ts;

    // path to IB directory
    backupDir = backupParams.outputPath + backupParams.backupLabel + '/' + backupTimestampedLabel;

    // make the directory ^
    fs::create_directory(backupDir);

    // path to IB digest file
    backupDigestLocation = backupDir + "/" + backupTimestampedLabel + ".json";
    std::cout << "Writing digest to: " << backupDigestLocation << std::endl;

    if(FILE* fp = fopen(backupDigestLocation.c_str(), "w+"))
    {
        char writeBuf[65536];
        rapidjson::FileWriteStream outStream(fp, writeBuf, sizeof(writeBuf));
        rapidjson::Writer<rapidjson::FileWriteStream> fsWriter(outStream);

        fsWriter.StartObject();
        fsWriter.Key("name");
        fsWriter.String(backupParams.backupLabel.c_str());
        fsWriter.Key("files");
        fsWriter.StartArray();

        std::vector<std::string> hashes;
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

    else throw std::runtime_error("Error opening digest file.");
}
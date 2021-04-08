#ifndef TREE
#define TREE

#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/filewritestream.h"

using namespace std;

typedef struct
{
    const std::string path;
    const int flag;
} SourceParam;

typedef struct
{
    std::string backupLabel;
    std::string outputPath;
    std::string ts;
    std::vector<SourceParam> sourceParams; 
} BackupParams;

BackupParams generateBackupParams(std::string path);

void buildDigest(BackupParams &backupParams, string &backupDigestLocation);

uintmax_t traverseTree(rapidjson::Writer<rapidjson::FileWriteStream> &writer, const SourceParam &s, vector<string> &hashes);

#endif
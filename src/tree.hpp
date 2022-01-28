#ifndef TREE
#define TREE

#include <iostream>
#include <filesystem>
#include <sys/stat.h>
#include <map>
#include <fstream>
#include <regex>
#include <stdlib.h>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/filewritestream.h"

#include "sha256_hash.hpp"

typedef struct
{
    const std::string path;
    const int flag;
} SourceParam;

typedef struct
{
    std::string backupLabel;
    std::string outputPath;
    const int backupFreq;
    std::string ts;
    std::vector<SourceParam> sourceParams; 
} BackupParams;

BackupParams generateBackupParams(std::string path);

void buildDigest(BackupParams &backupParams, std::string &backupDigestLocation, std::string &backupDir);

uintmax_t traverseTree(rapidjson::Writer<rapidjson::FileWriteStream> &writer, const SourceParam &s, std::vector<std::string> &hashes);

#endif

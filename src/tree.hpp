<<<<<<< HEAD
#ifndef TREE
#define TREE

=======
>>>>>>> ee33bb0c3328ae86ea9367ff5110360d210a1207
#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
<<<<<<< HEAD
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
=======
#include "rapidjson/stringbuffer.h"
#include "rapidjson/istreamwrapper.h"
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include "rapidjson/filewritestream.h"


struct SourceParam
{
    const std::string path;
    const int flag;
};

struct BackupParams
{
    std::string backupLabel;
    std::string outputPath;
    std::vector<SourceParam> sourceParams; 
};

BackupParams generateBackupParams(std::string path);

void buildDigest(BackupParams &backupParams);

int64_t traverseTree(rapidjson::Writer<rapidjson::FileWriteStream> &writer, const SourceParam &s);
>>>>>>> ee33bb0c3328ae86ea9367ff5110360d210a1207

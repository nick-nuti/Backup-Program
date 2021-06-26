#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
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
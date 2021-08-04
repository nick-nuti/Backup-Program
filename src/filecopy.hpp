#ifndef FILECOPY
#define FILECOPY

#include <filesystem>
#include <iostream>

#include "rapidjson\document.h"
#include "rapidjson\filereadstream.h"
 
using namespace std;
using namespace rapidjson;
using namespace filesystem;

//void recursive_arrcopy(Value::ConstMemberIterator itr_in);

void filecopy(string jsonname);

#endif
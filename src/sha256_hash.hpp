#ifndef sha256_hash
#define sha256_hash

#include <openssl/sha.h>
#include <filesystem>
#include <stdio.h>
#include <vector> //ubuntu 20.04 complains about vectors if this is not here

int sha256_file(std::string path, char output[65]);

void sha256_dir(std::vector<std::string> directory_hashes, bool del_dirhash_file, char dir_hash[65]);

#endif
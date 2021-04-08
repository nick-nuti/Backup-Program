#ifndef sha256_hash
#define sha256_hash

int sha256_file(std::string path, char output[65]);

void sha256_dir(vector<string> directory_hashes, bool del_dirhash_file, char dir_hash[65]);

#endif
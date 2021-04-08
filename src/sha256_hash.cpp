#include <openssl/sha.h>
#include <filesystem>
#include <stdio.h>

using namespace std;
using namespace filesystem;

int sha256_file(string path, char output[65])
{
    FILE* fp = fopen(path.c_str(), "rb");
    if(!fp) return -1;

    SHA256_CTX sha256_struct;
    SHA256_Init(&sha256_struct); //initializes an instance of sha_ctx structure

    const int bufSize = 32768;
    vector<char> buff(bufSize);

    int bytesRead = 0; // total number of bytes read from the current fread

    while((bytesRead = fread(&buff[0], 1, bufSize, fp))) //read up to 4096 bytes at a time from the file and send to the update function
    {
        SHA256_Update(&sha256_struct, &buff[0], bytesRead); 
    }

    unsigned char hash[SHA256_DIGEST_LENGTH]; //20 bytes of output data
    SHA256_Final(hash, &sha256_struct); // places hash digest into the character array and erases the SHA_CTX structure instance
    
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) 
    {
        sprintf(output + (i * 2), "%02x", hash[i]); //convert hash char array into hexidecimal and place into "output"
    }

    output[64] = 0;

    fclose(fp);
    return 0;
}

void sha256_dir(vector<string> directory_hashes, bool del_dirhash_file, char hash_final[65])
{
    //open and overwrite the contents of the "dirhash.txt"
    FILE* getdirhash = fopen("dirhash.txt", "w");

    //loop through the hashes of children
    for(vector<string>::iterator iter = directory_hashes.begin(); iter < directory_hashes.end(); iter++)
    {
        //send hash info from vector to file, then hit return
        fputs((*iter).c_str(),getdirhash);
    }
    fclose(getdirhash);
    
    //get the sha hash of the created file

    sha256_file("dirhash.txt", hash_final);

    if(del_dirhash_file == 1) remove("dirhash.txt");
} 
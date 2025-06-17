#include <cstdlib>
#include "fs_client.h"
#include <string>
#include <iostream>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server> <port>\n";
        return 1;
    }

    fs_clientinit(argv[1], atoi(argv[2]));

    const char* user = "ethan";
    std::string dataStr = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";

    char blockData[FS_BLOCKSIZE] = {0};
    strncpy(blockData, dataStr.c_str(), FS_BLOCKSIZE - 1);

    fs_create(user, "/ethan", 'd');  

    // Going up past 124 to test error
    fs_create(user, "/ethan/file", 'f');
    uint32_t fillFile = 124;
    for (uint32_t i = 0; i < fillFile; i++) {
        std::cout << "Creating directory number: " << i << " in ethan file\n";
        fs_writeblock(user, "/ethan/file", i, dataStr.c_str());
    }

    for (uint32_t i = 0; i < 34; i ++) {
        std::string filestr = "/ethan/file" + std::to_string(i);
        fs_create(user, filestr.c_str(), 'f');
        uint32_t fillFile = 124;
        for (uint32_t i = 0; i < fillFile; i++) {
            std::cout << "Creating directory number: " << i << " in ethan file\n";
            fs_writeblock(user, filestr.c_str(), i, dataStr.c_str());
        }
    }

    return 0;
}

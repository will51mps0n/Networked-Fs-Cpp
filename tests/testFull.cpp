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
    int fillFile = 124 * 8;
    for (int i = 0; i < fillFile; i++) {
        std::cout << "Creating directory number: " << i << " in ethan file\n";
        std::string path = "/ethan/ethan" + std::to_string(i);
        fs_create(user, path.c_str(), 'd');
        std::string path2 = path + std::to_string(i);
        fs_create(user, path2.c_str(), 'f');
        fs_writeblock(user, path2.c_str(), 0, dataStr.c_str());
    }

    fs_create(user, "/ethan2", 'd');  
    for (int i = 0; i < fillFile; i++) {
        std::cout << "Creating directory number: " << i << " in ethan file\n";
        std::string path = "/ethan2/ethan" + std::to_string(i);
        fs_create(user, path.c_str(), 'd');
        std::string path2 = path + std::to_string(i);
        fs_create(user, path2.c_str(), 'f');
        fs_writeblock(user, path2.c_str(), 0, dataStr.c_str());
    }

    fs_create(user, "/ethan3", 'd');  
    for (int i = 0; i < fillFile; i++) {
        std::cout << "Creating directory number: " << i << " in ethan file\n";
        std::string path = "/ethan3/ethan" + std::to_string(i);
        fs_create(user, path.c_str(), 'd');
        std::string path2 = path + std::to_string(i);
        fs_create(user, path2.c_str(), 'f');
        fs_writeblock(user, path2.c_str(), 0, dataStr.c_str());
    }
    std::cout << "Trying to create while full\n";
    fs_create(user, "/ethan/ethanFull", 'd');

    fs_readblock(user, "/ethan", 0, blockData);


    return 0;
}

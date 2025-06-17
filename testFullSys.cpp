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

    fs_readblock(user, "/ethan", 0, blockData);
    fs_readblock(user, "/ethan", 123, blockData);

    fs_readblock(user, "/ethan/ethan0", 0, blockData);
    fs_readblock(user, "/ethan/ethan5", 0, blockData);

    fs_writeblock(user, "/ethan/ethan00", 0, dataStr.c_str());
    fs_writeblock(user, "/ethan3/ethan267", 0, dataStr.c_str());
    fs_writeblock(user, "/ethan/ethan00", 1, dataStr.c_str());
    fs_writeblock(user, "/ethan/ethan00", 2, dataStr.c_str());

    fs_create(user, "/adam", 'd');  
    fs_create(user, "/adam/data", 'd');  

    fs_create(user, "/ethan/kitty", 'd'); 
    fs_create(user, "/ethan/ethan0/kittyHere", 'd'); 
    
    fs_create(user, "/sergey", 'd');  

    fs_delete(user, "/ethan3/ethan267");
    fs_create(user, "/adam", 'd');  

    for (int i = 0; i < 2; i++) {
        std::cout << "Creating directory number: " << i << " in ethan file\n";
        std::string path = "/ethan/ethan" + std::to_string(i);
        fs_create(user, path.c_str(), 'd');
        std::string path2 = path + std::to_string(i);
        fs_create(user, path2.c_str(), 'f');
        fs_writeblock(user, path2.c_str(), 0, dataStr.c_str());
    }

    fs_create(user, "/ethan2", 'd');  
    for (int i = 0; i < 2; i++) {
        std::cout << "Creating directory number: " << i << " in ethan file\n";
        std::string path = "/ethan2/ethan" + std::to_string(i);
        fs_create(user, path.c_str(), 'd');
        std::string path2 = path + std::to_string(i);
        fs_create(user, path2.c_str(), 'f');
        fs_writeblock(user, path2.c_str(), 0, dataStr.c_str());
    }

    fs_create(user, "8", 'd');  
    for (int i = 0; i < 2; i++) {
        std::cout << "Creating directory number: " << i << " in ethan file\n";
        std::string path = "/ethan3/ethan" + std::to_string(i);
        fs_create(user, path.c_str(), 'd');
        std::string path2 = path + std::to_string(i);
        fs_create(user, path2.c_str(), 'f');
        fs_writeblock(user, path2.c_str(), 0, dataStr.c_str());
    }

    return 0;
}

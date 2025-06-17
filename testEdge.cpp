#include <cstdlib>
#include "fs_client.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
    fs_clientinit(argv[1], atoi(argv[2]));
    std::string dataStr = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    char readdata[FS_BLOCKSIZE];
    
    // this is long name not sure if its max 
    fs_create("longuser", "/longuser", 'd');
    fs_create("longuser", "/longuser/abcdefghijklmnopqrstuvwxyz1234567890", 'f');
    
    // create many directories in the ROOT
    for (unsigned int i = 0; i < FS_MAXFILEBLOCKS; i++) {
        std::string userDstr = "user" + std::to_string(i);
        fs_create(userDstr.c_str(), userDstr.c_str(), 'd');
    }
    
    // reading and also perhaps writing when it doesnt exist
    fs_readblock("longuser", "/longuser/nonexistent", 0, readdata);
    fs_writeblock("longuser", "/longuser/nonexistent", 0, dataStr.c_str());
    
    // creating file with same name as dir
    fs_create("collision", "/collision", 'd');
    fs_create("collision", "/collision/samename", 'd');
    fs_create("collision", "/collision/samename", 'f');
    
    // working on deleted files
    fs_create("temp", "/temp", 'd');
    fs_create("temp", "/temp/deleteme", 'f');
    fs_delete("temp", "/temp/deleteme");
    fs_readblock("temp", "/temp/deleteme", 0, readdata);
    
    // try to delete
    fs_delete("longuser", "/longuser/abcdefghijklmnopqrstuvwxyz1234567890");
    fs_delete("longuser", "/longuser");
    
    for (int i = 0; i < 10; i++) {
        std::string userDstr = "user" + std::to_string(i);
        fs_create(userDstr.c_str(), userDstr.c_str(), 'd');
    }
    
    fs_delete("collision", "/collision/samename");
    fs_delete("collision", "/collision");
    fs_delete("temp", "/temp");
    
    std::cout << "Edge all done\n";
    return 0;
}
#include <cstdlib>
#include "fs_client.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
    fs_clientinit(argv[1], atoi(argv[2]));
    
    // Creating empty sergey file
    fs_create("sergey", "/sergey", 'd');
    fs_create("sergey", "/sergey/projects", 'd');
    fs_create("sergey", "/sergey/projects/empty", 'f');
    
    // read from an empty file
    char readbuf[FS_BLOCKSIZE];
    fs_readblock("sergey", "/sergey/projects/empty", 0, readbuf);
    
    // write to block 5 of the sergey idk if this shuld work
    std::string dataStr = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    fs_writeblock("sergey", "/sergey/projects/empty", 5, dataStr.c_str());
    
    //read from empty block
    fs_readblock("sergey", "/sergey/projects/empty", 2, readbuf);
    
    // Read the block we wrote to
    fs_readblock("sergey", "/sergey/projects/empty", 5, readbuf);
    
    // delete sergeys stuff :( sorry
    fs_delete("sergey", "/sergey/projects/empty");
    fs_delete("sergey", "/sergey/projects");
    fs_delete("sergey", "/sergey");
    
    std::cout << "Test done\n";
    return 0;
}
#include <cstdlib>
#include "fs_client.h"
#include <string>
#include <iostream>

// Testing file size limits
int main(int argc, char* argv[]) {
    fs_clientinit(argv[1], atoi(argv[2]));
    std::string dataStr = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    
    // making my file for testing
    fs_create("adam", "/adam", 'd');
    fs_create("adam", "/adam/homework", 'd');
    fs_create("adam", "/adam/homework/eecs482", 'f');
    
    // writing to max - 1
    int maxBlocks = FS_MAXFILEBLOCKS - 1;
    for (int i = 0; i < maxBlocks; i++) {
        fs_writeblock("adam", "/adam/homework/eecs482", i, dataStr.c_str());
    }
    
    //reading blocks the first and the last
    char readbuf[FS_BLOCKSIZE];
    fs_readblock("adam", "/adam/homework/eecs482", 0, readbuf);
    fs_readblock("adam", "/adam/homework/eecs482", maxBlocks - 1, readbuf);
    
    // clean all, not sure if this is correct
    fs_delete("adam", "/adam/homework/eecs482");
    fs_delete("adam", "/adam/homework");
    fs_delete("adam", "/adam");
    
    std::cout << "Done with the test\n";
    return 0;
}
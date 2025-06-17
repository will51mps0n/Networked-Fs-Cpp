#include <cstdlib>
#include "fs_client.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
    fs_clientinit(argv[1], atoi(argv[2]));
    std::string dataStr = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    
    // This is just testing the deep nesting of file
    fs_create("adam", "/adam", 'd');
    fs_create("adam", "/adam/level1", 'd');
    fs_create("adam", "/adam/level1/level2", 'd');
    fs_create("adam", "/adam/level1/level2/level3", 'd');
    fs_create("adam", "/adam/level1/level2/level3/level4", 'd');
    fs_create("adam", "/adam/level1/level2/level3/level4/brawlstars", 'f');
    
    //Read and write from deep
    fs_writeblock("adam", "/adam/level1/level2/level3/level4/brawlstars", 0, dataStr.c_str());
    char readbuf[FS_BLOCKSIZE];
    fs_readblock("adam", "/adam/level1/level2/level3/level4/brawlstars", 0, readbuf);
    
    // Delete it all bottom up
    fs_delete("adam", "/adam/level1/level2/level3/level4/brawlstars");
    fs_delete("adam", "/adam/level1/level2/level3/level4");
    fs_delete("adam", "/adam/level1/level2/level3");
    fs_delete("adam", "/adam/level1/level2");
    fs_delete("adam", "/adam/level1");
    fs_delete("adam", "/adam");
    
    std::cout << "Test completed brawling\n";
    return 0;
}
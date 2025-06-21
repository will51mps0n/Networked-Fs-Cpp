#include <cstdlib>
#include "fs_client.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
    fs_clientinit(argv[1], atoi(argv[2]));
    std::string dataStr = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    
    // try files with same names in different user directories
    fs_create("ethan", "/common", 'd');
    fs_create("adam", "/common", 'd');
    fs_create("adam", "/common2", 'd');

    // create
    fs_create("ethan", "/common/kittyfile", 'f');
    fs_create("adam", "/common2/brawlstars", 'f');
    
    // write diff blocks to same file
    fs_writeblock("ethan", "/common/kittyfile", 0, dataStr.c_str());
    fs_writeblock("adam", "/common2/brawlstars", 0, dataStr.c_str());
    fs_writeblock("adam", "/common2/brawlstars", 1, dataStr.c_str());
    fs_writeblock("adam", "/common2/brawlstars", 2, dataStr.c_str());
    fs_writeblock("ethan", "/common/kittyfile", 1, dataStr.c_str());
    
    // write to block beyond the current end
    fs_writeblock("adam", "/common2/brawlstars", 5, dataStr.c_str());
    
    // create nester dir
    fs_create("ethan", "/common/gaming", 'd');
    fs_create("adam", "/common2/homework", 'd');
    
    // Create in nexted dirs
    fs_create("ethan", "/common/gaming/gianmarco", 'f');
    fs_create("adam", "/common2/homework/eecs482", 'f');
    
    // try recreating path that we alreaady have
    fs_create("ethan", "/common", 'd');
    fs_create("adam", "/common2/brawlstars", 'f');
    fs_create("adam", "/common2/brawlstars", 'f');

    //  del user's files in reverse 
    fs_delete("ethan", "/common/gaming/gianmarco");
    fs_delete("ethan", "/common/gaming");
    fs_delete("ethan", "/common/kittyfile");
    fs_delete("ethan", "/common");
    
    // delete adam stuff
    fs_delete("adam", "/common2/homework/eecs482");
    fs_delete("adam", "/common2/homework");
    fs_delete("adam", "/common2/brawlstars");
    fs_delete("adam", "/common2");
    
    std::cout << "Test completed success!!!!!!! nice.\n";
    return 0;
}
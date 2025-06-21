#include <cstdlib>
#include "fs_client.h"
#include "string"
#include <iostream>

int main(int argc, char* argv[]) {
    fs_clientinit(argv[1], atoi(argv[2]));

    std::string dataStr = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    fs_create("ethan", "/ethan", 'd');
    fs_create("ethan", "/ethan/kittyfile", 'f');
    fs_writeblock("ethan", "/ethan/kittyfile", 0, dataStr.c_str());
    fs_create("ethan", "/ethan/gaming", 'f');
    fs_writeblock("ethan", "/ethan/gaming", 0, dataStr.c_str());

    fs_create("adam", "/adam", 'd');
    fs_create("adam", "/adam/brawlstars", 'f');
    fs_writeblock("adam", "/adam/brawlstars", 0, dataStr.c_str());
    fs_create("adam", "/adam/homework", 'd');
    fs_create("adam", "/adam/homework/eecs482", 'f');
    fs_writeblock("adam", "/adam/homework/eecs482", 0, dataStr.c_str());

    fs_create("sergey", "/sergey", 'd');
    fs_create("sergey", "/sergey/projects", 'd');
    fs_create("sergey", "/sergey/projects/notes", 'f');
    fs_writeblock("sergey", "/sergey/projects/notes", 0, dataStr.c_str());

    fs_create("sergey", "/sergey/projects/full", 'f');
    
    std::cout << "done doing test\n";
    return 0;
}

#include <cstdlib>
#include <string>
#include "fs_client.h"
#include <iostream>

// Incase you accidentally restart the fs or something, just run teh testFs to get the setup file system for this test case
// otherwise its in the fs file for submitting and if we can read it in idk if we can
std::string dataStr = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";

int main(int argc, char* argv[]) {
    fs_clientinit(argv[1], atoi(argv[2]));

    char readdata[FS_BLOCKSIZE];

    // Try to read and write to the data that is in mem:
    fs_readblock("sergey", "/sergey/projects/notes", 0, readdata);
    fs_readblock("ethan", "/sergey/projects/notes", 0, readdata);

    fs_readblock("ethan", "/ethan/kittyfile", 0, readdata);
    fs_readblock("ethan", "/ethan/gaming", 0, readdata);
    fs_readblock("ethan", "/sergey/projects/notes", 0, readdata);

    fs_readblock("adam", "/adam/brawlstars", 0, readdata);
    fs_readblock("adam", "/adam/homework/eecs482", 0, readdata);
    fs_readblock("adam", "/sergey/projects/notes", 0, readdata);

    fs_readblock("sergey", "/sergey/projects/full", 0, readdata);

    // Try to delete a directory with a file in it
    fs_delete("sergey", "/sergey/projects");
    fs_delete("sergey", "/sergey/projects/notes");

    // try to delete already deleted 
    fs_delete("sergey", "/sergey/projects/notes");
    // try to delete now that empty
    fs_delete("sergey", "/sergey/projects");
    
    fs_writeblock("sergey", "/sergey/projects/notes", 0, dataStr.c_str());
    std::cout << "ethan trying to write to sergeys file\n";
    fs_writeblock("ethan", "/sergey/projects/notes", 0, dataStr.c_str());


    std::cout << "Adam trying to DESTROY ethans file\n";
    fs_delete("adam", "/ethan/gaming");

    std::cout << "sergey tries to overwrite existing file (nuking everyones code reference)\n";
    fs_create("sergey", "/sergey/projects/notes", 'f');

    std::cout << "sergey creating a file in his own file system\n";
    fs_create("sergey", "/sergey/projects/todo", 'f');
    fs_writeblock("sergey", "/sergey/projects/todo", 0, dataStr.c_str());

    std::cout << "filling a new file to max in adam homework. cap\n";
    fs_create("adam", "/adam/homework/thesis", 'f');
    for (size_t i = 0; i < FS_MAXFILEBLOCKS; ++i) {
        std::string text = "block_" + std::to_string(i);
        fs_writeblock("adam", "/adam/homework/thesis", i, dataStr.c_str());
    }

    std::cout << "overwriting the kitty :(\n";
    fs_writeblock("ethan", "/ethan/kittyfile", 0, dataStr.c_str());

    std::cout << "Ethan creating and writing his beats\n";
    fs_create("ethan", "/ethan/music", 'd');
    fs_create("ethan", "/ethan/music/playlist", 'f');
    fs_writeblock("ethan", "/ethan/music/playlist", 0, dataStr.c_str());

    std::cout << "sergey trying to write to dir. not cool dude...\n";
    fs_writeblock("sergey", "/sergey/projects", 0, dataStr.c_str());

    return 0;
}
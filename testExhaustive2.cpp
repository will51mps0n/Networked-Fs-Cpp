#include <cstdlib>
#include "fs_client.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
    fs_clientinit(argv[1], atoi(argv[2]));
    
    std::string dataStr = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness.";
    char readbuf[FS_BLOCKSIZE];

    fs_create("ethan", "/ethan", 'd');
    fs_create("ethan", "/ethan/kittyfile", 'f');
    fs_writeblock("ethan", "/ethan/kittyfile", 0, dataStr.c_str());
    fs_writeblock("ethan", "/ethan/kittyfile", 1, dataStr.c_str());
    fs_readblock("ethan", "/ethan/kittyfile", 1, readbuf);

    fs_create("ethan", "/ethan/music", 'd');
    fs_create("ethan", "/ethan/music/gianmarco", 'f');
    fs_writeblock("ethan", "/ethan/music/gianmarco", 0, dataStr.c_str());

    fs_create("ethan", "/ethan/music/brawlstars", 'f');
    for (size_t i = 0; i < FS_MAXFILEBLOCKS - 1; i++) {
        fs_writeblock("ethan", "/ethan/music/brawlstars", i, dataStr.c_str());
    }
    fs_readblock("ethan", "/ethan/music/brawlstars", FS_MAXFILEBLOCKS - 2, readbuf);
    fs_writeblock("ethan", "/ethan/music/brawlstars", FS_MAXFILEBLOCKS, dataStr.c_str());
    fs_readblock("ethan", "/ethan/music/brawlstars", FS_MAXFILEBLOCKS, readbuf);

    fs_create("adam", "/adam", 'd');
    fs_create("adam", "/adam/kittyfile", 'f');
    fs_writeblock("adam", "/adam/kittyfile", 0, dataStr.c_str());
    fs_delete("adam", "/adam/kittyfile");
    fs_create("adam", "/adam/kittyfile", 'f');
    fs_readblock("adam", "/adam/kittyfile", 0, readbuf);
    
    fs_create("sergey", "/sergey", 'd');
    for (int i = 0; i < 8; i++) {
        std::string fname = "/sergey/file" + std::to_string(i);
        fs_create("sergey", fname.c_str(), 'f');
        if (i % 2 == 0) fs_writeblock("sergey", fname.c_str(), 0, dataStr.c_str());
    }

    for (int i = 0; i < 8; i += 2) {
        std::string fname = "/sergey/file" + std::to_string(i);
        fs_delete("sergey", fname.c_str());
    }

    for (int i = 0; i < 4; i++) {
        std::string fname = "/sergey/reuse" + std::to_string(i);
        fs_create("sergey", fname.c_str(), 'f');
        fs_writeblock("sergey", fname.c_str(), 0, dataStr.c_str());
    }

    fs_delete("sergey", "/sergey/reuse0");
    fs_delete("sergey", "/sergey/reuse1");
    fs_delete("sergey", "/sergey/reuse2");
    fs_delete("sergey", "/sergey/reuse3");
    fs_delete("sergey", "/sergey/file1");
    fs_delete("sergey", "/sergey/file3");
    fs_delete("sergey", "/sergey/file5");
    fs_delete("sergey", "/sergey/file7");

    fs_delete("sergey", "/sergey");

    fs_create("ethan", "/ethan/deep", 'd');
    fs_create("ethan", "/ethan/deep/deeper", 'd');
    fs_create("ethan", "/ethan/deep/deeper/deepest", 'd');
    fs_create("ethan", "/ethan/deep/deeper/deepest/bigfile", 'f');
    fs_writeblock("ethan", "/ethan/deep/deeper/deepest/bigfile", 0, dataStr.c_str());

    fs_delete("ethan", "/ethan/deep/deeper/deepest/bigfile");
    fs_delete("ethan", "/ethan/deep/deeper/deepest");
    fs_delete("ethan", "/ethan/deep/deeper");
    fs_delete("ethan", "/ethan/deep");

    fs_readblock("ethan", "/ethan/kittyfile", 2, readbuf);
    fs_delete("ethan", "/ethan/kittyfile");

    for (int i = 0; i < 124; i++) {
        std::string name = "/ethan/file" + std::to_string(i);
        fs_create("ethan", name.c_str(), 'f');
    }

    for (int i = 0; i < 124; i++) {
        std::string name = "/ethan/file" + std::to_string(i);
        fs_delete("ethan", name.c_str());
    }

    fs_create("ethan", "/ethan/final", 'f');
    fs_writeblock("ethan", "/ethan/final", 0, dataStr.c_str());
    fs_readblock("ethan", "/ethan/final", 0, readbuf);
    fs_delete("ethan", "/ethan/final");
    fs_delete("ethan", "/ethan");
    fs_delete("adam", "/adam");

    fs_create("ethan", "/ethan/reborn", 'f');
    fs_writeblock("ethan", "/ethan/reborn", 0, dataStr.c_str());
    fs_delete("ethan", "/ethan/reborn");
    fs_create("ethan", "/ethan/reborn", 'f');
    fs_writeblock("ethan", "/ethan/reborn", 0, dataStr.c_str());
    fs_readblock("ethan", "/ethan/reborn", 0, readbuf);

    // Delete empty file vs. written file
    fs_create("ethan", "/ethan/blank", 'f');
    fs_delete("ethan", "/ethan/blank"); // empty
    fs_create("ethan", "/ethan/data", 'f');
    fs_writeblock("ethan", "/ethan/data", 0, dataStr.c_str());
    fs_delete("ethan", "/ethan/data"); // written

    fs_create("ethan", "/ethan/tempdir", 'd');
    fs_create("ethan", "/ethan/tempdir/deleteme", 'f');
    fs_delete("ethan", "/ethan/tempdir/deleteme");
    fs_delete("ethan", "/ethan/tempdir");

    fs_create("ethan", "/ethan/blocktester", 'f');
    fs_writeblock("ethan", "/ethan/blocktester", 0, dataStr.c_str());
    fs_writeblock("ethan", "/ethan/blocktester", FS_MAXFILEBLOCKS - 1, dataStr.c_str());
    fs_writeblock("ethan", "/ethan/blocktester", 99, dataStr.c_str());
    fs_delete("ethan", "/ethan/blocktester");

    // Double delete test
    fs_create("ethan", "/ethan/doublekill", 'f');
    fs_writeblock("ethan", "/ethan/doublekill", 0, dataStr.c_str());
    fs_delete("ethan", "/ethan/doublekill");
    fs_delete("ethan", "/ethan/doublekill");

    std::string longName = "/ethan/" + std::string(FS_MAXFILENAME - 1, 'a');
    fs_create("ethan", longName.c_str(), 'f');
    fs_delete("ethan", longName.c_str());

    // empty dir and deleted
    for (int i = 0; i < 3; i++) {
        std::string dirName = "/ethan/emptydir" + std::to_string(i);
        fs_create("ethan", dirName.c_str(), 'd');
    }
    for (int i = 0; i < 3; i++) {
        std::string dirName = "/ethan/emptydir" + std::to_string(i);
        fs_delete("ethan", dirName.c_str());
    }

    //create file then del then create
    fs_create("ethan", "/ethan/switcheroo", 'f');
    fs_writeblock("ethan", "/ethan/switcheroo", 0, dataStr.c_str());
    fs_delete("ethan", "/ethan/switcheroo");
    fs_create("ethan", "/ethan/switcheroo", 'd');
    fs_delete("ethan", "/ethan/switcheroo");

    // leave surviving ethan files to look at fs after
    fs_create("ethan", "/ethan/survivor1", 'f');
    fs_create("ethan", "/ethan/survivor2", 'f');

    std::cout << "all done. complete\n";
    return 0;
}
#include <cstdlib>
#include "fs_client.h"
#include <string>
#include <iostream>
#include <cstring>

int main(int argc, char* argv[]) {
    fs_clientinit(argv[1], atoi(argv[2]));

    std::string dataStr = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    char buf[FS_BLOCKSIZE];

    fs_create("ethan", "/ethan", 'd');
    fs_create("ethan", "/ethan/kittyfile", 'f');
    fs_writeblock("ethan", "/ethan/kittyfile", 0, dataStr.c_str());
    fs_writeblock("ethan", "/ethan/kittyfile", 1, dataStr.c_str());
    fs_readblock("ethan", "/ethan/kittyfile", 0, buf);

    fs_create("ethan", "/ethan/brawlstars", 'f');
    fs_writeblock("ethan", "/ethan/brawlstars", 0, dataStr.c_str());
    fs_writeblock("ethan", "/ethan/brawlstars", FS_MAXFILEBLOCKS - 1, dataStr.c_str());
    fs_writeblock("ethan", "/ethan/brawlstars", FS_MAXFILEBLOCKS, dataStr.c_str());
    fs_readblock("ethan", "/ethan/brawlstars", 2, buf);

    fs_writeblock("ethan", "/ethan", 0, dataStr.c_str());
    fs_writeblock("ethan", "/ethan/doesnotexist", 0, dataStr.c_str());

    fs_create("sergey", "/sergey", 'd');
    for (uint32_t i = 0; i < 60; i++) {
        std::string fname = "/sergey/f" + std::to_string(i);
        fs_create("sergey", fname.c_str(), 'f');
        if (i % 3 == 0) {
            fs_writeblock("sergey", fname.c_str(), 0, dataStr.c_str());
        }
    }

    for (uint32_t i = 0; i < 30; i++) {
        std::string fname = "/sergey/f" + std::to_string(i);
        fs_delete("sergey", fname.c_str());
    }

    for (uint32_t i = 0; i < 15; i++) {
        std::string fname = "/sergey/z" + std::to_string(i);
        fs_create("sergey", fname.c_str(), 'f');
        fs_writeblock("sergey", fname.c_str(), 0, dataStr.c_str());
    }

    fs_create("adam", "/adam", 'd');
    fs_create("adam", "/adam/nest", 'd');
    fs_create("adam", "/adam/nest/deep", 'd');
    fs_create("adam", "/adam/nest/deep/deeper", 'd');
    fs_create("adam", "/adam/nest/deep/deeper/deepest", 'f');
    fs_writeblock("adam", "/adam/nest/deep/deeper/deepest", 0, dataStr.c_str());

    fs_delete("adam", "/adam/nest/deep/deeper/deepest");
    fs_delete("adam", "/adam/nest/deep/deeper");
    fs_delete("adam", "/adam/nest/deep");
    fs_delete("adam", "/adam/nest");
    fs_create("adam", "/adam/flat", 'f');
    fs_writeblock("adam", "/adam/flat", 0, dataStr.c_str());

    fs_create("ethan", "/ethan/longfile", 'f');
    for (uint32_t i = 0; i < FS_MAXFILEBLOCKS; i++) {
        fs_writeblock("ethan", "/ethan/longfile", i, dataStr.c_str());
    }

    fs_delete("ethan", "/ethan/longfile");
    fs_delete("ethan", "/ethan/kittyfile");
    fs_delete("ethan", "/ethan/brawlstars");

    std::string longName = "/ethan/" + std::string(FS_MAXFILENAME - 1, 'x');
    fs_create("ethan", longName.c_str(), 'f');
    fs_writeblock("ethan", longName.c_str(), 0, dataStr.c_str());
    fs_delete("ethan", longName.c_str());

    fs_create("ethan", "/ethan/trywrite", 'd');
    fs_writeblock("ethan", "/ethan/trywrite", 0, dataStr.c_str());

    std::cout << "Done with test. Now use as fs pre imng for testLoad\n";
    return 0;
}

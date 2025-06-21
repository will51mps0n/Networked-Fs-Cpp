#include <cstdlib>
#include "fs_client.h"
#include <string>

int main(int argc, char* argv[]) {
    fs_clientinit(argv[1], atoi(argv[2]));

    std::string dataStr = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    char readbuf[FS_BLOCKSIZE];

    fs_create("ethan", "/ethan", 'd');
    fs_create("ethan", "/ethan/kittyfile", 'f');
    fs_writeblock("ethan", "/ethan/kittyfile", 0, dataStr.c_str());

    fs_create("ethan", "/ethan/kittybox", 'd');
    fs_create("ethan", "/ethan/kittybox/kittyfile", 'f');
    fs_writeblock("ethan", "/ethan/kittybox/kittyfile", 0, dataStr.c_str());
    fs_delete("ethan", "/ethan/kittybox/kittyfile");

    fs_create("ethan", "/ethan/kittybox/kittyfile", 'd');
    fs_create("ethan", "/ethan/kittybox/kittyfile/nestedkitty", 'f');
    fs_writeblock("ethan", "/ethan/kittybox/kittyfile/nestedkitty", 0, dataStr.c_str());

    fs_readblock("ethan", "/ethan/kittyfile", 0, readbuf);
    fs_delete("ethan", "/ethan/kittyfile");

    fs_create("ethan", "/ethan/brawlstars", 'f');
    fs_writeblock("ethan", "/ethan/brawlstars", 0, dataStr.c_str());
    fs_writeblock("ethan", "/ethan/brawlstars", 1, dataStr.c_str());
    fs_writeblock("ethan", "/ethan/brawlstars", FS_MAXFILEBLOCKS, dataStr.c_str());
    fs_readblock("ethan", "/ethan/brawlstars", 1, readbuf);

    return 0;
}

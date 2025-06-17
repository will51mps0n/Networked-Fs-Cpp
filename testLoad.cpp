#include <cstdlib>
#include "fs_client.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
    fs_clientinit(argv[1], atoi(argv[2]));

    std::string dataStr = "post image boot state testing";
    char buf[FS_BLOCKSIZE];

    fs_readblock("ethan", "/ethan/trywrite", 0, buf);
    fs_create("ethan", "/ethan/followup", 'f');
    fs_writeblock("ethan", "/ethan/followup", 0, dataStr.c_str());

    fs_delete("adam", "/adam/flat");
    fs_create("adam", "/adam/fresh", 'f');
    fs_writeblock("adam", "/adam/fresh", 0, dataStr.c_str());

    fs_readblock("sergey", "/sergey/z3", 0, buf);
    fs_delete("sergey", "/sergey/z3");
    fs_delete("sergey", "/sergey/z4");

    fs_create("sergey", "/sergey/newone", 'f');
    fs_writeblock("sergey", "/sergey/newone", 0, dataStr.c_str());

    std::cout << "Complete with test using the pre made fs syste, from testFullEx\n";
    return 0;
}

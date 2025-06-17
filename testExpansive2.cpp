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
    fs_create("adam", "/adam", 'd');
    fs_create("sergey", "/sergey", 'd');

    fs_create("ethan", "/ethan/data", 'f');
    fs_writeblock("ethan", "/ethan/data", 0, dataStr.c_str());
    fs_readblock("ethan", "/ethan/data", 0, buf);

    fs_writeblock("ethan", "/ethan/notafile", 0, dataStr.c_str());
    fs_writeblock("ethan", "/notadir/ghost", 0, dataStr.c_str());
    fs_writeblock("ethan", "/ethan", 0, dataStr.c_str());
    fs_writeblock("ethan", "/ethan/data", FS_MAXFILEBLOCKS + 1, dataStr.c_str());

    fs_create("ethan", "/ethan/deep", 'd');
    fs_create("ethan", "/ethan/deep/deeper", 'd');
    fs_create("ethan", "/ethan/deep/deeper/deepest", 'd');
    fs_create("ethan", "/ethan/deep/deeper/deepest/notes", 'f');
    fs_writeblock("ethan", "/ethan/deep/deeper/deepest/notes", 0, dataStr.c_str());
    fs_readblock("ethan", "/ethan/deep/deeper/deepest/notes", 0, buf);

    fs_create("ethan", "/ethan/temp", 'f');
    fs_delete("ethan", "/ethan/temp");
    fs_create("ethan", "/ethan/temp", 'd');
    fs_create("ethan", "/ethan/temp/file", 'f');
    fs_writeblock("ethan", "/ethan/temp/file", 0, dataStr.c_str());

    for (int i = 0; i < 128; i++) {
        std::string name = "/ethan/dir" + std::to_string(i);
        fs_create("ethan", name.c_str(), 'd');
    }

    fs_create("ethan", "/ethan/overflow", 'f');

    for (int i = 0; i < 64; i++) {
        std::string name = "/adam/a" + std::to_string(i);
        fs_create("adam", name.c_str(), 'f');
        fs_writeblock("adam", name.c_str(), 0, dataStr.c_str());
    }

    for (int i = 0; i < 64; i++) {
        std::string name = "/adam/a" + std::to_string(i);
        fs_delete("adam", name.c_str());
    }

    for (int i = 0; i < 64; i++) {
        std::string name = "/adam/z" + std::to_string(i);
        fs_create("adam", name.c_str(), 'f');
        if (i % 3 == 0) {
            fs_writeblock("adam", name.c_str(), 0, dataStr.c_str());
        }
    }

    for (int i = 0; i < 64; i += 2) {
        std::string name = "/adam/z" + std::to_string(i);
        fs_delete("adam", name.c_str());
    }

    for (int i = 0; i < 30; i++) {
        std::string name = "/sergey/f" + std::to_string(i);
        fs_create("sergey", name.c_str(), 'f');
        fs_writeblock("sergey", name.c_str(), 0, dataStr.c_str());
    }

    fs_delete("ethan", "/ethan/deep/deeper/deepest/notes");
    fs_delete("ethan", "/ethan/deep/deeper/deepest");
    fs_delete("ethan", "/ethan/deep/deeper");
    fs_delete("ethan", "/ethan/deep");

    fs_delete("ethan", "/ethan/data");
    fs_delete("ethan", "/ethan/temp/file");
    fs_delete("ethan", "/ethan/temp");
    fs_delete("ethan", "/ethan/overflow");

    for (int i = 0; i < 128; i++) {
        std::string name = "/ethan/dir" + std::to_string(i);
        fs_delete("ethan", name.c_str());
    }

    fs_create("ethan", "/ethan/reuse1", 'f');
    fs_create("ethan", "/ethan/reuse2", 'f');
    fs_create("ethan", "/ethan/reuse3", 'f');

    fs_writeblock("ethan", "/ethan/reuse3", 0, dataStr.c_str());
    fs_readblock("ethan", "/ethan/reuse3", 0, buf);

    fs_delete("ethan", "/ethan/reuse1");
    fs_delete("ethan", "/ethan/reuse2");
    fs_delete("ethan", "/ethan/reuse3");

    fs_delete("ethan", "/ethan");
    fs_delete("adam", "/adam");
    fs_delete("sergey", "/sergey");

    fs_create("ethan", "/ethan/deep", 'd');
    fs_create("ethan", "/ethan/deep/deeper", 'd');
    fs_create("ethan", "/ethan/deep/deeper/deepest", 'd');
    fs_create("ethan", "/ethan/deep/deeper/deepest/notes", 'f');

    std::cout << "done with expansive test\n";
    return 0;
}

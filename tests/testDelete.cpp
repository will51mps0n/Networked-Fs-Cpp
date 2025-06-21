#include <cstdlib>
#include "fs_client.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server> <port>\n";
        return 1;
    }
    fs_clientinit(argv[1], atoi(argv[2]));

    std::cout << "deleting file in the root\n";
    fs_delete("ethan", "/kittyfile");

    std::cout << "deleting directory in the root\n";
    fs_delete("ethan", "/music");

    std::cout << "deleting file in a subdirectory\n";
    fs_delete("ethan", "/music/song1");

    std::cout << "deleting directory in a subdirectory\n";
    fs_delete("ethan", "/music/classics");

    std::cout << "deleting file at beginning of sibling list\n";
    fs_delete("ethan", "/music/jazz");

    std::cout << "delete file in middle of sibling list\n";
    fs_delete("ethan", "/music/rock");

    std::cout << "Delete file at end of sibling list\n";
    fs_delete("ethan", "/music/metal");

    std::cout << "Delete empty dir with siblings\n";
    fs_delete("ethan", "/music/alt");

    std::cout << "deleting only file in a directory\n";
    fs_delete("ethan", "/singledir/onlyfile");

    std::cout << "deleting now-empty directory (should shrink)\n";
    fs_delete("ethan", "/singledir");

    std::cout << "deleting file in last directory block (causes shrink)\n";
    fs_delete("ethan", "/overflow/file15");

    std::cout << "deleting file from deeply nested structure\n";
    fs_delete("ethan", "/deep/deeper/deepest/bigfile");

    std::cout << "deleting and recreating file (reuse test)\n";
    fs_delete("adam", "/adam/recycleme");

    std::cout << "deleting empty file in middle of dir\n";
    fs_delete("sergey", "/sergey/file1");

    std::cout << "deleting empty file at end of dir\n";
    fs_delete("sergey", "/sergey/file7");

    std::cout << "deleting written file\n";
    fs_delete("ethan", "/switcheroo");

    std::cout << "deleting file and then recreating\n";
    fs_delete("ethan", "/reborn");

    std::cout << "deleting empty dir 0\n";
    fs_delete("ethan", "/emptydir0");

    std::cout << "deleting empty dir 1\n";
    fs_delete("ethan", "/emptydir1");

    std::cout << "deleting empty dir 2\n";
    fs_delete("ethan", "/emptydir2");

    std::cout << "deleting file and recreating it as a dir\n";
    fs_delete("ethan", "/ethan/switcheroo");

    std::cout << "all done\n";
    return 0;
}

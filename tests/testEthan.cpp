#include <cstdlib>
#include "fs_client.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
    fs_clientinit(argv[1], atoi(argv[2]));

    std::string dataStr = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    
    std::cout << "Creating multiple directories ";
    fs_create("user1", "/dir", 'd');
    for (int i = 0; i < 6; i++) {
        std::string toprint = "/dir/file" + std::to_string(i);
        fs_create("ethan", toprint.c_str(), 'f');
    }
    
    // Write to and read from multiple blocks in a file
    fs_writeblock("ethan", "/dir/file0", 0, dataStr.c_str());
    fs_writeblock("ethan", "/dir/file0", 1, dataStr.c_str());
    char readbuf[FS_BLOCKSIZE];
    fs_readblock("ethan", "/dir/file0", 1, readbuf);
    
    // Delete files to create "holes" in directory
    fs_delete("ethan", "/dir/file1");
    fs_delete("ethan", "/dir/file3");
    fs_delete("ethan", "/dir/file5");
    
    // Create new files (should reuse directory slots)
    fs_create("ethan", "/dir/newfile1", 'f');
    fs_create("ethan", "/dir/newfile2", 'f');
    
    // Delete all files to test empty directory block handling
    fs_delete("ethan", "/dir/file0");
    fs_delete("ethan", "/dir/file2");
    fs_delete("ethan", "/dir/file4");
    fs_delete("ethan", "/dir/newfile1");
    fs_delete("ethan", "/dir/newfile2");
    
    // Create file in "empty" directory (should use first block again)
    fs_create("ethan", "/dir/after_empty", 'f');
    
    // Test nested path handling and deletion
    fs_create("ethan", "/dir/nested", 'd');
    fs_create("user1", "/dir/nested/deep", 'd');
    fs_create("user1", "/dir/nested/deep/file", 'f');
    
    // Deleting in proper order (bottom-up)
    fs_delete("user1", "/dir/nested/deep/file");
    fs_delete("user1", "/dir/nested/deep");
    fs_delete("user1", "/dir/nested");
    fs_delete("user1", "/dir/after_empty");
    fs_delete("user1", "/dir");
    
    std::cout << "Test completed successfully\n";
    return 0;
}
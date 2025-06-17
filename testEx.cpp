#include <cstdlib>
#include "fs_client.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
    fs_clientinit(argv[1], atoi(argv[2]));

    std::string dataStr = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    
    std::cout << "Creating multiple directories in ethans stuff:\n";
    fs_create("user1", "/dir", 'd');
    for (int i = 0; i < 6; i++) {
        std::string toprint = "/dir/file" + std::to_string(i);
        fs_create("ethan",toprint.c_str(), 'f');
    }
    
    std::cout << "Writing and reading from multipe blocks of same file in ethan thanks ethan nice job!:\n";
    fs_writeblock("ethan", "/dir/file0", 0, dataStr.c_str());
    fs_writeblock("ethan", "/dir/file0", 1, dataStr.c_str());
    char readbuf[FS_BLOCKSIZE];
    fs_readblock("ethan", "/dir/file0", 1, readbuf);
    
    std::cout << "Deleting files out of order - if failing here check that removal of resizing directory blocks is correct:\n";
    fs_delete("ethan", "/dir/file1");
    fs_delete("ethan", "/dir/file3");
    fs_delete("ethan", "/dir/file5");
    
    std::cout << "Creating new files in the same dir that we just deleted some. This should fill ethans holes in the file system - soda speak\n";
    fs_create("ethan", "/dir/newfile1", 'f');
    fs_create("ethan", "/dir/newfile2", 'f');
    
    std::cout << "Deleting all files to handle empty file stuff\n";
    fs_delete("ethan", "/dir/file0");
    fs_delete("ethan", "/dir/file2");
    fs_delete("ethan", "/dir/file4");
    fs_delete("ethan", "/dir/newfile1");
    fs_delete("ethan", "/dir/newfile2");
    
    std::cout << "Creating  afile after the emptyin g of the file sys\n";
    fs_create("ethan", "/dir/after_empty", 'f');
    
    std::cout << "Creating a file deep inside of ethan nested deep in him\n";
    fs_create("ethan", "/dir/nested", 'd');
    fs_create("ethan", "/dir/nested/deep", 'd');
    fs_create("ethan", "/dir/nested/deep/file", 'f');
    
    std::cout << "Delete in bottom up order\n";
    fs_delete("ethan", "/dir/nested/deep/file");
    fs_delete("ethan", "/dir/nested/deep");
    fs_delete("ethan", "/dir/nested");
    fs_delete("ethan", "/dir/after_empty");
    fs_delete("ethan", "/dir");
    
    std::cout << "Test completed nice job epicly\n";
    return 0;
}
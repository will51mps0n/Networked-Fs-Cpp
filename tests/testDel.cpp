#include <cstdlib>
#include "fs_client.h"
#include <string>
#include <iostream>

// Creating then deleting a full directory
int main(int argc, char* argv[]) {
    fs_clientinit(argv[1], atoi(argv[2]));
    std::string dataStr = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";
    
    // crete dir
    fs_create("ethan", "/ethan", 'd');
    
    // Assiuming the max is 124, this should cover every case of filling a directory -
    // Fill new fill full single entry fill full everything try to fill a full and fail
    for (int i = 0; i < 125; i++) {
        std::string fileStr = "/ethan/file" + std::to_string(i);
        fs_create("ethan", fileStr.c_str(), 'f');
    }
    
    // This should also cover every delete case 
    for (int i = 0; i < 125; i++) {
        std::string fileStr = "/ethan/file" + std::to_string(i);
        fs_delete("ethan", fileStr.c_str());
    }
    
    fs_delete("ethan", "/ethan");
    
    std::cout << "Test completed successfully\n";
    return 0;
}
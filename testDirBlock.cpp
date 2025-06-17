#include <cstdlib>
#include "fs_client.h"
#include <string>
#include <iostream>

// In this one i try to handle the block management 
// Random order and such of which blocks we access
int main(int argc, char* argv[]) {
    fs_clientinit(argv[1], atoi(argv[2]));
    
    fs_create("ethan", "/ethan", 'd');
    
    // create and del files to test directory entry reuse
    for (int i = 0; i < 8; i++) {
        std::string fileStr = "/ethan/kittyfile" + std::to_string(i);
        fs_create("ethan", fileStr.c_str(), 'f');
    }
    
    // Deleting every other file
    for (int i = 0; i < 8; i += 2) {
        std::string fileStr = "/ethan/kittyfile" + std::to_string(i);
        fs_create("ethan", fileStr.c_str(), 'f');
    }
    
    // Create new files and it should re use the free blocks that were freed recently
    for (int i = 0; i < 4; i++) {
        std::string fileStr = "/ethan/gianmarco" + std::to_string(i);
        fs_create("ethan", fileStr.c_str(), 'f'); 
    }
    
    // Delete the remaining original files 
    for (int i = 1; i < 8; i += 2) {
        std::string fileStr = "/ethan/kittyfile" + std::to_string(i);
        fs_create("ethan", fileStr.c_str(), 'f');
    }
    
    // Delete new files
    for (int i = 0; i < 4; i++) {
        std::string fileStr = "/ethan/gianmarco" + std::to_string(i);
        fs_create("ethan", fileStr.c_str(), 'f'); 
    }
    
    // Delete parent, gooodbye ethan.
    fs_delete("ethan", "/ethan");
    
    std::cout << "Test all done\n";
    return 0;
}
#include <cstdlib>
#include "fs_client.h"
#include <string>
#include <iostream>

// Creating and deleting another mixed structure, the file uhhh search thing needs to be good for this
int main(int argc, char* argv[]) {
    fs_clientinit(argv[1], atoi(argv[2]));
    
    // mixed struct of file and dirs
    fs_create("ethan", "/ethan", 'd');
    fs_create("ethan", "/ethan/kittyfile", 'f');
    fs_create("ethan", "/ethan/gaming", 'd');
    fs_create("ethan", "/ethan/gaming/gianmarco", 'f');
    fs_create("ethan", "/ethan/music", 'd');
    fs_create("ethan", "/ethan/music/playlist", 'f');
    fs_create("ethan", "/ethan/music/settings", 'f');
    
    // Delete a file from a directory with multiple files
    fs_delete("ethan", "/ethan/music/settings");
    
    // delete file from a directory with one file
    fs_delete("ethan", "/ethan/gaming/gianmarco");
    
    // killing an empty directory
    fs_delete("ethan", "/ethan/gaming");
    
    // be gone directory with one file
    fs_delete("ethan", "/ethan/music/playlist");
    fs_delete("ethan", "/ethan/music");
    
    // getting rid of remaining remaining file and parent directory bye ethan
    fs_delete("ethan", "/ethan/kittyfile");
    fs_delete("ethan", "/ethan");
    
    std::cout << "Finished with da test\n";
    return 0;
}
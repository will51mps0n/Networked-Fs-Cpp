#include <cstdlib>
#include "fs_client.h"
#include <string>
#include <iostream>
#include <cstring>
#include <cassert>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server> <port>\n";
        return 1;
    }

    fs_clientinit(argv[1], atoi(argv[2]));

    const char* user = "adam";

    // Test non-unique pathname but different type (d vs f)
    fs_create(user, "/adam", 'd');

    // This one should fail
    assert(fs_create(user, "/adam", 'f') == -1);

    // Test pathname not starting with '/'
    assert(fs_create(user, "sergey", 'f') == -1);

    // Test wrong pathname ending in '/'
    assert(fs_create(user, "/ethan/", 'f') == -1);

    // Create a valid jawn for later use (manos = user)
    // These next three should be good:
    const char* user1 = "manos";
    fs_create(user1, "/manos", 'd');
    fs_create(user1, "/manos/bing", 'd');

    const char* user2 = "hawktwuah";
    // Should fail because user 2 'hawktwuah' does not have access to 'manos' /manos
    assert(fs_create(user2, "/manos/bong", 'd') == -1);

    // Should fail because we have spaces in the jawns
    assert(fs_create(user1, "/ma nos/ting", 'd') == -1);

    // Should fail because we have leading 0s in block
    char buffer[FS_BLOCKSIZE];
    assert(fs_readblock("ethan", "/ethan", 00, buffer) == -1);


    char readbuf[FS_BLOCKSIZE];
    readbuf[0] = 's';
    readbuf[1] = 'e';
    readbuf[2] = 'r';
    readbuf[3] = 'g';
    readbuf[4] = 'e';
    readbuf[5] = 'y';
    readbuf[6] = 'k';

    // Access files you don't own
    fs_readblock("sergey", "/ethan", 0, readbuf);

    return 0;
}

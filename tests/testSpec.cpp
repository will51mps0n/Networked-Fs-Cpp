#include <iostream>
#include <cassert>
#include <cstdlib>
#include "fs_client.h"

int main(int argc, char* argv[]) {
    char* server;
    int server_port;

    const char* writedata = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";

    char readdata[FS_BLOCKSIZE];
    int status;

    if (argc != 3) {
        std::cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }
    server = argv[1];
    server_port = atoi(argv[2]);

    std::cout << "Check 1: done initializing Server and port\n ";

    fs_clientinit(server, server_port);
    std::cout << "Check 2: done initializing client\n ";

    status = fs_create("user1", "/dir", 'd');
    assert(!status);
    std::cout << "Check 3: done calling fs create with type d\n ";

    status = fs_create("user1", "/dir/file", 'f');
    assert(!status);
    std::cout << "Check 4: done calling fs create with type f\n ";


    status = fs_writeblock("user1", "/dir/file", 0, writedata);
    assert(!status);
     std::cout << "Check 5: done calling fs write\n ";

    status = fs_readblock("user1", "/dir/file", 0, readdata);
    assert(!status);
    std::cout << "Check 6: done calling fs read\n ";

    status = fs_delete("user1", "/dir/file");
    assert(!status);
    std::cout << "Check 7: done calling fs delete on file\n ";

    status = fs_delete("user1", "/dir");
    assert(!status);
    std::cout << "Check 8: done calling fs delete on dir\n "; 

    std::cout << "Done with test\n ";

    return 0;
} 
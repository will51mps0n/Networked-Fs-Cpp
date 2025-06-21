#include <iostream>
#include <cstdlib>
#include <string>
#include "fs_client.h"

// test change for git push
int main(int argc, char* argv[]) {
    char* server;
    int server_port;

    const char* writedata = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";

    char readdata[FS_BLOCKSIZE];
    int status;

    std::string invalidName = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"; 
    std::string whitespaceName = "  Sergy  ";
    std::string whitespaceFile = "   /file  ";
    std::string badFile = "/file/";
    std::string whiteSpaceType = " d ";
    std::string badType = "s";
    std::string whiteSpaceBlock = " 0 ";
    std::string badBlock = "two";
    std::string tooBigBlock = "1000000000000000";
    std::string leadingZeroBlock = "001";
    std::string emptyPath = "";
    std::string rootPath = "/";
    std::string doubleSlashPath = "/dir//file";
    std::string internalWhitespace = "/di r/file";
    std::string weirdCharPath = "/dir/fi$le";
    std::string badUser = "user 1";
    std::string emptyUser = "";

    if (argc != 3) {
        std::cout << "error: usage: " << argv[0] << " <server> <serverPort>\n";
        exit(1);
    }

    server = argv[1];
    server_port = atoi(argv[2]);

    std::cout << "Check 1: initialized server and port\n";
    fs_clientinit(server, server_port);
    std::cout << "Check 2: initialized client\n";

    // valid dir + file
    status = fs_create("user1", "/dir", 'd');
    std::cout << "Valid create dir /dir, status: " << status << "\n";

    status = fs_create("user1", "/dir/file", 'f');
    std::cout << "Valid create file /dir/file, status: " << status << "\n";

    status = fs_writeblock("user1", "/dir/file", 0, writedata);
    std::cout << "Valid write to /dir/file, status: " << status << "\n";

    status = fs_readblock("user1", "/dir/file", 0, readdata);
    std::cout << "Valid read from /dir/file, status: " << status << "\n";
    std::cout << " Valid: special char: " << fs_create("user1", weirdCharPath.c_str(), 'f') << "\n";

    std::cout << "Trying invalid create:\n";
    std::cout << "  empty : " << fs_create("user1", emptyPath.c_str(), 'f') << "\n";
    std::cout << "  trailing slash: " << fs_create("user1", badFile.c_str(), 'f') << "\n";
    std::cout << "  whitespace : " << fs_create("user1", "   /file  ", 'f') << "\n";
    std::cout << "  whitespace : " << fs_create("  user1   ", "/file", 'f') << "\n";
    std::cout << "  whitespace : " << fs_create("us er1", "/file", 'f') << "\n";
    std::cout << "  internal whitespace: " << fs_create("user1", "/di r/file", 'f') << "\n";
    std::cout << "  too long: " << fs_create("user1", ("/" + invalidName).c_str(), 'f') << "\n";
    std::cout << "  too long: " << fs_create(invalidName.c_str(), ("/" + invalidName).c_str(), 'f') << "\n";
    std::cout << "  double slash: " << fs_create("user1", doubleSlashPath.c_str(), 'f') << "\n";
    std::cout << "  whitespace: " << fs_create("user1", "/dir/f1", whiteSpaceType[1]) << "\n";
    std::cout << "  invalid type: " << fs_create("user1", "/dir/f2", badType[0]) << "\n";

    std::cout << "Trying invalid writing:\n";
    std::cout << "  zero " << fs_writeblock("user1", "/dir/file", atoi(leadingZeroBlock.c_str()), writedata) << "\n";
    std::cout << " too big " << fs_writeblock("user1", "/dir/file", atoi(tooBigBlock.c_str()), writedata) << "\n";

    std::cout << "invalid reading:\n";
    std::cout << "  user whitsepace: " << fs_readblock(badUser.c_str(), "/dir/file", 0, readdata) << "\n";
    std::cout << "  empty user: " << fs_readblock(emptyUser.c_str(), "/dir/file", 0, readdata) << "\n";

    std::cout << "delete invalid calls:\n";
    std::cout << "trailing slash: " << fs_delete("user1", badFile.c_str()) << "\n";
    std::cout << "  invalid user: " << fs_delete(badUser.c_str(), "/dir/file") << "\n";
    std::cout << "  empty user: " << fs_delete(emptyUser.c_str(), "/dir/file") << "\n";
    std::cout << "  delete the root directory " << fs_delete(emptyUser.c_str(), "/") << "\n";

    status = fs_delete("user1", "/dir/file");
    std::cout << "delete of /dir/file, done: " << status << "\n";

    status = fs_delete("user1", "/dir");
    std::cout << "delete of /dir, done: " << status << "\n";

    std::cout << "Done with tests\n";

    return 0;
}

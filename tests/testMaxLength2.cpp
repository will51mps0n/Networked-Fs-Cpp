#include <cstdlib>
#include "fs_client.h"
#include <string>

int main(int argc, char* argv[]) {
    fs_clientinit(argv[1], atoi(argv[2]));

    std::string dataStr = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";

    std::string maxUser(FS_MAXUSERNAME, 'A');
    std::string maxFile = "/" + std::string(FS_MAXFILENAME, 'F');
    char readbuf[FS_BLOCKSIZE];

    fs_create(maxUser.c_str(), "/maxdir", 'd');
    fs_create(maxUser.c_str(), maxFile.c_str(), 'f');
    fs_writeblock(maxUser.c_str(), maxFile.c_str(), 0, dataStr.c_str());
    fs_readblock(maxUser.c_str(), maxFile.c_str(), 0, readbuf);
    fs_delete(maxUser.c_str(), maxFile.c_str());
    fs_delete(maxUser.c_str(), "/maxdir");

    return 0;
}

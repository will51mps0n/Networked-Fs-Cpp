#include <cstdlib>
#include "fs_client.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
    fs_clientinit(argv[1], atoi(argv[2]));

    std::string dataStr = "We hold these truths to be self-evident, that all men are created equal, that they are endowed by their Creator with certain unalienable Rights, that among these are Life, Liberty and the pursuit of Happiness. -- That to secure these rights, Governments are instituted among Men, deriving their just powers from the consent of the governed, -- That whenever any Form of Government becomes destructive of these ends, it is the Right of the People to alter or to abolish it, and to institute new Government, laying its foundation on such principles and organizing its powers in such form, as to them shall seem most likely to effect their Safety and Happiness.";

    std::cout << "creating deep structure with only one block written to in the most baby one\n";
    fs_create("ethan", "/deep", 'd');
    fs_create("ethan", "/deep/deeper", 'd');
    fs_create("ethan", "/deep/deeper/deepest", 'd');
    fs_create("ethan", "/deep/deeper/deepest/bigfile", 'f');
    fs_writeblock("ethan", "/deep/deeper/deepest/bigfile", 0, dataStr.c_str());

    std::cout << "Created Nest Strcture\n";
    return 0;
}

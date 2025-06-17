#include <cstdlib>
#include "fs_client.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server> <port>\n";
        return 1;
    }
    fs_clientinit(argv[1], atoi(argv[2]));

    std::cout << "Deleteing nested file - should recrusively delete everything\n";
    fs_delete("ethan", "/deep/deeper/deepest/bigfile");

    return 0;
}

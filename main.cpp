#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <boost/thread.hpp>
#include "fs_server.h"
#include "parseRequests.h"
#include "kittyServer.h"
#include "diskManager.h"

extern DiskManager serverMemory;

// This function gets a port
int parse_port(int argc, char *argv[])
{
    // default the OS will define the port
    int port = 0;

    if (argc > 1)
    {
        try
        {
            // Iterate through the whole string
            for (const char *p = argv[1]; *p; ++p)
            {
                if (!isdigit(*p))
                {
                    throw std::invalid_argument("Port not an integer :(");
                }
            }
            port = std::stoi(argv[1]);
            // Need to be in valid port range:
            if (port < 1024 || port > 65535)
            {
                throw std::out_of_range("Port out of range - should be 1024 - 65535");
            }
        }
        catch (const std::exception &e)
        {
            return 0;
        }
    }
    return port;
}

// Main - Creates a socket to port and runs the file server
int main(int argc, char *argv[])
{
    // Parse port from command line
    int port = parse_port(argc, argv);

    // Set up the server socket
    int serverFd = setup_socket(port);
    if (serverFd < 0)
        return 1;

    serverMemory.initialize_from_disk();

    // Print the port after both initialization steps.
    print_port(port);
    run_server(serverFd);
}

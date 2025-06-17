#include "FsOperations.h"

//DiskManager serverMemory;

int create_socket() {
    // *** Creating a socket *** 
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) {
        //std::cerr << "Error making meow the socket socket meow\n"; 
        return -1;
    }

    // *** Setting socket options ***
    int optionVal = 1;
    
    if(setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &optionVal, sizeof(optionVal)) < 0) {
        //std::cerr << "Error setting the socket options\n";
        close(serverFd);
        return -1;
    } 
    return serverFd;
}

bool bind_socket(int serverFd, int& port) {
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY); 
    address.sin_port = htons(port);

    // *** Binding the socket ***
    if(bind(serverFd, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0) {
        //std::cerr << "Error binding socket to port\n";
        close(serverFd);
        return false;
    }

    // *** Getting the Assigned Port ***
    if (port == 0) {
        socklen_t addrLen = sizeof(address);
        if (getsockname(serverFd, reinterpret_cast<sockaddr*>(&address), &addrLen) < 0) {
            //std::cerr << "Error: socket name retrieval failure" << std::endl;
            close(serverFd);
            return false;
        }
        port = ntohs(address.sin_port);
    }
    return true;
}

bool set_socket_listening(int serverFd) {
    if (listen(serverFd, 30) < 0) {
        //std::cerr << "Error listening on socket" << std::endl;
        return false;
    }
    return true;
}

int setup_socket(int &port) {
    // * Create socket *
    int serverFd = create_socket();
    if (serverFd < 0) {
        return -1;
    }

    // * Bind Socket *
    if (!bind_socket(serverFd, port)) {
        close(serverFd);
        return -1;
    }

    // * Set to listening mode *
    if (!set_socket_listening(serverFd)) {
        close(serverFd);
        return -1;
    }

    return serverFd;
}

void handle_client(int clientFd){

    char buffer[FS_BLOCKSIZE + 1] = {0};
    ssize_t bytesRead = 0;
    ssize_t totalRead = 0;
    
    while (totalRead < FS_BLOCKSIZE) {
        bytesRead = recv(clientFd, buffer + totalRead, 1, 0);
        if (bytesRead <= 0) {
            close(clientFd);
            return;
        }
        
        if (buffer[totalRead] == '\0') {
            break;
        }
        
        totalRead += bytesRead;
    }
    
    buffer[totalRead] = '\0';
    
    std::string message(buffer);
    
    std::vector<std::string> requestTokens = parse_request(message);
    if (requestTokens.empty()) {
        close(clientFd);
        return;
    }
    
    std::string requestType = requestTokens[0];
    

    if ((requestTokens.empty()) || 
        (!is_valid_request_type(requestType)) ||
        (!valid_request(message, requestTokens))) {
        close(clientFd);
        return;   
    }
    std::string rawPath = requestTokens[2];
    std::vector<std::string> pathTokens = get_path_files(rawPath);
    FsOperations kittyManager(clientFd, message, requestTokens, false, rawPath, pathTokens);

    char code = requestType[3];
    switch (code) {
        case 'R': 
            kittyManager.handle_read_request();
            break;
        case 'W': 
            kittyManager.handle_write_request();
            break;
        case 'C': 
            kittyManager.handle_create_request();
            break;
        case 'D': 
            kittyManager.handle_delete_request();
            break;   
    }
    close(clientFd);
    return;
}

void run_server(int serverFd) {
    // Continue accepting new client connections until the server is shut down
    while(true) {
        // Create a storage struct for the connecting clients addr info
        sockaddr_in clientAddress{};
        socklen_t clientAddrLen = sizeof(clientAddress);
        // Accept function blocks waits and creates client 
        int clientFd = accept(serverFd,reinterpret_cast<sockaddr*>(&clientAddress), &clientAddrLen);
        if (clientFd < 0) {
            // Print an error, but continue to accept other clients (print is for our debuging early maybe delete later)
            continue;  
        }
        // Create a new thread to handle client
        boost::thread client_thread(handle_client, clientFd);
        // Let thread run independent:
        client_thread.detach();
    }
}
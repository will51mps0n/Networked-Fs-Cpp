#pragma once
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <boost/thread.hpp>
#include "fs_server.h"
#include "parseRequests.h"
#include "diskManager.h"

// Global disk Manager object

/*** create_socket() ***
 * This function creates a socket and returns the server Fd int of the created socket on success and -1 on failure
 * This function has two main steps:
 * 1 * Creating a socket *
 * AF_INET address family for the IPv4 protocol family
 * SOCK_STREAM as socket type - communication semanitcs with TCP
 * 0 Protocol - specified to use the default protocol for socket type (TCP)
 * 2 * Setting socket options *
 * serverFd is given as the socket file descriptor to configure
 * SOL_SOCKET specifies this option resides in the socket level (change socket to reuse addr)
 * SO_REUSEADDR specified which option to set - allow reusing of the local address
 * Must pass a pointer to option val to the function, 1 = enable
 */
int create_socket();
/*** bind_socket() ***
 * The function binds a socket to an address - Returns true if the socket is binded correctly, false otherwise
 * Takes in an int server file data and a reference to the port of the socket
 * This function has three main steps:
 * 1 * Preparing Address Structure *
 * Initialize a sock address object, and zero initialize it
 * Set IP address family to use AF_INET for IPv4
 * Set IP address field to bind to all available interfaces (INADDR_ANY) - Host to net work long (htonl) - converts 32 bit val to network bytes
 * Set the port number field - ie what port to bind to - Host to network short (htons) - convers 16 bit int to network bytes
 * 2 * Binding the socket *
 * serverFd the socket file to bind the info to
 * bind() works through multiple address families, use generic reinterpret_cast<sockaddr*>(&address)
 * sizeof(address) size of the address structure in bytes
 * 3 * Getting the Assigned Port *
 * Pass in the socketFd as the socket we retrieve information about
 * If 0, OS assigns the port number
 * Adderess is the Buffer wehre the address info will be stored
 * Third param is both input (for the buffer size) and output (actual size used) 
 */
bool bind_socket(int serverFd, int &port);
/*
 *** set_socket_listening() ***
 * This function tells the socket to begin listenign for connections
 * We know now waht port the OS assign if we req port 0. Socket itself remains the same
 * First parameter socketFd is which socket to put in listening mode
 * Second parameter 30 - is the connection backlog - the maximum queue lengtuh for pending connections
 * So if the queue fills to 30, new connection attempts are rejected until space is avaiable 
 */
bool set_socket_listening(int serverFd);
/*** setup_socket() ***
 * This function sets up a socket
 * Takes in an int for the port number
 * It returns a -1 on failure and the socket server fd otherwise
 * It first creates the socket , then binds it, then sets it to listening mode
 * After it prints the port and returns the file fd
 */
int setup_socket(int &port);
void handle_client(int clientFd);
void run_server(int serverFd);

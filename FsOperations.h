#pragma once
#include "diskManager.h"
#include "kittyServer.h"

extern DiskManager serverMemory;

/**
 * FsOperations class
 * 
 * This class handles filesystem operations requested by clients
 * Implements create, read, write, and delete operations for files and directories
 */
class FsOperations {
    public:
        /**
         * Constructor for FsOperations
         * 
         * @param clFd - Client socket file descriptor for communication
         * @param m - Message to be sent back to client
         * @param rT - Tokenized request received from client
         * @param debugFlag - Whether to print debug information
         * @param FP - File path string from the request
         * @param t - Tokenized path components
         */
        FsOperations(int clFd, std::string &m, std::vector<std::string> &rT, bool debugFlag, std::string &FP, std::vector<std::string> &t);
        
        /**
         * Handle client create request (FS_CREATE)
         * Creates a new file or directory in the filesystem
         */
        void handle_create_request();
        
        /**
         * Handle client read request (FS_READBLOCK)
         * Reads a block of data from a file or directory
         */
        void handle_read_request();
        
        /**
         * Handle client write request (FS_WRITEBLOCK)
         * Writes a block of data to a file
         */
        void handle_write_request();
        
        /**
         * Handle client delete request (FS_DELETE)
         * Deletes a file or empty directory
         */
        void handle_delete_request();

    private:
        // Client connection variables
        int clientFd;            // Client socket file descriptor
        std::string message;     // Response message to send
        std::vector<std::string> &requestTokens;  // Tokenized request
        bool debug;              // Debug flag
        std::string filePath;    // Request file path
        std::vector<std::string> &tokens;  // Tokenized path components
        std::string user;        // Username from request
        
        /**
         * Traverse filesystem to find a node
         * 
         * Consolidated path traversal function that handles both finding
         * target nodes and parent directories
         * 
         * @param isDirectory - Whether we expect a directory
         * @param needParent - Whether to get parent of target (for create/delete)
         * @param outInode - Reference to store found inode
         * @param outName - If needParent is true, stores the target name
         * @return Block number of found node, or -1 if not found
         */
        int traverse_path(bool isDirectory, bool needParent, fs_inode &outInode, std::string &outName, std::unique_ptr<WriteLockGuard>& guardOut);
        int traverse_path_read(bool isDirectory, bool needParent, fs_inode &outInode, std::string &outName, std::unique_ptr<ReadLockGuard> &guardOut);

        
        /* Check if a path name conflicts with existing entries
         * 
         * @param dir - Directory inode to check
         * @param name - Name to check for conflicts
         * @param entryToFind - Reference to store conflicting entry if found
         * @return True if conflict exists, false otherwise
         */
        bool path_conflict(fs_inode &dir, const std::string &name, fs_direntry &entryToFind);
        
        /**
         * Insert a directory entry into a parent directory
         * 
         * @param parent - Parent directory inode
         * @param parentBlockNum - Block number of parent directory
         * @param name - Name for new entry
         * @param newInode - Inode data for new entry
         * @return True if successful, false otherwise
         */
        bool insert_direntry(fs_inode &parent, int parentBlockNum, const std::string &name, fs_inode &newInode);
        
        /**
         * Check if a directory block is empty
         * 
         * @param entries - Array of directory entries to check
         * @return True if all entries are unused (inode_block=0)
         */
        bool is_directory_block_empty(fs_direntry* entries);
        
        /**
         * Remove an empty directory block
         * 
         * @param parentInode - Parent directory inode
         * @param blockIndex - Index of block to remove in parent's blocks array
         */
        int remove_directory_block(fs_inode &parentInode, uint32_t blockIndex);
        
        /**
         * Check if a block number is valid for a file
         * 
         * @param blockNum - Block number to check
         * @param inode - Inode of the file
         * @return True if valid, false otherwise
         */
        bool is_block_valid(uint32_t blockNum, const fs_inode& inode);
        
        /**
         * Allocate a free block from the disk
         * 
         * @return Block number allocated, or -1 if no free blocks
         */
        int allocate_free_block();
        
        /**
         * Print debug information if debug flag is set
         * 
         * @param operation - Operation being performed (e.g., "CREATE")
         * @param message - Debug message to print
         */
        void debug_print(const std::string& operation, const std::string& message);
        
        /**
         * Check if inode type matches expected type
         * 
         * @param inode - Inode to check
         * @param isDirectory - Whether a directory is expected
         * @param needParent - Whether this is a parent lookup
         * @return True if permissions are valid, false otherwise
         */
        bool check_permissions(const fs_inode &inode, bool isDirectory, bool needParent);
};

/**
 * Parse a file path into components
 * 
 * @param request - File path string (starting with '/')
 * @return Vector of path components
 */
std::vector<std::string> get_path_files(std::string &request);
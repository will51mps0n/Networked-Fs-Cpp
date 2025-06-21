#include "FsOperations.h"

extern DiskManager serverMemory;

FsOperations::FsOperations(int clFd, std::string &m, std::vector<std::string> &rT, bool debugFlag, std::string &FP, std::vector<std::string> &t)
    : clientFd(clFd), message(m), requestTokens(rT), debug(debugFlag), filePath(FP), tokens(t), user(rT[1]) {}

int FsOperations::traverse_path(bool isDirectory, bool needParent, fs_inode &outInode, std::string &outName, std::unique_ptr<WriteLockGuard> &guardOut)
{
    if (tokens.empty())
        return -1;
    debug_print("TRAVERSE", "Begin traverse function");

    // Create a copy of tokens - dont modify original tokens
    std::vector<std::string> pathTokens = tokens;
    std::unique_ptr<ReadLockGuard> currentReadGuard;

    // Extract target name if we need parent
    if (needParent && !pathTokens.empty())
    {
        outName = pathTokens.back();
        pathTokens.pop_back();
    }

    size_t tokenNum = 0;
    int currentBlock = ROOT_DIR_BLOCK;

    debug_print("TRAVERSE", "Creating current guard for block: " + std::to_string(currentBlock));
    if (pathTokens.size() == 0) {
        guardOut = std::make_unique<WriteLockGuard>(serverMemory, currentBlock);
    }
    else {
        currentReadGuard = std::make_unique<ReadLockGuard>(serverMemory, currentBlock);
    }

    // Navigate path
    while (tokenNum < pathTokens.size())
    {
        disk_readblock(currentBlock, &outInode);
        // Check permissions of this block match the user
        if (currentBlock != 0 && (user != std::string(outInode.owner)))
            return -1;
        // Verify current node is directory
        if (outInode.type != 'd')
            return -1;

        bool found = false;

        // send current dir
        for (uint32_t i = 0; i < outInode.size && !found; i++)
        {
            uint32_t dataBlock = outInode.blocks[i];
            fs_direntry entries[ENTRIES_PER_BLOCK]{};
            disk_readblock(dataBlock, &entries);

            // 
            for (int j = 0; j < ENTRIES_PER_BLOCK && !found; j++)
            {
                if (entries[j].inode_block != 0 &&
                    pathTokens[tokenNum] == std::string(entries[j].name))
                {

                    int nextBlock = entries[j].inode_block;
                    debug_print("TRAVERSE", "Attempting to acquire lock for " + std::to_string(nextBlock));
                    if(tokenNum == pathTokens.size()-1) {
                        guardOut = std::make_unique<WriteLockGuard>(serverMemory, nextBlock);
                        currentReadGuard.reset();
                        //guardOut = std::move(newLock);
                    }
                    else {
                        auto newLock = std::make_unique<ReadLockGuard>(serverMemory, nextBlock);
                        currentReadGuard = std::move(newLock);
                    }
                    debug_print("TRAVERSE", "Setting current to new and releasing guard for current block " + std::to_string(currentBlock));
                    //currentGuard = std::move(newLock);

                    currentBlock = nextBlock;
                    ++tokenNum;

                    found = true;
                }
            }
        }

        if (!found)
            return -1;
    }

    // Read the final node
    // We have current node locked
    disk_readblock(currentBlock, &outInode);

    if (currentBlock != 0 && user != std::string(outInode.owner))
        return -1;
    if ((isDirectory && outInode.type != 'd') || (!isDirectory && !needParent && outInode.type != 'f'))
        return -1;

    debug_print("TRAVERSE", "Moving guard to be for " + std::to_string(currentBlock));

    //guardOut = std::move(currentGuard);
    return currentBlock;
}

int FsOperations::traverse_path_read(bool isDirectory, bool needParent, fs_inode &outInode, std::string &outName, std::unique_ptr<ReadLockGuard> &guardOut)
{
    if (tokens.empty())
        return -1;
    debug_print("TRAVERSE", "Begin traverse function");

    // Create a copy of tokens - dont modify original tokens
    std::vector<std::string> pathTokens = tokens;

    // Extract target name if we need parent
    if (needParent && !pathTokens.empty())
    {
        outName = pathTokens.back();
        pathTokens.pop_back();
    }

    size_t tokenNum = 0;
    int currentBlock = ROOT_DIR_BLOCK;
    debug_print("TRAVERSE", "Creating current guard for block: " + std::to_string(currentBlock));
    std::unique_ptr<ReadLockGuard> currentGuard = std::make_unique<ReadLockGuard>(serverMemory, currentBlock);

    // Navigate path
    while (tokenNum < pathTokens.size())
    {
        disk_readblock(currentBlock, &outInode);
        // Check permissions of this block match the user
        if (currentBlock != 0 && (user != std::string(outInode.owner)))
            return -1;
        // Verify current node is directory
        if (outInode.type != 'd')
            return -1;

        bool found = false;

        // send current dir
        for (uint32_t i = 0; i < outInode.size && !found; i++)
        {
            uint32_t dataBlock = outInode.blocks[i];
            fs_direntry entries[ENTRIES_PER_BLOCK]{};
            disk_readblock(dataBlock, &entries);

            // 
            for (int j = 0; j < ENTRIES_PER_BLOCK && !found; j++)
            {
                if (entries[j].inode_block != 0 &&
                    pathTokens[tokenNum] == std::string(entries[j].name))
                {

                    int nextBlock = entries[j].inode_block;
                    debug_print("TRAVERSE", "Attempting to acquire lock for " + std::to_string(nextBlock));
                    auto newLock = std::make_unique<ReadLockGuard>(serverMemory, nextBlock);
                    debug_print("TRAVERSE", "Setting current to new and releasing guard for current block " + std::to_string(currentBlock));
                    currentGuard = std::move(newLock);

                    currentBlock = nextBlock;
                    ++tokenNum;

                    found = true;
                }
            }
        }

        if (!found)
            return -1;
    }

    // Read the final node
    // We have current node locked
    disk_readblock(currentBlock, &outInode);

    if (currentBlock != 0 && user != std::string(outInode.owner))
        return -1;
    if ((isDirectory && outInode.type != 'd') || (!isDirectory && !needParent && outInode.type != 'f'))
        return -1;

    debug_print("TRAVERSE", "Moving guard to be for " + std::to_string(currentBlock));

    guardOut = std::move(currentGuard);
    return currentBlock;
}

void FsOperations::handle_create_request()
{
    char type = requestTokens[3][0];
    debug_print("CREATE", "Request for user '" + user + "', path '" + filePath + "', type '" + type + "'");

    std::string name;
    fs_inode parentInode;
    std::unique_ptr<WriteLockGuard> parentGuard;

    // find parent
    int parentBlock = traverse_path(true, true, parentInode, name, parentGuard);
    if (parentBlock == -1) return;

    if (parentInode.type != 'd') return;

    debug_print("CREATE", "Found parent directory at block: " + std::to_string(parentBlock));

    // init new inode
    fs_inode newInode{};
    newInode.type = type;
    strncpy(newInode.owner, user.c_str(), FS_MAXUSERNAME + 1);
    newInode.size = 0;

    // attempt to insert dir entry
    if (!insert_direntry(parentInode, parentBlock, name, newInode))
        return;

    send(clientFd, message.c_str(), message.length() + 1, MSG_NOSIGNAL);
    debug_print("CREATE", "Request completed successfully");
}

// This is only called in create when parent lock is already held
bool FsOperations::insert_direntry(fs_inode &parent, int parentBlockNum, const std::string &name, fs_inode &newInode)
{
    debug_print("INSERT", "Beginning insert direntry for parentBlock: " + std::to_string(parentBlockNum));
    bool spotFound = false;
    std::pair<uint32_t, int> dataLocation;
    fs_direntry targetEntries[ENTRIES_PER_BLOCK];
    uint32_t freeBlockForNew = 0;
    // check name and find an empty spot
    for (uint32_t i = 0; i < parent.size; i++)
    {
        fs_direntry entries[ENTRIES_PER_BLOCK]{};
        debug_print("INSERT", "In for loop, parent at i (target) is block:  " + std::to_string(parent.blocks[i]));
        std::unique_ptr<ReadLockGuard> currentReadGuard = std::make_unique<ReadLockGuard>(serverMemory, parent.blocks[i]);
        disk_readblock(parent.blocks[i], &entries);
        // CHANGED NAMING 
        for (int j = 0; j < ENTRIES_PER_BLOCK; j++)
        {
            if (entries[j].inode_block != 0 && name == std::string(entries[j].name))
                return false;
        }

        // Lots of ifs - but skip this if we already found a spot. Still need to go through the for loop to check naming conglicts
        if (!spotFound) {
            for (int j = 0; j < ENTRIES_PER_BLOCK; j++)
            {
                if (!spotFound && entries[j].inode_block == 0)
                {
                    freeBlockForNew = parent.blocks[i];

                    std::memcpy(targetEntries, entries, sizeof(entries));
                    spotFound = true;
                    // BUG FIX FOR FULL SYS
                    freeBlockForNew = parent.blocks[i];
                    dataLocation = std::make_pair(i, j);
                }
            }
        }
    }

    if (!spotFound && parent.size == FS_MAXFILEBLOCKS)
        return false;
    
    // allocate a new blcok for the inode
    debug_print("INSERT", "Allocating new inode block");
    int inodeBlock = allocate_free_block();
    if (inodeBlock == -1)
        return false;

    debug_print("INSERT", "New block allocated for inodeBlock is:  " + std::to_string(inodeBlock));
    // write new inode to disk
    disk_writeblock(inodeBlock, &newInode);

    if (!spotFound)
    {
        // need to allocate a new directory blck in this case
        int newDirectoryBlock = allocate_free_block();
        if (newDirectoryBlock == -1)
        {
            serverMemory.free_block(inodeBlock);
            return false;
        }
        debug_print("INSERT", "Spot not found, NEW block allocated for newDirBlock is:  " + std::to_string(newDirectoryBlock));
        // Put in scope so the RAII releases after we write
        {
            WriteLockGuard dirGuard(serverMemory, newDirectoryBlock);
            fs_direntry newEntries[ENTRIES_PER_BLOCK]{};
            strncpy(newEntries[0].name, name.c_str(), FS_MAXFILENAME + 1);
            newEntries[0].inode_block = inodeBlock;
            debug_print("INSERT", "Spot not found, writing to block: " + std::to_string(newDirectoryBlock));
            disk_writeblock(newDirectoryBlock, &newEntries);
        }

        // We still have parent locked when entering here thanks to RAII call in the create handle funct
        debug_print("INSERT", "Writing to parent block " + std::to_string(parentBlockNum));
        parent.blocks[parent.size++] = newDirectoryBlock;
        disk_writeblock(parentBlockNum, &parent);
    }
    else
    {
        // update existing dir entry
        WriteLockGuard updateGuard(serverMemory, freeBlockForNew);
        targetEntries[dataLocation.second].inode_block = inodeBlock;
        strncpy(targetEntries[dataLocation.second].name, name.c_str(), FS_MAXFILENAME + 1);

        // Write lock guard releases when out of else
        debug_print("INSERT", "In else statement, writing to block: " + std::to_string(freeBlockForNew));
        disk_writeblock(freeBlockForNew, &targetEntries);
    }

    return true;
}

void FsOperations::handle_write_request()
{
    if (requestTokens.size() < 4)
        return;
    uint32_t blockNum = std::stoul(requestTokens[3]);

    debug_print("WRITE", "Request for user '" + user + "', path '" + filePath + "', block " + std::to_string(blockNum));

    // read data from client
    char dataBlock[FS_BLOCKSIZE];
    size_t readData = 0;
    while (readData < FS_BLOCKSIZE)
    {
        ssize_t bytes = recv(clientFd, dataBlock + readData, FS_BLOCKSIZE - readData, 0);
        if (bytes <= 0)
            return;
        readData += bytes;
    }

    fs_inode inode;
    std::string dummyName;
    std::unique_ptr<WriteLockGuard> inodeGuard;

    // find the file
    int inodeBlock = traverse_path(false, false, inode, dummyName, inodeGuard);
    if (inodeBlock == -1)
        return;

    // validating the block number
    if (!is_block_valid(blockNum, inode))
        return;

    int newBlock = 0;

    // Handle writing to a new block vs. existing block
    if (blockNum == inode.size)
    {
        newBlock = allocate_free_block();
        if (newBlock == -1)
            return;

        // Lock new data and then write
        {
            WriteLockGuard blockGuard(serverMemory, newBlock);
            disk_writeblock(newBlock, &dataBlock);
        }

        // Update inode on disk
        inode.blocks[inode.size++] = newBlock;
        disk_writeblock(inodeBlock, &inode);
    }
    else
    {
        uint32_t dataBlockNum = inode.blocks[blockNum];

        WriteLockGuard blockGuard(serverMemory, dataBlockNum);
        disk_writeblock(dataBlockNum, &dataBlock);
    }

    send(clientFd, message.c_str(), message.length() + 1, MSG_NOSIGNAL);
    debug_print("WRITE", "Request completed successfully");
}

void FsOperations::handle_read_request()
{
    if (requestTokens.size() < 4)
        return;
    uint32_t blockNum = std::stoul(requestTokens[3]);

    debug_print("READ", "Request for user '" + user + "', path '" + filePath + "', block " + std::to_string(blockNum));

    fs_inode inode;
    std::string dummyName;
    std::unique_ptr<ReadLockGuard> inodeGuard;

    // find the file
    int inodeBlock = traverse_path_read(false, false, inode, dummyName, inodeGuard);
    if (inodeBlock == -1)
        return;

    // LOCK GUARD FOR READING

    // Check if block number is valid
    if (blockNum >= inode.size)
        return;

    // Then read the data block
    uint32_t dataBlock = inode.blocks[blockNum];

    // Read and send data based on node type
    if (inode.type == 'd')
    {
        return;
    }
    else
    {
        // Reading file data
        ReadLockGuard blockGuard(serverMemory, dataBlock);
        char buffer[FS_BLOCKSIZE];
        debug_print("READ", "In else statement of read, reading block " + std::to_string(dataBlock));
        disk_readblock(dataBlock, &buffer);

        send(clientFd, message.c_str(), message.length() + 1, MSG_NOSIGNAL);
        send(clientFd, buffer, FS_BLOCKSIZE, MSG_NOSIGNAL);
    }

    debug_print("READ", "Request completed successfully");
}

void FsOperations::handle_delete_request()
{
    debug_print("DELETE", "Request for user '" + user + "'");

    std::string name;
    fs_inode parentInode;

    std::unique_ptr<WriteLockGuard> parentGuard;

    // find parent
    int parentBlock = traverse_path(true, true, parentInode, name, parentGuard);
    if (parentBlock == -1)
        return;

    // trying to delete parent
    if (parentBlock == 0 && name.empty())
        return;
    bool entryFound = false;
    
    // search for target
    for (uint32_t i = 0; i < parentInode.size && !entryFound; i++)
    {
        uint32_t datablock = parentInode.blocks[i];

        fs_direntry entries[ENTRIES_PER_BLOCK];

        // Dont need to lock to read data block
        disk_readblock(datablock, &entries);

        // CHANGED!!
        for (uint32_t j = 0; j < ENTRIES_PER_BLOCK; j++)
        {
            if (entries[j].inode_block != 0 && name == std::string(entries[j].name))
            {
                int blockToDelete = entries[j].inode_block;

                if (entryFound) {
                    debug_print("DELETE", "Entry of the same name found again");
                }
                // We need to write lock target block

                fs_inode toDelete;
                WriteLockGuard blockGuard(serverMemory, blockToDelete);
                debug_print("DELETE", "Reading block to delete " + std::to_string(blockToDelete));
                disk_readblock(blockToDelete, &toDelete);

                // check user permissions
                if (user != std::string(toDelete.owner))
                {
                    debug_print("DELETE", "User not authorized to delete inode");
                    return;
                }

                // If not an empty directory - canot delete
                if (toDelete.type == 'd' && toDelete.size > 0)
                    return;
                // Mark entry as deleted
                entries[j].inode_block = 0;

                // Update directory
                if (is_directory_block_empty(entries))
                {
                    int goodbye = remove_directory_block(parentInode, i);
                    if (goodbye != -1) {
                        debug_print("DELETE", "Writing to Parentblock: " + std::to_string(parentBlock));
                        disk_writeblock(parentBlock, &parentInode);  
                        debug_print("DELETE", "Freeing goodbye: " + std::to_string(goodbye));
                        serverMemory.free_block(goodbye); 
                    }
                    else {
                        disk_writeblock(datablock, entries);
                    }
                }
                else
                {
                    debug_print("DELETE", "Writing to datablock: " + std::to_string(datablock));
                    disk_writeblock(datablock, entries);
                }

                if (toDelete.type == 'f') {
                    debug_print("DELETE", "THIS IS A FILE IN THE IF FOR TO DELETE");
                    for (uint32_t kill = 0; kill < toDelete.size; kill++) {
                        debug_print("DELETE", "Iteration number: " + std::to_string(kill));
                        if (toDelete.blocks[kill] != 0) {
                            debug_print("DELETE", "Freeing Block: " + std::to_string(toDelete.blocks[kill]));
                            serverMemory.free_block(toDelete.blocks[kill]);
                        }
                    } 
                }


                serverMemory.free_block(blockToDelete);
                send(clientFd, message.c_str(), message.length() + 1, MSG_NOSIGNAL);
                debug_print("DELETE", "Request completed successfully");
                entryFound = true;
            }
        }
    }
    return;
}

bool FsOperations::is_directory_block_empty(fs_direntry *entries)
{
    for (int i = 0; i < ENTRIES_PER_BLOCK; i++)
    {
        if (entries[i].inode_block != 0)
            return false;
    }
    return true;
}

int FsOperations::remove_directory_block(fs_inode &parentInode, uint32_t blockIndex)
{
    if (blockIndex >= parentInode.size)
        return -1;

    uint32_t blockToFree = parentInode.blocks[blockIndex];
    //parentInode.blocks[blockIndex] = 0;
    //serverMemory.free_block(blockToFree); 

    // WriteLockGuard blockGuard(serverMemory, blockToFree);

    // shift blocks to fill the gap
    for (uint32_t i = blockIndex; i < parentInode.size - 1; i++)
    {
        parentInode.blocks[i] = parentInode.blocks[i + 1];
    }

    // clear the last entry
    parentInode.blocks[parentInode.size - 1] = 0;
    parentInode.size--;

    return blockToFree;
}

bool FsOperations::path_conflict(fs_inode &dir, const std::string &name, fs_direntry &entryToFind)
{
    for (uint32_t i = 0; i < dir.size; i++)
    {
        uint32_t block = dir.blocks[i];

        // Read lock the directory data block
        ReadLockGuard guard(serverMemory, block);

        fs_direntry entries[ENTRIES_PER_BLOCK]{};
        disk_readblock(block, &entries);

        for (int j = 0; j < ENTRIES_PER_BLOCK; j++)
        {
            if (entries[j].inode_block != 0 && name == std::string(entries[j].name))
            {
                entryToFind = entries[j];
                return true;
            }
        }
    }
    return false;
}

void FsOperations::debug_print(const std::string &operation, const std::string &message)
{
    if (debug)
    {
        boost::lock_guard<boost::mutex> lock(cout_lock);
        std::cout << operation << ": " << message << std::endl;
    }
}

bool FsOperations::check_permissions(const fs_inode &inode, bool isDirectory, bool needParent)
{
    if (isDirectory && inode.type != 'd')
        return false;
    if (!isDirectory && !needParent && inode.type != 'f')
        return false;
    return true;
}

bool FsOperations::is_block_valid(uint32_t blockNum, const fs_inode &inode)
{
    // Out of range for all
    if (blockNum >= FS_MAXFILEBLOCKS) return false;
    
    //  allow wring to block at end of file
    return ((blockNum <= inode.size) && (blockNum >= 0));
}

int FsOperations::allocate_free_block()
{
    debug_print("ALLOC", "Allocating free block");
    int block = serverMemory.allocate_block();
    debug_print("ALLOC", "Got block " + std::to_string(block));
    return block;
}

std::vector<std::string> get_path_files(std::string &request)
{
    std::vector<std::string> result;
    if (request.empty() || request[0] != '/')
        return result;

    std::string path = request.substr(1);
    std::stringstream ss(path);
    std::string token;

    while (std::getline(ss, token, '/'))
    {
        if (!token.empty())
        {
            result.push_back(token);
        }
        else
        {
            return {};
        }
    }

    return result;
}
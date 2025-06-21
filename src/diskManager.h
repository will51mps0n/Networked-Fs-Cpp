#pragma once
#include "fs_server.h"
#include "fs_client.h"
#include "fs_param.h"
#include <set>
#include <queue>
#include <unordered_map>

const int ENTRIES_PER_BLOCK = FS_BLOCKSIZE / sizeof(fs_direntry);
const int ROOT_DIR_BLOCK = 0;
const int MAX_FREE_BLOCKS = 4096;

/**
 * DiskManager class
 *
 * Manages allocation and deallocation of disk blocks.
 * Also provides locking primitives to protect block-level concurrency.
 */
class DiskManager
{
private:
    /**
     * Mutex to protect access to the free block set
     */
    boost::mutex freeMutex;

    /**
     * Recursively marks used blocks starting from a given inode block
     *
     * @param blockNum - Starting block number
     * @param usedBlocks - Boolean vector of used blocks
     */
    void find_used_blocks(uint32_t blockNum, std::vector<bool> &usedBlocks);

public:
    /**
     * Constructor
     */
    DiskManager();

    /**
     * Set of currently free blocks on the disk
     */
    std::set<uint32_t> freeBlocks;

    /**
     * Allocates and removes the first available free block from the set
     *
     * @return Block number if available, 0 if none
     */
    int allocate_block();

    /**
     * Marks a block as free again and adds it to the set
     *
     * @param blockNum - Block number to free
     */
    void free_block(uint32_t blockNum);

    /**
     * Initializes free block set by scanning the disk for used blocks
     */
    void initialize_from_disk();

    /**
     * Lock management for read/write locks on inode blocks
     */
    void lock_read(int inodeBlock);
    void unlock_read(int inodeBlock);
    void lock_write(int inodeBlock);
    void unlock_write(int inodeBlock);

    /**
     * Lock table mapping block numbers to shared mutexes
     * Used for both read and write synchronization
     */
    boost::mutex lockTableMutex;
    std::unordered_map<int, boost::shared_mutex> inodeLocks;
};

/**
 * Global DiskManager instance shared across server code
 */
extern DiskManager serverMemory;

/**
 * ReadLockGuard class
 *
 * Acquires a shared read lock on the given block for the lifetime of the object
 */
class ReadLockGuard
{
public:
    ReadLockGuard(DiskManager &mgr, int blockNum);
    ~ReadLockGuard();

private:
    DiskManager &diskMgr;
    int block;
    boost::shared_mutex *mutex;
};

/**
 * WriteLockGuard class
 *
 * Acquires an exclusive write lock on the given block for the lifetime of the object
 */
class WriteLockGuard
{
public:
    WriteLockGuard(DiskManager &mgr, int blockNum);
    ~WriteLockGuard();

private:
    DiskManager &diskMgr;
    int block;
    boost::shared_mutex *mutex;
};
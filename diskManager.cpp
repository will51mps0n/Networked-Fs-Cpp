#include "diskManager.h"

DiskManager serverMemory;

DiskManager::DiskManager() {}

/*
 * Use up one of the free blocks for a create call
 */
int DiskManager::allocate_block()
{
    boost::lock_guard<boost::mutex> lock(freeMutex);
    if (freeBlocks.empty())
        return -1;
    uint32_t block = *freeBlocks.begin();
    freeBlocks.erase(block);
    return block;
}

/*
 * Use up one of the free blocks for a create call
 */
void DiskManager::free_block(uint32_t blockNum)
{
    boost::lock_guard<boost::mutex> lock(freeMutex);
    if (blockNum < MAX_FREE_BLOCKS)
    {
        freeBlocks.insert(blockNum);
    }
}

/*
 * Use up one of the free blocks for a create call
 */
void DiskManager::initialize_from_disk()
{
    boost::lock_guard<boost::mutex> lock(freeMutex);
    std::vector<bool> usedBlocks(MAX_FREE_BLOCKS, false);
    find_used_blocks(ROOT_DIR_BLOCK, usedBlocks);
    freeBlocks.clear();
    for (uint32_t i = 0; i < MAX_FREE_BLOCKS; i++)
    {
        if (!usedBlocks[i])
        {
            freeBlocks.insert(i);
        }
    }
}

// Find the used blocks so we know which ones are free
void DiskManager::find_used_blocks(uint32_t blockNum, std::vector<bool> &usedBlocks)
{
    usedBlocks[blockNum] = true;

    fs_inode currentInode;
    disk_readblock(blockNum, &currentInode);

    for (uint32_t i = 0; i < currentInode.size; i++)
    {
        uint32_t childBlockNum = currentInode.blocks[i];

        if (childBlockNum == 0)
        {
            continue;
        }
        usedBlocks[childBlockNum] = true;
        if (currentInode.type == 'd')
        {

            fs_direntry entries[ENTRIES_PER_BLOCK];
            disk_readblock(childBlockNum, &entries);

            for (uint32_t j = 0; j < ENTRIES_PER_BLOCK; j++)
            {
                if (entries[j].inode_block != 0)
                {
                    find_used_blocks(entries[j].inode_block, usedBlocks);
                }
            }
        }
    }
}

// Construct read lock RAII
ReadLockGuard::ReadLockGuard(DiskManager &mgr, int blockNum)
    : diskMgr(mgr), block(blockNum)
{
    {
        std::lock_guard<boost::mutex> lock(diskMgr.lockTableMutex);
        auto [it, _] = diskMgr.inodeLocks.try_emplace(block);
        mutex = &it->second;
    }
    mutex->lock_shared();
}

// Destruct read lock RAII
ReadLockGuard::~ReadLockGuard()
{
    mutex->unlock_shared();
}

// Construct write lock RAII
WriteLockGuard::WriteLockGuard(DiskManager &mgr, int blockNum)
    : diskMgr(mgr), block(blockNum)
{
    {
        std::lock_guard<boost::mutex> lock(diskMgr.lockTableMutex);
        auto [it, _] = diskMgr.inodeLocks.try_emplace(block);
        mutex = &it->second;
    }
    mutex->lock();
}

// Destruct write lock RAII
WriteLockGuard::~WriteLockGuard()
{
    mutex->unlock();
}

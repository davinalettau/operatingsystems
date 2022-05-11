
#include <xinu.h>

/**
 * Attempt to free a block of memory based on malloc() accounting information
 * stored in preceding two words.
 *
 * @param ptr
 *      A pointer to the memory block to free.
 */
syscall free(void *ptr)
{
    struct memblock *pmem;

    /* TODO:
     *      1) set block to point to memblock to be free'd (ptr)
     *      2) find accounting information of the memblock
     *      3) call freemem syscall on the block with its length
     */
    pmem = ((struct memblock *)ptr) - 1;

    // check to make sure not unallocating block that wasn't allocated by malloc
    if (pmem->next != pmem)
    {
        return SYSERR;
    }
    return freemem(pmem, pmem->length);
}

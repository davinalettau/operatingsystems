
#include <xinu.h>

/**
 * Request heap storage, record accounting information, returning pointer
 * to assigned memory region.
 *
 * @param size
 *      number of bytes requested
 *
 * @return
 *      pointer to region on success, or @c NULL on failure
 */
void *malloc(ulong size)
{
    struct memblock *pmem;

    /* we don't allocate 0 bytes. */
    if (0 == size)
    {
        return NULL;
    }

    /** TODO:
     *      1) Make room for accounting info
     *      2) Acquire memory with getmem syscall
     *         - handle possible error (SYSERR) from getmem...
     *      3) Set accounting info in pmem
     *      4) Return proper pointer to base of free memory region
     */
    // Get memory block with extra space for accounting info
    pmem = getmem(size + sizeof(memblk));
    // ERROR
    if (pmem == (void *)SYSERR)
    {
        return NULL;
    }
    pmem->length = size + sizeof(memblk);
    pmem->next = pmem;
    return pmem + 1;
}


#include <xinu.h>

/**
 * Frees a block of heap-allocated memory.
 *
 * @param memptr
 *      Pointer to memory block allocated with memget().
 *
 * @param nbytes
 *      Length of memory block, in bytes.  (Same value passed to memget().)
 *
 * @return
 *      ::OK on success; ::SYSERR on failure.  This function can only fail
 *      because of memory corruption or specifying an invalid memory block.
 */
syscall freemem(void *memptr, ulong nbytes)
{
    register struct memblock *block, *next, *prev;

    /* make sure block is in heap */
    if ((0 == nbytes) || ((ulong)memptr < freelist.base) || ((ulong)memptr > freelist.base + freelist.bound))
    {
        return SYSERR;
    }

    block = (struct memblock *)memptr;
    nbytes = (ulong)roundmb(nbytes);

    /* TODO:
     *      - Disable interrupts
     *      - Find where the memory block should
     *        go back onto the freelist (based on address)
     *      - Find top of previous memblock
     *      - Make sure block is not overlapping on prev or next blocks
     *      - Coalesce with previous block if adjacent
     *      - Coalesce with next block if adjacent
     *      - Restore interrupts
     */

    // Disable interrupts
    irqmask im = disable();

    prev = NULL;
    next = freelist.head;
    while (next != NULL && ((prev != NULL && block < prev) || block > next))
    {
        prev = next;
        next = next->next;
    }
    // Check adjacency to prev
    if (prev != NULL)
    {
        memblk *prevtop = prev + (prev->length / sizeof(memblk));
        // check if overlap
        if (prevtop > block)
        {
            return SYSERR;
        }
        // check if adjacent
        else if (prevtop == block)
        {
            prev->length = prev->length + nbytes;
            block = prev;
        }
        // not adjacent
        else
        {
            prev->next = block;
            block->length = nbytes;
        }
    }
    else
    {
        block->length = nbytes;
        freelist.head = block;
    }
    // Check adjacency to next
    if (next != NULL)
    {
        memblk *blocktop = block + (block->length / sizeof(memblk));
        // check if overlap
        if (blocktop > next)
        {
            return SYSERR;
        }
        // check if adjacent
        else if (blocktop == next)
        {
            block->length = block->length + next->length;
            block->next = next->next;
        }
        // not adjacent
        else
        {
            block->next = next;
        }
    }
    else
    {
        block->next = NULL;
    }

    freelist.size = freelist.size + nbytes;

    // Restore interrupts
    restore(im);

    return OK;
}

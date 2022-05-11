

#include <xinu.h>

/**
 * Allocate heap memory.
 *
 * @param nbytes
 *      Number of bytes requested.
 *
 * @return
 *      ::SYSERR if @p nbytes was 0 or there is no memory to satisfy the
 *      request; otherwise returns a pointer to the allocated memory region.
 *      The returned pointer is guaranteed to be 8-byte aligned.  Free the block
 *      with memfree() when done with it.
 */
void *getmem(ulong nbytes)
{
    register memblk *prev, *curr, *leftover;

    if (0 == nbytes)
    {
        return (void *)SYSERR;
    }

    /* round to multiple of memblock size   */
    nbytes = (ulong)roundmb(nbytes);

    /* TODO:
     *      - Disable interrupts
     *      - Traverse through the freelist
     *        to find a block that's suitable
     *        (Use First Fit with remainder splitting)
     *      - Restore interrupts
     *      - return memory address if successful
     */

    // Disable interrupts
    irqmask im = disable();
    // Traverse through the freelist to find a satisfactory block
    prev = NULL;
    curr = freelist.head;
    while (curr != NULL && curr->length < nbytes)
    {
        prev = curr;
        curr = curr->next;
    }
    // Error: ran through entire freelist without finding a satisfactory block.
    if (curr == NULL)
    {
        return (void *)SYSERR;
    }
    // Check if there is a leftover chunk
    if (curr->length - nbytes > 0)
    {
        // Calculate address of leftover chunk
        leftover = curr + (nbytes / sizeof(memblk));
        // Maintain the freelist data structure
        leftover->next = curr->next;
        leftover->length = curr->length - nbytes;
        // Handle if curr is the head of the freelist
        if (prev == NULL)
        {
            freelist.head = leftover;
        }
        else
        {
            prev->next = leftover;
        }
    }
    else
    {
        // Handle if curr is the head of the freelist
        if (prev == NULL)
        {
            freelist.head = curr->next;
        }
        else
        {
            prev->next = curr->next;
        }
    }
    // Update freelist size
    freelist.size = freelist.size - nbytes;

    // Restore interrupts
    restore(im);

    // Return address
    return curr;
}

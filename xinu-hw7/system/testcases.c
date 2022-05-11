/**
 * COSC 3250 - Project #7
 * Testcases
 * @author Thomas Florian Andi Hennessey
 * Instructor Dennis Brylow
 * TA-BOT:MAILTO thomas.florian@marquette.edu alexandra.hennessey@marquette.edu
 */

#include <xinu.h>

uint getTotalTickets(void);

syscall user_putc_1param(char c)
{
    return user_putc(0, c);
}

syscall user_printf(const char *format, ...)
{
    int retval;
    va_list ap;

    va_start(ap, format);
    retval = _doprnt(format, ap, (int (*)(int, int))user_putc_1param, 0);
    va_end(ap);
    return retval;
}

void printReadyList(void)
{
    int head = queuehead(readylist);
    int tail = queuetail(readylist);
    int pid = queuetab[head].next;
    user_printf("Ready List:\n");
    while (pid != tail)
    {
        user_printf("PID: %d NAME: %s TICKETS: %u\n", pid,
                    proctab[pid].name, proctab[pid].tickets);
        pid = queuetab[pid].next;
    }
    user_printf("\n");
}

void donothing(int argc, char **argv)
{
    return;
}

int testmain(int argc, char **argv)
{
    kprintf("Process %d started!\r\n", currpid);
    int i = 0;
    for (i = 0; i < 100; i++) {
        kprintf("Running process %d\r\n", currpid);
    }
    kprintf("Process %d finished!\r\n", currpid);
    return 0;
}

void printpcb(int pid)
{
    irqmask im = disable();
    pcb *ppcb = NULL;

    /* Using the process ID, access it in the PCB table. */
    ppcb = &proctab[pid];

    /* Printing PCB */
    kprintf("Process name		  : %s \r\n", ppcb->name);
    kprintf("Number of tickets	  : %u \r\n", ppcb->tickets);
    switch (ppcb->state)
    {
    case PRFREE:
        kprintf("State of the process	  : FREE \r\n");
        break;
    case PRCURR:
        kprintf("State of the process 	  : CURRENT \r\n");
        break;
    case PRSUSP:
        kprintf("State of the process	  : SUSPENDED \r\n");
        break;
    case PRREADY:
        kprintf("State of the process	  : READY \r\n");
        break;
    default:
        kprintf("ERROR: Process state not correctly set!\r\n");
        break;
    }

    /* Print PCB contents and registers */
    kprintf("Base of run time stack    : 0x%08X \r\n", ppcb->stkbase);
    kprintf("Stack length of process   : %8u \r\n\n", ppcb->stklen);
    restore(im);

}

void printpcb_curr(void) {
    printpcb(currpid);
}

/**
 * testcases - called after initialization completes to test things.
 */
void printFreeList(void)
{
    memblk *node = freelist.head;

    while(node != NULL)
    {
        kprintf("node: %d length: %10d next: %d\n", node, node->length, node->next);
        node = node->next;
    }
}
void testcases(void)
{
    int c;
    user_printf("1) Print freelist\r\n");
    user_printf("2) getmem 2 chunks and then freemem them.\r\n");
    user_printf("3) getmem 2 chunks and then freemem them in reverse order.\r\n");
    user_printf("4) malloc 2 chunks and then free them.\r\n");
    user_printf("5) malloc 2 chunks and then free them in reverse order.\r\n");
    user_printf("6) malloc until run out of memory\r\n");
    user_printf("7) Create 3 processes and check freelist\r\n");
    user_printf("8) Free memory that wasn't allocated\r\n");
    user_printf("9) malloc 5 chucks and free them.\r\n");

    user_printf("===TEST BEGIN===\r\n");

    // TODO: Test your operating system!
    int *addy, *addy1, *addy2;
    irqmask im;
    c = kgetc();
    switch (c) {
    case '1':
        printFreeList();
        break;
    case '2':
        kprintf("Initial freelist:\n");
        printFreeList();
        addy1 = getmem(16);
        kprintf("\nAfter 16 byte allocation:\n");
        printFreeList();
        addy2 = getmem(16);
        kprintf("\nAfter another 16 byte allocation:\n");
        printFreeList();
        kprintf("\nFree initial 16 byte allocation:\n");
        freemem(addy1, 16);
        printFreeList();
        kprintf("\nFree second 16 byte allocation:\n");
        freemem(addy2, 16);
        printFreeList();
        break;
     case '3':
        kprintf("Initial freelist:\n");
        printFreeList();
        addy1 = getmem(16);
        kprintf("\nAfter 16 byte allocation:\n");
        printFreeList();
        addy2 = getmem(16);
        kprintf("\nAfter another 16 byte allocation:\n");
        printFreeList();
        kprintf("\nFree second 16 byte allocation:\n");
        freemem(addy2, 16);
        printFreeList();
        kprintf("\nFree initial 16 byte allocation:\n");
        freemem(addy1, 16);
        printFreeList();
        break;
    case '4':
        kprintf("Initial freelist:\n");
        printFreeList();
        addy1 = malloc(16);
        kprintf("\nAfter 16 byte allocation:\n");
        printFreeList();
        addy2 = malloc(16);
        kprintf("\nAfter another 16 byte allocation:\n");
        printFreeList();
        kprintf("\nFree initial 16 byte allocation:\n");
        free(addy1);
        printFreeList();
        kprintf("\nFree second 16 byte allocation:\n");
        free(addy2);
        printFreeList();
        break;
     case '5':
        kprintf("Initial freelist:\n");
        printFreeList();
        addy1 = malloc(16);
        kprintf("\nAfter 16 byte allocation:\n");
        printFreeList();
        addy2 = malloc(16);
        kprintf("\nAfter another 16 byte allocation:\n");
        printFreeList();
        kprintf("\nFree second 16 byte allocation:\n");
        free(addy2);
        printFreeList();
        kprintf("\nFree initial 16 byte allocation:\n");
        free(addy1);
        printFreeList();
        break;
    case '6':
        while(1) {
            addy = malloc(1e7);
            if (addy == NULL) {
                kprintf("Malloc returned NULL :(\n");
                break;
            }
            kprintf("Malloc returned address 0x%X\n");
        }
        break;

    case '7':
        // Create three process that test preemption
        im = disable();
        user_printf("\nBEFORE PROCESS CREATION:\n");
        printFreeList();
        ready(create((void *)testmain, INITSTK, 1, "MAIN1", 2, 0, NULL),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, 1, "MAIN2", 2, 0, NULL),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, 1, "MAIN3", 2, 0, NULL),
              RESCHED_NO);
        user_printf("\nAFTER PROCESS CREATION:\n");
        printFreeList();
        restore(im);
        user_yield();
        user_printf("\nAFTER PROCESS FINISH:\n");
        printFreeList();
        break;
    
    case '8':
        if (free((int *)0x80001000) != SYSERR) {
            user_printf("ERROR. FREED UNALLOCATED MEMORY!\n");
        }
        else if (free((int *)0x81000000) != SYSERR) {
            user_printf("ERROR. FREED UNALLOCATED MEMORY!\n");
        } else {
            user_printf("PASS: DID NOT FREE UNALLOCATED MEMORY!\n");
        }
        break;
    case '9':
        kprintf("Initial freelist:\n");
        printFreeList();
        addy = malloc(16);
        malloc(16);
        addy1 = malloc(16);
        addy2 = malloc(16);
        malloc(16);
        kprintf("\nAfter 5 16 byte allocation:\n");
        printFreeList();
        kprintf("\nFree middle 16 byte allocation:\n");
        free(addy1);
        printFreeList();
        kprintf("\nFree 1 adjacent 16 byte allocations to middle:\n");
        free(addy2);
        printFreeList();
        kprintf("\nFree 1 non-adjacent 16 byte allocations to middle:\n");
        free(addy);
        printFreeList();
        break;

    default:
        break;
    }

    kprintf("\r\n===TEST END=== \r\n");
    return;
}

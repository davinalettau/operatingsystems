/**
 * @file testcases.c
 * @provides testcases
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

/**
* COSC 3250 - Project 6
* Explain briefly the functionality of the program.
* @author Davina Lettau Daniel Muneiz
* Instructor Dr.Brylow
* TA-BOT:MAILTO davina.lettau@marquette.edu daniel.muniz@marquette.edu
*/
 
#include <xinu.h>
 
extern void main(int, char *);
 
int testmain(int n)
{
    int i = 0;
    for (i = 0; i < n; i++)
    {
        kprintf("This is process %d\r\n", currpid);
     }
    return 0;
}
 
void testbigargs(int a, int b, int c, int d, int e, int f, int g, int h)
{
    kprintf("Testing bigargs...\r\n");
    kprintf("a = 0x%08X\r\n", a);
    kprintf("b = 0x%08X\r\n", b);
    kprintf("c = 0x%08X\r\n", c);
    kprintf("d = 0x%08X\r\n", d);
    kprintf("e = 0x%08X\r\n", e);
    kprintf("f = 0x%08X\r\n", f);
    kprintf("g = 0x%08X\r\n", g);
    kprintf("h = 0x%08X\r\n", h);
}
 
void printpcb(int pid)
{
    pcb *ppcb = NULL;
 
    /* Using the process ID, access it in the PCB table. */
    ppcb = &proctab[pid];
 
    /* Printing PCB */
    kprintf("Process name         : %s \r\n", ppcb->name);
 
    switch (ppcb->state)
    {
    case PRFREE:
        kprintf("State of the process     : FREE \r\n");
        break;
    case PRCURR:
        kprintf("State of the process     : CURRENT \r\n");
        break;
    case PRSUSP:
        kprintf("State of the process     : SUSPENDED \r\n");
        break;
    case PRREADY:
        kprintf("State of the process     : READY \r\n");
        break;
    default:
        kprintf("ERROR: Process state not correctly set!\r\n");
        break;
    }
 
    /* Print PCB contents and registers */
    kprintf("Base of run time stack    : 0x%08X \r\n", ppcb->stkbase);
    kprintf("Stack length of process   : %8u \r\n", ppcb->stklen);
}
 
/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c;
 
    kprintf("1) Create 2 processes and get total tickets\r\n");
    kprintf("2) Test priority\r\n");
    kprintf("P) Create three processes test preemption\r\n");
 
    kprintf("===TEST BEGIN===\r\n");
 
    // TODO: Test your operating system!
 
    irqmask im;
    c = kgetc();
    switch (c)
    {
    case '1':
        // should print 16, 10 for proc 1, 5 for proc 2, and 1 for null proc
        im = disable();
        ready(create((void *)testmain, INITSTK, 10, "MAIN1", 1, 0),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, 5, "MAIN2", 1, 0),
              RESCHED_NO);
        kprintf("Total Tickets: %u", getTotalTickets());
        restore(im);
        break;
 
    case '2':
        // Should run proc 3, then proc 2, then proc 1
        im = disable();
        ready(create((void *)testmain, INITSTK, 10, "MAIN1", 1, 1),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, 100, "MAIN2", 1, 1),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, 1000, "MAIN3", 1, 1),
              RESCHED_NO);
        restore(im);
        break;

    case 'P':
        // should switch between them.
        ready(create((void *)testmain, INITSTK, 10, "MAIN1", 1, 100),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, 10, "MAIN2", 1, 100),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, 10, "MAIN3", 1, 100),
              RESCHED_NO);
        break;
 
    default:
        break;
    }
 
    kprintf("\r\n===TEST END===\r\n");
    return;
}
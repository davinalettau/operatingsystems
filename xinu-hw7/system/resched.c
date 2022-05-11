/**
 * @file resched.c
 * @provides resched
 *
 * COSC 3250 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

/**
* COSC 3250 - Project 6
* Explain briefly the functionality of the program.
* @author Davina Lettau Daniel Muneiz
* Instructor Dr.Brylow
* TA-BOT:MAILTO davina.lettau@marquette.edu daniel.muniz@marquette.edu
*/

#include <xinu.h>

extern void ctxsw(void *, void *);
/**
 * Reschedule processor to next ready process.
 * Upon entry, currpid gives current process id.  Proctab[currpid].pstate 
 * gives correct NEXT state for current process if other than PRREADY.
 * @return OK when the process is context switched back
 */

uint getTotalTickets(void) {
    uint total = 0;
    int pid;
    for (pid = 0; pid < NPROC; pid++) {
        if (proctab[pid].state == PRREADY || proctab[pid].state == PRCURR) {
            total += proctab[pid].tickets;
        }
    }
    return total;
}

syscall resched(void)
{
    irqmask im;
    im = disable();
    pcb *oldproc;               /* pointer to old process entry */
    pcb *newproc;               /* pointer to new process entry */

    oldproc = &proctab[currpid];

    /* place current process at end of ready queue */
    if (PRCURR == oldproc->state)
    {
        oldproc->state = PRREADY;
        enqueue(currpid, readylist);
    }

    /**
     * We recommend you use a helper function for the following:
     * TODO: Get the total number of tickets from all processes that are in current and ready states.
     * Utilize the random() function to pick a random ticket. 
     * Traverse through the process table to identify which proccess has the random ticket value.
     * Remove process from queue.
     * Set currpid to the new process.
     */

    uint counter = 0;
    uint totalTickets = getTotalTickets();
    uint winningTicket = random(totalTickets);
   
    int pid;
    for (pid = 0; pid < NPROC; pid++) {
        if (proctab[pid].state == PRREADY || proctab[pid].state == PRCURR) {
            counter += proctab[pid].tickets;
            if (counter > winningTicket) {
                currpid = pid;
                break;
            }
        }
    }
    remove(currpid);
    newproc = &proctab[currpid];
    newproc->state = PRCURR;    /* mark it currently running    */

#if PREEMPT
    preempt = QUANTUM;
#endif

    ctxsw(&oldproc->stkptr, &newproc->stkptr);
    restore(im);


    /* The OLD process returns here when resumed. */
    return OK;
}

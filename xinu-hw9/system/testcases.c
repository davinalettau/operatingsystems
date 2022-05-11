/**
* COSC 3250 - Project 6
* Explain briefly the functionality of the program.
* @author Davina Lettau Daniel Muneiz
* Instructor Dr.Brylow
* TA-BOT:MAILTO davina.lettau@marquette.edu daniel.muniz@marquette.edu
*/

#include <xinu.h>

void testcases(void)
{
    int c;
    kprintf("1) Test printf\r\n");
    kprintf("===TEST BEGIN===\r\n");

    // TODO: Test your operating system!
    irqmask im;
    c = kgetc();
    switch (c) {
    case '1':
        printf("testing printf\n");
        break;

    default:
        break;
    }

    kprintf("\r\n===TEST END=== \r\n");
    return;
}

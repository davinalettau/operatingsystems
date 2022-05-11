/**
 * @file kprintf.c
 */

/* Embedded Xinu, Copyright (C) 2009, 2022.  All rights reserved. */

#include <xinu.h>

/**
 * Synchronously read a character from a UART.  This blocks until a character is
 * available.  The interrupt handler is not used.
 *
 * @return
 *      The character read from the UART as an int. When calling this function, do NOT try to store
 *      this value in a char, as the EOF value is -1 under the hood.
 *      If ASCII value 4 is seen, return special constant EOF instead.
 */
syscall kgetc(void)
{
    volatile struct pl011_uart_csreg *regptr;
    /* Pointer to the UART control and status registers.  */
    regptr = (struct pl011_uart_csreg *)0x3F201000;
    /*  Get the flags register (fr) from the UART struct and check the PL011_FR_RXFE flag.
        The PL011_FR_RXFE indicates that the UART receive register is empty.
        If it is empty, wait in an infinite while loop until it is full and ready to give the next character */
    while (regptr->fr & PL011_FR_RXFE)
        ;
    // Get the character from the data register.
    int c = regptr->dr;
    // If ASCII value 4 is seen, return special constant EOF instead.
    if (c != 4)
        return c;
    else
        return EOF;
}

/**
 * kcheckc - check to see if a character is available.
 * @return true if a character is available, false otherwise.
 */
syscall kcheckc(void)
{
    volatile struct pl011_uart_csreg *regptr;
    regptr = (struct pl011_uart_csreg *)0x3F201000;

    /*  Get the flags register (fr) from the UART struct and check the PL011_FR_RXFF flag.
        The PL011_FR_RXFF indicates that the UART receive register is full.
        If it is full, it is ready and return true. If it is empty, it is not ready and return false */
    if (regptr->fr & PL011_FR_RXFF)
        return 1;
    else
        return 0;
}

/**
 * Synchronously write a character to a UART.  This blocks until the character
 * has been written to the hardware.  The interrupt handler is not used.
 *
 * @param c
 *      The character to write.
 *
 * @return
 *      The character written to the UART.
 */
syscall kputc(uchar c)
{
    volatile struct pl011_uart_csreg *regptr;

    /* Pointer to the UART control and status registers.  */
    regptr = (struct pl011_uart_csreg *)0x3F201000;

    /*  Get the flags register (fr) from the UART struct and check the PL011_FR_TXFF flag.
       The PL011_FR_TXFF indicates that the UART transmit register is full.
       If it is full, wait in an infinite while loop until it is empty and ready to receive the next character */
    while (regptr->fr & PL011_FR_TXFF)
        ;
    if (c != '\n')
    {
        // Send the character to the data register.
        regptr->dr = c;
    }
    else
    {
        // Send carriage return to the data register, wait, then send the new line.
        regptr->dr = '\r';
        while (regptr->fr & PL011_FR_TXFF)
            ;
        regptr->dr = '\n';
    }

    return c;
}

/**
 * kernel printf: formatted, synchronous output to SERIAL0.
 *
 * @param format
 *      The format string.  Not all standard format specifiers are supported by
 *      this implementation.  See _doprnt() for a description of supported
 *      conversion specifications.
 * @param ...
 *      Arguments matching those in the format string.
 *
 * @return
 *      The number of characters written.
 */
syscall kprintf(const char *format, ...)
{
    int retval;
    va_list ap;

    va_start(ap, format);
    retval = _doprnt(format, ap, (int (*)(int, int))kputc, 0);
    va_end(ap);
    return retval;
}
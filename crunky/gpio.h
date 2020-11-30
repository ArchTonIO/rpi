#pragma once
/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include <basal.h>

/**
 modes for the pins
to use in gpio_sel in table 6-5 of BCM2835 datasheet
*/

#define INPUT  0b000
#define OUTPUT 0b001
#define ALTFN0 0b100 // alternative function 0
#define ALTFN1 0b101 // alternative function 1
#define ALTFN2 0b110 // alternative function 2
#define ALTFN3 0b111 // alternative function 3
#define ALTFN4 0b011 // alternative function 4
#define ALTFN5 0b010 // alternative function 5


/**
gpio_sel:
For mode, use one of INPUT, OUTPUT, ALTFNn (for n =0,1,..5)
*/

void gpio_clr(int bcm_pin);
int  gpio_get(int bcm_oin);
void gpio_sel(int bcm_pin, int mode);
void gpio_set(int bcm_pin);
void gpio_toggle(int bcm_pin);


// some of this is userd by sd.c
#define GPFSEL0         REG(PBASE+0x00200000)
#define GPFSEL1         REG(PBASE+0x00200004)
#define GPFSEL2         REG(PBASE+0x00200008)
#define GPFSEL3         REG(PBASE+0x0020000C)
#define GPFSEL4         REG(PBASE+0x00200010)
#define GPFSEL5         REG(PBASE+0x00200014)
#define GPSET0          REG(PBASE+0x0020001C)
#define GPSET1          REG(PBASE+0x00200020)
#define GPCLR0          REG(PBASE+0x00200028)
#define GPLEV0          REG(PBASE+0x00200034)
#define GPLEV1          REG(PBASE+0x00200038)
#define GPEDS0          REG(PBASE+0x00200040)
#define GPEDS1          REG(PBASE+0x00200044)
#define GPHEN0          REG(PBASE+0x00200064)
#define GPHEN1          REG(PBASE+0x00200068)
#define GPPUD           REG(PBASE+0x00200094)
#define GPPUDCLK0       REG(PBASE+0x00200098)
#define GPPUDCLK1       REG(PBASE+0x0020009C)

#define MBOX_REQUEST    0

/* channels */
#define MBOX_CH_POWER   0
#define MBOX_CH_FB      1
#define MBOX_CH_VUART   2
#define MBOX_CH_VCHIQ   3
#define MBOX_CH_LEDS    4
#define MBOX_CH_BTNS    5
#define MBOX_CH_TOUCH   6
#define MBOX_CH_COUNT   7
#define MBOX_CH_PROP    8

/* tags */
#define MBOX_TAG_SETPOWER       0x28001
#define MBOX_TAG_SETCLKRATE     0x38002
#define MBOX_TAG_LAST           0

#define VIDEOCORE_MBOX  (PBASE+0x0000B880)
#define MBOX_READ       ((volatile unsigned int*)(VIDEOCORE_MBOX+0x0))
#define MBOX_POLL       ((volatile unsigned int*)(VIDEOCORE_MBOX+0x10))
#define MBOX_SENDER     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x14))
#define MBOX_STATUS     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x18))
#define MBOX_CONFIG     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x1C))
#define MBOX_WRITE      ((volatile unsigned int*)(VIDEOCORE_MBOX+0x20))
#define MBOX_RESPONSE   0x80000000
#define MBOX_FULL       0x80000000
#define MBOX_EMPTY      0x40000000

//int mbox_call(unsigned char ch, volatile unsigned int* mbox);

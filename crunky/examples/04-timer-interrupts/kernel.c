#include <basal.h>
#include <timers.h>
#include <gpio.h>
//#include <mini_uart.h>
#include <interrupts.h>



// Per BCM2837 s14.2  p196 for Pi3
#define ARM_TIMER_LOD REG(0x3F00B400)
#define ARM_TIMER_VAL REG(0x3F00B404)
#define ARM_TIMER_CTL REG(0x3F00B408)
#define ARM_TIMER_CLI REG(0x3F00B40C)
#define ARM_TIMER_RIS REG(0x3F00B410)
#define ARM_TIMER_MIS REG(0x3F00B414)
#define ARM_TIMER_RLD REG(0x3F00B418)
#define ARM_TIMER_DIV REG(0x3F00B41C)
#define ARM_TIMER_CNT REG(0x3F00B420)

#define SYSTIMERCLO REG(0x3F003004)




#define TIME_INT 1000000        		// in microsec

#if 0
const int pin = 19;
#else
const int pin = 26;
#endif



void blink_led()
{
	gpio_toggle(pin);
	ARM_TIMER_CLI = 0; // clear the timer interrupt
}

void kernel_main ( void )
{
	gpio_sel(pin, OUTPUT);

	ARM_TIMER_CTL = 0x003E0000;	// 0x3E is the reset for the counter
	ARM_TIMER_LOD = TIME_INT-1;	// 1000000 is equal to 1 second
	ARM_TIMER_RLD = TIME_INT-1;	// RLD is copied tO LOD when it reaches 0
	ARM_TIMER_DIV = 0x000000F9;	// dividing APB_CLK by 0xF9 + 1 (250) -> 1 MHz (this signal is timer_clk)
	ARM_TIMER_CLI = 0;				// writing here clears the interrupt (write only)
	ARM_TIMER_CTL = 0x003E00A2;	// 23bit counting mode, no timer_clk prescaling, enabling interrupts and the timer
	IRQ_ENABLE_BASIC = 1;			// enabling interrupts

	set_irq_handler(blink_led);
	enable_irq();
}

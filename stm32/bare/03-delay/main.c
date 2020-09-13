#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "../blue.h"


// see video https://youtu.be/o6ZWD0PAoJk

void system_clock_init()
{
	RCC_CR |= RCC_CR_HSION;
	RCC_CFGR &= ~RCC_CFGR_SW;
	while((RCC_CR && RCC_CR_HSIRDY) == 0);
	//RCC_CR &= ~RCC_CR_MSI_RANGE;
	//RCC_CR != RCC_CR_MSI_RANGE;

	//RCC_CR |= RCC_CR_HSIROSEL;
	while((RCC_CR & RCC_CR_HSIRDY) == 0);
}



unsigned int _div(unsigned int a, unsigned int b)
{
	return a/b;
}
unsigned int _mod(unsigned int a, unsigned int b)
{
	return a%b;
}
////struct {unsigned int quot; unsigned int rem} __aeabi_uidivmod(unsigned int a, unsigned int b)
void  __aeabi_uidivmod(unsigned int a, unsigned int b)
{
	putchar('U');
	unsigned int c = _div(a,  b);
	unsigned int d = _mod(a, b); /* Likely uses the result of the division. */
	//#pragma asm(@r0=c, @r1 = d)
	//#pragma endasm
	asm("mov r0, %0\t\n" : "=r"(c));
	asm("mov r1, %0\t\n" : "=r"(d));
	//return ; // TODO
}

// stub for newlib nano
extern uint32_t __ssystem_ram__;
void *_sbrk(int incr) {
	//static unsigned char *heap = HEAP_START;
	static unsigned char *heap = (unsigned char*) &__ssystem_ram__;
	putchar('S');
	unsigned char *prev_heap = heap;
	heap += incr;
	return prev_heap;
}
char greeting[] = "Hello from bare metal usart 1";

void nop()
{

}
uint32_t rtc_cnt()
{
	// The clock, although 32-bit, is implemented in 2 32-bit registers with the lower half of the word containing low and high values

	// TODO buggy because of potential timer overflow during middle of computation
	//static int i = 0;
	uint32_t hi = 0;
	//hi = RTC_CNTH;
	uint32_t lo = 0; 
	//lo = RTC_CNTL;
	uint32_t ret = 0;
	ret = (hi<<16) | lo ;
	return ret;
}

void delay(uint16_t ms)
{
	/* According to https://bit.ly/2GXlmxd 
	 * you must define start and now as 32-bit integers rather than 16- or 8-,
	 * otherwise the bridge creates duplications
	 */
#if 1	
	uint32_t start = TIM4->CNT;
	while(1) {
		uint32_t now = TIM4->CNT;
		if(now-start>=ms) break;
	}
#else	
	TIM4->CNT =0;
	while(TIM4->CNT < ms);
#endif
}


void main() 
{
	system_clock_init();
	init_serial();


	// timer setup
	RCC_APB1ENR |= RCC_APB1ENR_TIM4EN;
	TIM4->PSC=7999;
	TIM4->ARR=65535;
	//TIM4->ARR=30000;
	//TIM4->ARR=10000;
	TIM4->CR1 |= TIM_CR1_CEN;

	puts("Test of delay 1");
	char life[40];
	itoa(42, life, 10);
	puts(life);
	//sprintf(life, "Meaning of life is %d", 42);

	// Main loop: wait for a new byte, then echo it back.
	char rxb = '\0';
	putchar('\a'); // beep
	puts(greeting);
	int secs = 0;
	while ( 1 ) {
		itoa(secs++, life, 10);
		puts(life);
		delay(1000);
	}
}

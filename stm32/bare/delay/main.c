#include <string.h>
#include <stddef.h> // for size_t
#include <stdio.h>
#include <stdbool.h>
#include "../blue.h"

extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;

// cribbed from blink sketch
#define RCC_BASE      	0x40021000
// reference page 51
//#define GPIOA_BASE    	0x40010800

struct TIMx_t;

#define TIM4	((TIMx_t*) 0x40000800)

#define RTC_BASE 0x40028000
#define RTC_CNTH *(volatile uint32_t *)(RTC_BASE   + 0x18)
#define RTC_CNTL *(volatile uint32_t *)(RTC_BASE   + 0x1C)

// section 7.3.11 RCC register map page 121
#define RCC_CR   *(volatile uint32_t *)(RCC_BASE + 0x00) 
#define RCC_CR_HSION (1<<0)
#define RCC_CR_HSIRDY (1<<1)
#define RCC_CFGR   *(volatile uint32_t *)(RCC_BASE + 0x04) 
#define RCC_CFGR_SW (1<<0)
#define RCC_APB1ENR   *(volatile uint32_t *)(RCC_BASE   + 0x1C) // page 148
#define RCC_APB1ENR_TIM4EN (1<<2)
#define RCC_APB1ENR_USART2EN	(1<<17)
#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE   + 0x18)
#define RCC_APB2ENR_IOPAEN	(1<<2)
#define GPIOA_CRL     *(volatile uint32_t *)(GPIOA_BASE + 0x00)

#define GPIO_CRL_CNF2_Pos 10 // page 171
#define GPIO_CRL_CNF3_Pos 14
#define GPIO_CRL_MODE2_Pos 8
#define GPIO_CRL_MODE3_Pos 12
#define GPIO_CRL_CNF2 (0x3<<GPIO_CRL_CNF2_Pos)
#define GPIO_CRL_CNF3 (0x3<<GPIO_CRL_CNF3_Pos)
#define GPIO_CRL_MODE2 (0x3<<GPIO_CRL_MODE2_Pos)
#define GPIO_CRL_MODE3 (0x3<<GPIO_CRL_MODE3_Pos)
#define USART_BRR_DIV_Fraction_Pos 0 // page 820
#define USART_BRR_DIV_Mantissa_Pos 4
#define USART_CR1_RE (1<<2) // page 821
#define USART_CR1_TE (1<<3)
#define USART_CR1_UE (1<<13)
#define USART_SR_RXNE (1 << 5) // page 818
#define USART_SR_TXE (1 << 7)

// section 15.4.18 TIMx register map page 423
typedef struct {
	__IO uint32_t CR1; // 0x00
	__IO uint32_t CR2; // 0x04
	__IO uint32_t SMCR; // 0x08
	__IO uint32_t DIER; // 0x0C
	__IO uint32_t SR; // 0x10
	__IO uint32_t EGR; // 0x14
	__IO uint32_t CCMR1; // 0x18
	__IO uint32_t CCMR2; // 0x1C
	__IO uint32_t CCER; // 0x20
	__IO uint32_t CNT; // 0x24
	__IO uint32_t PSC; // 0x28
	__IO uint32_t ARR; // 0x2C
	// ... more ... incomplete
} TIMx_t;

#define TIM_CR1_CEN (1<<0)

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
// USART register map: page 827
typedef struct {
	__IO uint32_t SR; // 0x00
	__IO uint32_t DR; // 0x04
	__IO uint32_t BRR; // 0x08
	__IO uint32_t CR1; // 0x0C
	__IO uint32_t CR2; // 0x10
	__IO uint32_t CR3; // 0x14
	__IO uint32_t GTPT; // 0x18
} USART_t;

#define USART2	((USART_t*) 0x40004400)


// stub for newlib nano - apparently required by nano-vfprintf_i

void putc2(char c)
{
	while( !( USART2->SR & USART_SR_TXE ) ) {};
	USART2->DR = c;
}

void puts2(const char* s)
{
	while(s && *s) putc2(*s++);
	putc2('\r');
	putc2('\n');
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
	putc2('U');
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
	putc2('S');
	unsigned char *prev_heap = heap;
	heap += incr;
	return prev_heap;
}
char greeting[] = "Hello from bare metal usart 9";

void* memcpy_usr(void* dst, const void* src, size_t n) {
	// Copies n bytes from src to dst

	// Since we cannot dereference a void* ptr,
	// we first typecast it to a char* ptr
	// and then do the copying byte by byte,
	// since a char* ptr references a single byte
	char* char_dst = (char*) dst;
	char* char_src = (char*) src;

	for (int i=0; i<n; i++) {
		*char_dst++ = *char_src++;
	}

	return dst;
}

void *memset_usr(void *b, int c, int len)
{
	int           i;
	unsigned char *p = b;
	i = 0;
	while(len > 0)
	{
		*p = c;
		p++;
		len--;
	}
	return(b);
}

void init_mem()
{
	// not called explicitly, but done in crt.s prior to calling main()

	// Copy initialized data from .sidata (Flash) to .data (RAM)
	memcpy_usr( &_sdata, &_sidata, ( ( void* )&_edata - ( void* )&_sdata ) );
	// Clear the .bss section in RAM.
	memset_usr( &_sbss, 0x00, ( ( void* )&_ebss - ( void* )&_sbss ) );
}

/* reverse() and itoa() from https://www.geeksforgeeks.org/implement-itoa/
*/


/* A utility function to reverse a string  */
void reverse(char str[], int length) 
{ 
	int start = 0; 
	int end = length -1; 
	while (start < end) 
	{ 
		//swap(*(str+start), *(str+end)); 
		int c = *(str+start);
		*(str+start) = *(str+end);
		*(str+end) = c;

		start++; 
		end--; 
	} 
} 

// Implementation of itoa() 
char* itoa(int num, char* str, int base) 
{ 
	int i = 0; 
	bool isNegative = false; 

	/* Handle 0 explicitely, otherwise empty string is printed for 0 */
	if (num == 0) 
	{ 
		str[i++] = '0'; 
		str[i] = '\0'; 
		return str; 
	} 

	// In standard itoa(), negative numbers are handled only with  
	// base 10. Otherwise numbers are considered unsigned. 
	if (num < 0 && base == 10) 
	{ 
		isNegative = true; 
		num = -num; 
	} 

	// Process individual digits 
	while (num != 0) 
	{ 
		int rem = num % base; 
		str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0'; 
		num = num/base; 
	} 

	// If number is negative, append '-' 
	if (isNegative) 
		str[i++] = '-'; 

	str[i] = '\0'; // Append string terminator 

	// Reverse the string 
	reverse(str, i); 

	return str; 
} 

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

	// Set the core system clock speed.
	// Default clock source is the 8MHz internal oscillator.
	uint32_t SystemCoreClock = 8000000;

	// Enable peripheral clocks and set up GPIO pins.
	// Enable peripheral clocks: GPIOA, USART2.
	RCC_APB1ENR  |=  ( RCC_APB1ENR_USART2EN );
	RCC_APB2ENR  |=  ( RCC_APB2ENR_IOPAEN );
	// Configure pins A2, A3 for USART2.
	GPIOA->CRL    &= ~( GPIO_CRL_MODE2 |
			GPIO_CRL_CNF2 |
			GPIO_CRL_MODE3 |
			GPIO_CRL_CNF3 );
	GPIOA->CRL    |= ( ( 0x1 << GPIO_CRL_MODE2_Pos ) |
			( 0x2 << GPIO_CRL_CNF2_Pos ) |
			( 0x0 << GPIO_CRL_MODE3_Pos ) |
			( 0x1 << GPIO_CRL_CNF3_Pos ) );

	// Set the baud rate
	uint32_t baud_rate = 115200;
	uint16_t uartdiv = SystemCoreClock / baud_rate;
	USART2->BRR = ( ( ( uartdiv / 16 ) << USART_BRR_DIV_Mantissa_Pos ) |
			( ( uartdiv % 16 ) << USART_BRR_DIV_Fraction_Pos ) );

	// Enable the USART peripheral.
	USART2->CR1 |= ( USART_CR1_RE | USART_CR1_TE | USART_CR1_UE );

	// timer setup
	RCC_APB1ENR |= RCC_APB1ENR_TIM4EN;
	TIM4->PSC=7999;
	TIM4->ARR=65535;
	//TIM4->ARR=30000;
	//TIM4->ARR=10000;
	TIM4->CR1 |= TIM_CR1_CEN;

	char life[40];
	itoa(42, life, 10);
	puts2(life);
	//sprintf(life, "Meaning of life is %d", 42);

	// Main loop: wait for a new byte, then echo it back.
	char rxb = '\0';
	putc2('\a'); // beep
	puts2(greeting);
	int secs = 0;
	while ( 1 ) {
		itoa(secs++, life, 10);
		puts2(life);
		delay(1000);
	}
}

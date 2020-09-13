#include "blue.h"

void init_serial()
{

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
}

int getchar()
{
	while( !( USART2->SR & USART_SR_RXNE ) ) {};
	return USART2->DR;
}


int putchar(int c)
{
	while( !( USART2->SR & USART_SR_TXE ) ) {};
	USART2->DR = c;
	return c;
}


int puts(const char* s)
{
	while(s && *s) putchar(*s++);
	putchar('\r');
	putchar('\n');
	return 0;
}


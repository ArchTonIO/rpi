#pragma once

#include <stdint.h>
#include <stddef.h> // for size_t


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define __IO volatile

#define REG(x) *(volatile uint32_t *)(x)

extern u32  SystemCoreClock;

#define RCC_BASE      	0x40021000


#define RTC_BASE 0x40002800
#define RTC_CNTH *(volatile uint32_t *)(RTC_BASE   + 0x18)
#define RTC_CNTL *(volatile uint32_t *)(RTC_BASE   + 0x1C)



// section 3.3 Memory map page 50

// forward declarations
struct GPIO_t;
struct USART_t;

#define GPIO_BASE 0x40010800

#define USART1 ((USART_t*) 0x40013800)

#define NVIC_ISER0 *(volatile uint32_t *)(0xE000E000+0x100)


#define GPIOC ((GPIO_t*) (GPIO_BASE + 0x800))
#define GPIOB ((GPIO_t*) (GPIO_BASE + 0x400))
#define GPIOA ((GPIO_t*) (GPIO_BASE + 0x000))
#define USART5 ((USART_t*) 0x40005000)
#define USART4 ((USART_t*) 0x40004C00)
#define USART3 ((USART_t*) 0x40004800)
#define USART2 ((USART_t*) 0x40004400)

#define RCC_APB2ENR   	REG(RCC_BASE + 0x18)
#define RCC_APB1ENR   	REG(RCC_BASE + 0x1C) // page 148
#define RCC_CR   	REG(RCC_BASE + 0x00) 
#define RCC_CFGR   	REG(RCC_BASE + 0x04) 

// section 7.3.11 RCC register map page 121
#define RCC_CR_HSION (1<<0)
#define RCC_CR_HSIRDY (1<<1)
#define RCC_CR_PLLON (1<<24)
#define RCC_CR_PLLRDY (1<<25)

#define RCC_CFGR_SW (1<<0)
#define RCC_CFGR_PLLMUL 18

#define RCC_APB1ENR_TIM3EN (1<<1)
#define RCC_APB1ENR_TIM4EN (1<<2)
#define RCC_APB1ENR_USART2EN	(1<<17)

#define RCC_APB2ENR_USART1EN (1<<14)
#define RCC_APB2ENR_IOPAEN	(1<<2)
#define RCC_APB2ENR_AFIOEN (1<<0)
#define RCC_APB2ENR_SPI1EN (1<<12)

#define RCC_APB2ENR_AFIOEN (1<<0)

#define RCC_APB1ENR_I2C1EN (1<<21)
#define RCC_AHBENR   REG(RCC_BASE   + 0x14)
#define RCC_AHBENR_DMA1EN (1<<0)


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
// page 194
typedef struct
{
	__IO uint32_t CRL; // 0x00
	__IO uint32_t CRH; // 0x04
	__IO uint32_t IDR; // 0x08
	__IO uint32_t ODR; // 0x0C
	__IO uint32_t BSRR; // 0x10
	__IO uint32_t BRR; // 0x14
	__IO uint32_t LCKR; //0x18
} GPIO_t;





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

void put32(u32 addr, u32 val);
u32 get32(u32 addr);
#define disable_irq() asm("CPSID I")
#define enable_irq() asm("CPSIE I")
#define nop() asm volatile ("nop")
void* memcpy(void* dst, const void* src, size_t n);
void *memset(void *b, int c, int len);
void delayish(u32 ms);
void delaysecsish(u32 secs);

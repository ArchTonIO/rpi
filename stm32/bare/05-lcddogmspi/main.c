//#include <blue.h>
//#include <stdio.h>
#include <stdbool.h>

#include <gpio.h>

#define HIGH 1
#define LOW 0

u32 rs_pin = PB0;
u32 cs_pin = PA4;
void delay(uint16_t ms)
{
	static bool initialised = false;
	if(!initialised) {
		// timer setup
		RCC_APB1ENR |= RCC_APB1ENR_TIM4EN;
		TIM4->PSC=7999;
		TIM4->ARR=65535;
		TIM4->CR1 |= TIM_CR1_CEN;
		initialised = true;
	}

	/* According to https://bit.ly/2GXlmxd 
	 * you must define start and now as 32-bit integers rather 
	 * than 16- or 8-,
	 * otherwise the bridge creates duplications
	 */
	uint32_t start = TIM4->CNT;
	while(1) {
		uint32_t now = TIM4->CNT;
		if(now-start>=ms) break;
	}
}

#define SPI_SR_TXE (1<<1)
#define SPI_SR_BSY (1<<7)


/* rs_val = LOW to send command, HIGH to send ASCII char 
*/
void sendByte(int rs_val, int val) {
	gpio_write(rs_pin, rs_val);
	gpio_write(cs_pin, LOW);
	
	// transmit sequence on page 712, section 25.3.5
	SPI1->DR = val;
	while((SPI1->SR & SPI_SR_TXE) != 1); //wait until TXE=1
	//while(SPI1->SR & SPI_SR_BSY); //wait until BSY=0
	//(void)SPI1->DR;

	gpio_write(cs_pin, HIGH);  
	delay(60);
}

#define RCC_APB2ENR_SPI1EN (1<<12)
#define SPI_CR1_SPE (1<<6)
#define SPI_CR1_SSM (1<<9)
#define SPI_CR1_MSTR (1<<2)
#define SPI_CR2_SSOE (1<<2)

void main() {
	gpio_mode_out(cs_pin);
	gpio_write(cs_pin, 1);
	gpio_mode_out(rs_pin);

	// setup SPI as master, transmit only
	RCC_APB2ENR |= (1<<2); // enable port A, where our SPI is
	RCC_APB2ENR |= RCC_APB2ENR_SPI1EN; // enable SPI1
	SPI1->CR1 |= SPI_CR1_SSM // we'll manage CS pin ourselves
		| SPI_CR1_MSTR // master mode
		| (0b100 <<3) // Baud rate. Just a guess for now
		;
	SPI1->CR2 |= SPI_CR2_SSOE; // some bizarre output enabling
	SPI1->CR1 |= SPI_CR1_SPE; // enable SPI

	u8 contrast = 0x70  | 0b1000; // from 0x7C
	u8 display = 0b1111; // ori 0x0F
	u8 cmds[] = {0x39, 0x1D, 0x50, 0x6C, contrast , 0x38, display, 0x01, 0x06};
	for(int i=0; i<sizeof(cmds); ++i) sendByte(LOW, cmds[i]);
	//Serial.println(sizeof(cmds));

	// now send some intersting output
	uint8_t msg[] = {'S', 'T', 'M', '3', '2'};
	for(int i=0; i<sizeof(msg); ++i) sendByte(HIGH, msg[i]);
	//Serial.println(sizeof(msg));

	while(1);
}




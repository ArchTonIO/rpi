// attempt to convert arduino library to bare metal

// I've made a right mess of this code


#include <stdbool.h>

#include <gpio.h>
#include <stm8.h>
#include <millis.h>
#include <i2c.h>


// /home/pi/.arduino15/packages/sduino/hardware/stm8/0.5.0/libraries/I2C
// /home/pi/.arduino15/packages/sduino/hardware/stm8/0.5.0/STM8S_StdPeriph_Driver/src


#if 1 
#define HEIGHT 32
#else
#define HEIGHT 64
#endif

#define WIDTH 128
#define SID 0x3C
const u8 pages = HEIGHT/8-1;

void check()
{
	if(I2C_SR2 & IC2_SR2_AF) digitalWrite(PA2, 1);
	if(I2C_SR2 & IC2_SR2_BERR) digitalWrite(PA1, 1);
}

void here()
{
	digitalWrite(PD6, 1); // check that we make it here
}

void write_i2c_byte_1(uint8_t dat)
{
	while (!(I2C_SR1 & I2C_SR1_TXE));
	//while (!(I2C_SR1 & I2C_SR1_BTF));
	I2C_DR = dat;
	//check();
}

void pause()
{
	for(u32 i = 0; i< 1000UL; i++) nop();
}

static void end_i2c_1()
{
	// seems to have problems
	//while(!(I2C_SR1 & I2C_SR1_BTF));
	//while(!(I2C_SR1 & I2C_SR1_TXE));

	//for(u32 i = 0; i< 1000UL; i++) nop();

	for(u8 i = 0; i< 100; i++) nop();
	//nop();
	//while(!(I2C_SR1 & I2C_SR1_TXE) || !(I2C_SR1 & I2C_SR1_BTF));
	//pause();
	//check();
	I2C_CR2 |= I2C_CR2_STOP;
	//while(I2C_CR2 & I2C_CR2_STOP);
}




static void begin_i2c_write(uint8_t slave_id)
{

	I2C_CR2 |= I2C_CR2_ACK;  // set ACK
	I2C_CR2 |= I2C_CR2_START;  // send start sequence
	while (!(I2C_SR1 & I2C_SR1_SB)); // EV5
	here();
	I2C_DR = (slave_id << 1); 

	// EV6 ADDR=1, cleared by reading SR1 register, then SR3
	while (!(I2C_SR1 & I2C_SR1_ADDR));
	I2C_SR3;   // read SR3 to clear ADDR event bit

}


void init_i2c_1() {
	uint32_t OutputClockFrequencyHz = I2C_MAX_STANDARD_FREQ;
	//Serial_println_u(I2C_MAX_STANDARD_FREQ);
	uint8_t InputClockFrequencyMHz = 2; // 16;
	InputClockFrequencyMHz  = 2;
	I2C_FREQR = InputClockFrequencyMHz;
	I2C_TRISER = InputClockFrequencyMHz + 1; // max rise time

	// set clock control frequency registers
	uint16_t speed = (uint16_t)((InputClockFrequencyMHz * 1000000) / (OutputClockFrequencyHz / 2));
	//if (speed < (uint16_t)0x0004) speed = (uint16_t)0x0004; // must be at least 4
	speed = 4;
	I2C_CCRL = (uint8_t)speed;
	I2C_CCRH = (uint8_t)(speed >> 8);


	I2C_CR1 |= I2C_CR1_PE; // enable I2C
}


#define SSD1306_BLACK 0   ///< Draw 'off' pixels
#define SSD1306_WHITE 1   ///< Draw 'on' pixels
#define SSD1306_INVERSE 2 ///< Invert pixels

#define SSD1306_MEMORYMODE 0x20          ///< See datasheet
#define SSD1306_COLUMNADDR 0x21          ///< See datasheet
#define SSD1306_PAGEADDR 0x22            ///< See datasheet
#define SSD1306_SETCONTRAST 0x81         ///< See datasheet
#define SSD1306_CHARGEPUMP 0x8D          ///< See datasheet
#define SSD1306_SEGREMAP 0xA0            ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON 0xA5        ///< Not currently used
#define SSD1306_NORMALDISPLAY 0xA6       ///< See datasheet
#define SSD1306_INVERTDISPLAY 0xA7       ///< See datasheet
#define SSD1306_SETMULTIPLEX 0xA8        ///< See datasheet
#define SSD1306_DISPLAYOFF 0xAE          ///< See datasheet
#define SSD1306_DISPLAYON 0xAF           ///< See datasheet
#define SSD1306_COMSCANINC 0xC0          ///< Not currently used
#define SSD1306_COMSCANDEC 0xC8          ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET 0xD3    ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5  ///< See datasheet
#define SSD1306_SETPRECHARGE 0xD9        ///< See datasheet
#define SSD1306_SETCOMPINS 0xDA          ///< See datasheet
#define SSD1306_SETVCOMDETECT 0xDB       ///< See datasheet

#define SSD1306_SETLOWCOLUMN 0x00  ///< Not currently used
#define SSD1306_SETHIGHCOLUMN 0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE 0x40  ///< See datasheet

#define SSD1306_EXTERNALVCC 0x01  ///< External display voltage source
#define SSD1306_SWITCHCAPVCC 0x02 ///< Gen. display voltage from 3.3V

#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26              ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27               ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A  ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL 0x2E                    ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL 0x2F                      ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3             ///< Set scroll range



// Issue single command to SSD1306, using I2C or hard/soft SPI as needed.
// Because command calls are often grouped, SPI transaction and selection
// must be started/ended in calling function for efficiency.
// This is a private function, not exposed (see ssd1306_command() instead).
void ssd1306_command1(uint8_t c) {
	begin_i2c_write(SID);
	write_i2c_byte_1(0x80);
	write_i2c_byte_1(c);
	end_i2c_1();
}

void send_u8_i2c(u8 c) {
	write_i2c_byte_1(c);
}

void ssd1306_commandList(const uint8_t *c, uint8_t n) 
{
	while (n--) ssd1306_command1(*c++);
}

void send_datum(u8 val)
{
	begin_i2c_write(SID);
	send_u8_i2c(0x40);
	send_u8_i2c(val);
	end_i2c_1();
}


void triplet(u8 v1, u8 v2, u8 v3)
{
	ssd1306_command1(v1);
	ssd1306_command1(v2);
	ssd1306_command1(v3);
}


void clr_scr()
{
	// seems to work for both screens
	// not sure how efficient it is, though
	int pages = HEIGHT/8;
	triplet(SSD1306_PAGEADDR, 0, pages-1);
	triplet(SSD1306_COLUMNADDR, 0, 127);

	begin_i2c_write(SID);
	send_u8_i2c(0x40);
	for(int i = 0; i<128*pages; i++)
		send_u8_i2c(0);
	end_i2c_1();


}

void init1306() {
	u8 comPins = 0x12;
	if(HEIGHT==32) comPins = 0x02;


	u8 init1[] = {
		SSD1306_DISPLAYOFF,         // 0xAE
		SSD1306_SETDISPLAYCLOCKDIV, // 0xD5
		0x80, // the suggested ratio 0x80
		//SSD1306_SETDISPLAYOFFSET, // 0xD3
		//0x0,                      // no offset
		SSD1306_SETSTARTLINE | 0x0, // line #0
		SSD1306_CHARGEPUMP,        // 0x8D
		0x14,
		SSD1306_MEMORYMODE, 0, // 0x20, write across then down
		SSD1306_SEGREMAP | 0x1,
		SSD1306_COMSCANDEC,
		SSD1306_SETCOMPINS, comPins, // 0xDA
		SSD1306_SETPRECHARGE, // 0xd9
		0xF1,
		SSD1306_SETVCOMDETECT, // 0xDB
		0x40,
		//SSD1306_DISPLAYALLON_RESUME, // 0xA4
		0xA4, // Entire display on, output follows RAM content
		//SSD1306_NORMALDISPLAY,       // 0xA6
		//SSD1306_DEACTIVATE_SCROLL,
		SSD1306_SETMULTIPLEX, HEIGHT-1, // 0xA8 
		SSD1306_DISPLAYON // 0xAF
	};
	ssd1306_commandList(init1, sizeof(init1));
	clr_scr();
}

const uint8_t digital_font5x7_123[] =
{
    0x00, 0x05, 0x07, 0x30,
    0x36, 0x41, 0x41, 0x36, 0x00, // char '0' (0x30/48)
    0x00, 0x00, 0x00, 0x36, 0x00, // char '1' (0x31/49)
    0x30, 0x49, 0x49, 0x06, 0x00, // char '2' (0x32/50)
    0x00, 0x49, 0x49, 0x36, 0x00, // char '3' (0x33/51)
    0x06, 0x08, 0x08, 0x36, 0x00, // char '4' (0x34/52)
    0x06, 0x49, 0x49, 0x30, 0x00, // char '5' (0x35/53)
    0x36, 0x49, 0x49, 0x30, 0x00, // char '6' (0x36/54)
    0x00, 0x01, 0x01, 0x36, 0x00, // char '7' (0x37/55)
    0x36, 0x49, 0x49, 0x36, 0x00, // char '8' (0x38/56)
    0x06, 0x49, 0x49, 0x36, 0x00, // char '9' (0x39/57)
};



void draw_num(u8 n)
{
	begin_i2c_write(SID);
	send_u8_i2c(0x40);
	//send_u8_i2c(val);
	int offset = n*5 + 4;
	for(int i =offset; i< offset + 5; i++)
		send_u8_i2c(digital_font5x7_123[i]);
	end_i2c_1();
}

void  low_level_test() {
	static const uint8_t  dlist1[] = {
		SSD1306_PAGEADDR,
		0,                      // Page start address
		0,                   // Page end (not really, but works here)
		SSD1306_COLUMNADDR, 0, 127
	};
	ssd1306_commandList(dlist1, sizeof(dlist1));

	for(int i = 0; i< 10; i++)
		draw_num(i);
#if 0
	return;

	u8 letterP[] = {0x00, 0x7F, 0x09, 0x09, 0x09, 0x06};
	//u8 letterP[] = {0xFF, 0x7F, 0x09, 0x09, 0x09, 0x06};
	//for(int i = 0; i<1024; i++)
	//	send_datum(0);

	for(int i = 0; i<6; i++)
		send_datum(letterP[i]);

	//for(int i = 0; i<6; i++)
	//	send_datum(letterP[i]);
#endif
}



void gpio_mode_out_drain(u8 pin)
{
#if 1
	pinMode(pin, OUTPUT);
#else
	PORT_t* port = pin_to_port(pin);
	u8 pos = pin_to_pos(pin);
	port->DDR |= (1<< pos); // direction is output
#endif
}


void main() {
#if 0
	gpio_mode_out_drain(PB4);
	gpio_mode_out_drain(PB5);
#endif
	pinMode(PA1, OUTPUT);
	pinMode(PA2, OUTPUT);
	pinMode(PD6, OUTPUT);

	//init_millis();
	init_i2c_1(); // this completes
	//here();
	//while(1);
	//init1306(SSD1306_SWITCHCAPVCC); // this completes
	init1306();
	low_level_test();

	//for(u32 i = 0; i < 5000; i++) nop();
	//send_cmd(0xAE); // turn display off - which doesn't seem to work

	//gpio_write(PD6, 1); // check that we make it to end
	//here();

	while(1);
}

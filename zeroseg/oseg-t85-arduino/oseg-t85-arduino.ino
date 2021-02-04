//#include <tinySPI.h> // doesn't seem to work

/* use usual pins for SPI:
   PB2 SCK
   PB0 MOSI
   MISO not required
   Use 5V input to the display
 */

#define CS PB1
#define MOSI PB0
#define SCK PB2
//#define CS (1<<4)

void transfer_7219(uint8_t address, uint8_t value) {
	digitalWrite(CS, LOW); 
	shiftOut(MOSI, SCK, MSBFIRST, address);
	shiftOut(MOSI, SCK, MSBFIRST, value);
	//SPI.transfer(address);
	//SPI.transfer(value);
	digitalWrite(CS, HIGH);
}

void init_7219() {
	pinMode(CS, OUTPUT);
	digitalWrite(CS, HIGH);
	pinMode(MOSI, OUTPUT);
	pinMode(SCK, OUTPUT);
	//SPI.setBitOrder(MSBFIRST);
	//SPI.begin();
	transfer_7219(0x0F, 0x00);
	transfer_7219(0x09, 0xFF); // Enable mode B
	transfer_7219(0x0A, 0x0F); // set intensity (page 9)
	transfer_7219(0x0B, 0x07); // use all pins
	transfer_7219(0x0C, 0x01); // Turn on chip
}

void setup() {
	init_7219();

}

void loop() {
	static int cnt = 0;
	int num = cnt;
	for (uint8_t i = 0; i < 8; ++i)
	{
		transfer_7219(i+1, num % 10);
		//transfer_7219(8 - i, 3);
		num /= 10;
		delay(1);
	}
	cnt++;
	delay(1000);

}

#include "pico/stdlib.h"
#include "oled.h"

extern "C" const uint8_t splash1_data[];
extern "C" const uint8_t splash2_data[][1024];

int main()
{
	//init_i2c();

	int h = 32;
	if(1) h = 64;
	init_display(h);
	
	ssd1306_print("HELLO PICO...\n"); // demonstrate some text
	show_scr();
	sleep_ms(2000);
	fill_scr(0); // empty the screen
	
	drawAnimation(0, 0, splash2_data, 128, 64, 1, /*frames number: */ 20, /*duration:*/ 2000);
	drawBitmap(0, 0, splash1_data, 64, 64, 1);
	show_scr();

	sleep_ms(2000);
	fill_scr(0);
	setCursorx(0);
	setCursory(0);
	ssd1306_print("Testing cursor");
	show_scr();
	sleep_ms(2000);
	setCursorx(0);
	ssd1306_print("Overwritten   ");
	show_scr();

	for(;;);
	return 0;

	return 0;
}

/**************************************************************************
  This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

  This example is for a 128x32 pixel display using I2C to communicate
  3 pins are required to interface (two I2C and one reset).

  Adafruit invests time and resources providing this open
  source code, please support Adafruit and open-source
  hardware by purchasing products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries,
  with contributions from the open source community.
  BSD license, check license.txt for more information
  All text above, and the splash screen below must be
  included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Timezone.h> // https://github.com/JChristensen/Timezone


#include "RTClib.h"

typedef unsigned long ulong;
//typedef unsigned long micros_t;
typedef ulong ms_t;

TimeChangeRule myBST = {"BST", Last, Sun, Mar, 1, 60};
TimeChangeRule mySTD = {"GMT", Last, Sun, Nov, 2, 0};
Timezone myTZ(myBST, mySTD);
TimeChangeRule *tcr;        //pointer to the time change rule, use to get TZ abbrev

RTC_DS3231 rtc;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

template<int N, typename T>
struct Buffer {
  int capacity = N;
  int size = 0;
  T data[N];
  void push(T value) {
    if (size < capacity) data[size++] = value;
  };
  void zap() {
    for (int i = 0; i < capacity; ++i) data[i] = 0;
    size = 0;
  }
};



struct Periodic {
  //bool active = false;
  ms_t _started;
  //bool _periodic = false;
  ms_t _period;
  Periodic(ms_t period) : _period(period) {
    _started = millis();

  }
  /*
    void begin(bool periodic = true) {
    started = micros();
    active = true;
    _periodic = periodic;
    }
  */
  bool expired() {
    ms_t ms = millis();
    if (ms - _started < _period) return false;
    //active = false;
    _started = ms;
    return true;
  }

};

void do_set(char *buf) {
  buf[15] = 0;
  buf[24] = 0;
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  rtc.adjust(DateTime(buf + 4, buf + 16));
  Serial.println(buf + 4);
  Serial.println(buf + 16);

}
Buffer<30, char> input;
void serialise() {
  if (!Serial.available()) return;
  char c = Serial.read();
  input.push(c);
  if (c != '\r') return;
  if (strncmp("SET", input.data, 3) != 0) return;
  do_set(input.data);
  input.zap();
}


void display_text(const String& str, int col = 0, int row = 0) {
  //display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(col, row);
  //display.println(F("scroll"));
  display.print(str);
  //display.display();      // Show initial text
  //delay(100);
}
void display_char(char c) {
  char buf[2];
  buf[0] = c;
  buf[1] = 0;
  display_text(buf);

}

void setup() {
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//#pragma message __DATE__
//#pragma message __TIME__

  Serial.begin(9600);
  Serial.print("Sizeof ulong (ms_t) = ");
  Serial.println(sizeof(ulong));
  
  rtc.begin(); // this is important

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(100);
}


char* day_names[] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};

void loop() {
  char text[11];
  serialise();
  static Periodic regular(250);
  if (regular.expired()) {
    DateTime dt = rtc.now();
    display.clearDisplay();
    auto tim = dt.unixtime();
    tim = myTZ.toLocal(tim, &tcr);
    DateTime dt_local{tim};
    display.setTextSize(2);
    //display_text(dt.timestamp(DateTime::TIMESTAMP_TIME), 0, 0);
    display_text(dt_local.timestamp(DateTime::TIMESTAMP_TIME), 0, 0);
    //display_text(dt_local.timestamp(DateTime::TIMESTAMP_DATE), 0, 17);
    float degf = rtc.getTemperature();
    char* day_name = day_names[dt_local.dayOfTheWeek()];
    snprintf(text, sizeof(text), "%s %2d %dc     ", day_name, dt_local.day(), (int) degf);
    display_text(text, 0, 17);
    
    display.display();
  }

  static Periodic debouncer(20);
  if(debouncer.expired()) {
    // TODO
  }
}





void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for (int16_t i = 0; i < 256; i++) {
    if (i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 0);            // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.print(F("0x")); display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}

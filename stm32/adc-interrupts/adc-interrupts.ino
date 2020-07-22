/*
  This example shows how to use the ADC library sample
  one channels/pin at a time.
  A bit like analogRead, but with the possibility of having an interrupt.
*/
#include <STM32ADC.h>
#include <SPI.h>

STM32ADC myADC(ADC1);
uint8 adc_pin = PB1;
#define PIN_440 PA0


/* choices for ADC cyles are x = 1,7, 13, 28, 41, 55, 71, 239
    in ADC_SMPR_x_5
    is the number of "ADC cycles"
*/
auto& ser = Serial1;
volatile uint32_t adc_result = 666;
//volatile uint32_t countr;

void adc_eoc_callback() {
  //ser.print("Readin: ");
  adc_result = myADC.getData();
  //countr++;
  //ser.println(adc_result);
};

void setup() {
  ser.begin(9600);
  ser.println("\nHello from oscope-2");
  //myADC.setTrigger(ADC_EXT_EV_SWSTART);//start on SWStart bit
  //myADC.setChannels(pin, 1); //this is actually the pin you want to measure

  // init ADC with interrupts
  pinMode(adc_pin, INPUT_ANALOG);
  myADC.calibrate();
  myADC.setSampleRate(ADC_SMPR_55_5);
  myADC.attachInterrupt(adc_eoc_callback, ADC_EOC);
  myADC.setPins(&adc_pin, 1);  ;
  myADC.setContinuous();
  myADC.startConversion();

  // init SPI
  // The clock value is not used
  // SPI1 is selected by default
  // MOSI, MISO, SCK and NSS PINs are set by the library
  SPI.beginTransactionSlave(SPISettings(18000000, MSBFIRST, SPI_MODE0, DATA_SIZE_8BIT));

  // produce a canned frequency
  pinMode(PIN_440, OUTPUT);
  //tone(PIN_440, 440); // emit a tone on this pin
}

void loop() {
  uint16_t reading = adc_result;
  SPI.transfer16(reading);
  /*
    static int count = 0;
    uint32_t reading = adc_result;
    ser.print(count++);
    ser.print(" ");
    ser.println(reading);
    delay(250);
  */
}

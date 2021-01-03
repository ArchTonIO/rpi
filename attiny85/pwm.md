# PWM

PWM is available on PB0 (OC0A), PB1 (OC1A, OC0B), PB4 (OC1B)

## Arduino

```
#define F_CPU 1000000UL

#define PIN PB4

void setup()
{
	pinMode(PIN, OUTPUT);
}

void loop()
{
	static uint8_t i = 0;
	analogWrite(PIN, i++);
	delay(20);
}
```


Using oscilloscope, PWM frequence is 1.93KHz

See: [pwm-cli](pwm-cli)


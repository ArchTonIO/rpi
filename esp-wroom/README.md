# esp32-wroom


## Pins

```
21 SDA (I2C)
22 SDL (I2C)
```

![Diagram](wroom-pins.png)

## Unclassified

**Built-in blue LED**: GPIO2 (confirmed)

**Ubuntu 20.04**
```
sudo apt install python-is-python3 python3-serial
```

**Upload program** whilst avoiding having to press Boot: 
put a 10uF cap between EN and GND (1-3uF was the one I saw 
recommended, though)


## Also on this site

* [1306](../1306) OLED display
* [FreeRTOS](FreeRTOS)

## Links to other sites

### Audio

* [I2S](https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/peripherals/i2s.html) - IDF from Espressif
* [I2S: what's the rationale behind changing WS on LSB?](https://www.raspberrypi.org/forums/viewtopic.php?f=44&t=243876)
* [I2S and WAV](http://www.iotsharing.com/2017/07/how-to-use-arduino-esp32-i2s-to-play-wav-music-from-sdcard.html?m=1)
* [Playing hard-coded audio on an ESP32 via DAC](https://gist.github.com/blippy/ee1b0f970c56d8eee7f54f796c5b20f1)
* [Stream Your Audio on the ESP32](https://www.hackster.io/julianfschroeter/stream-your-audio-on-the-esp32-2e4661)
* [Streaming audio on an ESP32 with a blinkt](https://gist.github.com/blippy/f9d008d7c6a7db1018bd3929f2352e12)
* [Synthesizer](https://www.youtube.com/watch?v=aShBEQNSTgA)
* [WAV](https://www.xtronical.com/basics/audio/digitised-speech-sound-esp32-playing-wavs/)

### Other

* [EN and BOOT buttons](https://esp32.com/viewtopic.php?t=5822)
* [Gauging button bounce](https://mcturra2000.wordpress.com/2019/06/11/gauging-button-bounce-on-an-esp32-using-atlast-forth/)
* [IDF setup](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/#get-started-setup-toolchain)
* [MicroPython setup](http://micropython.org/download/#esp32)
* [tcp server](https://www.esp32.com/viewtopic.php?f=2&t=7791&p=49302#p49302)

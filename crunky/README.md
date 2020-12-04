# crunky

> 32 bits ought to be enough for anyone


Attempt to build a "unikernel" for Raspberry Pi 3

## setup

Define the environment variable CRUNKY in your .bashrc file:
```
export CRUNKY=/path/to/crunky
```


## Dissasembly

One of 
```
aarch64-linux-gnu-objdump -D kernel8.elf | less
arm-none-eabi-objdump -D kernel.elf 
```

## In this directory

* [usb](usb) - it's not easy, that's for sure.

## Links to libraries

* [printf](https://github.com/mpaland/printf) - but needs support for math, unfortunately

## Links to other sites

* [Raspberry Pi IoT In C - Almost Realtime Linux ](https://www.iot-programmer.com/index.php/books/22-raspberry-pi-and-the-iot-in-c/chapters-raspberry-pi-and-the-iot-in-c/33-raspberry-pi-iot-in-c-almost-realtime-linux?showall=1) book

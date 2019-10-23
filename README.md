# Digital Analog Converter
Basic music player for STM32 Nucleo, developed during Microcontrollers programming course on University of Warsaw in 2019.

## Main features
* Converting digital audio samples to analog signal using real-time clock and timers (pulse-width modulator)
* User input (play, pause, rewind and fast-forward) handled asynchronously with interrupts
* Debug sent via USART to connected PC

## song processing
```
sox song.mp3 -b 8 song.wav channels 1 rate 22050 vol 0.98
gcc music_formatter.c -o music_formatter
./music_formatter <song.wav >src/music.h
```
## compilation
```
make
```

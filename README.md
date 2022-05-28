# EvLamp

Clean implementation of Alex Guyver's FireLamp analogue in pure C for ESP32.

## What it does

Display beautiful effects on a multiple matrices of NxN WS2812B/SK6812/APA106/SM16703 LEDs.

### Features

- ~30 very beautiful effects from different authors, each with set of parameters
- WiFi connectivity in station or access point modes (support for DHCP/static IP addressing)
- [JSON HTTP API for remote controlling](API.md)
- Embedded webserver for remote controlling from browser
- *TODO: OTA*

## Hardware

You will need:

- ESP32 (ESP32 only! ESP32-S2/ESP32-C3/ESP32-C4 and others are not yet supported)
- One or more WS2812B/SK6812/APA106/SM16703 LED matrices
- 4 buttons:
    - Main: long press - On/Off, click - next effect
    - Up/Down: brightness up/down
    - Reset: long press - Reset settings to defaults

You can use the KiCad 6 project located in the [hardware directory](hardware) and containing
an example implementation: a schematic, a simple board and ready-made Gerber files for production
(compatible with JLCPCB requirements).

### LED matrix

The LEDs in the matrix are connected in series like a snake, starting from the bottom row:

![8x8 matrix](images/8x8_matrix.png)

The sizes of the most common matrices:
- 8x8
- 32x8
- 16x16

You can build your own matrix with widely available WS2812B/SK6812/APA106 LED strips.

The dimensions of the matrix side are limited to the range 8..128 and
number of pixels in matrix cannot be more than 1024.

I do not recommend using matrices with a large number of LEDs due to the maximum frame
rate limitations: for a matrix containing 512 LEDs, this limit is 60 fps, and for a matrix of
1024 LEDs maximal fps is 30. It is better to use multiple small matrices.

### Single matrix configuration

To control one matrix, only one GPIO pin of the microcontroller is needed:

![Single matrix](images/single_block.png)

This configuration is the simplest supported.  

### Multiple LED matrices (blocks) configuration

You can connect up to 8 LED matrices of the same size to different GPIO pins and achieve high resolution
rendering of the effect. For example:

![4 blocks configuration example](images/2x2_blocks.png)

### Maximal recommended configuration

2x4 blocks, each 32x16 LEDs.
Total screen dimensions: 64x64, 4096 LEDs, 60 fps max.

### Changing LED layout

Configure your LED layout at runtime by API or in browser ("Menu -> LEDs Settings"):

![LED layout configuration](images/led_config.png)

## Changing default settings 

You can select GPIOs, set default WiFi config, LED layout and more in `idf.py menuconfig`
(`*** EvLamp configuration ***`). 

## How to build firmware

- Setup latest ESP-IDF master
- `git clone https://github.com/UncleRus/EvLamp.git`
- `git submodule init --recursive --remote`
- `idf.py menuconfig`
- `idf.py build`
- `idf.py flash` to upload firmware to ESP

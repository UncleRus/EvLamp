# EvLamp

"Clean" implementation of Alex Guyver's FireLamp analogue in pure C for ESP32.

## What it does

Display beautiful effects on a matrix of NxN WS2812B/SK6812/APA106 LEDs.
Tested with 16x16 WS21812B.

### Feautures

- 16 very beautiful effects from different authors, each with set of parameters
- WiFi connectivity in station or access point modes (support for DHCP/static IP addressing)
- JSON API for remote controlling (*TODO: describe API*)
- TODO: Web pages for remote controlling from browser

## Hardware

- ESP32 devkit
- WS2812B/SK6812/APA106 LED matrix
- 4 buttons:
	- Main: long press - On/Off, click - next effect
	- Up/Down: brightness up/down
	- Reset: Reset settings to defaults
	
TODO: schematic diagram

## How to build

- Setup latest ESP-IDF master
- `git clone --recursive https://github.com/UncleRus/EvLamp.git`
- `idf.py menuconfig`
- `idf.py build`
- `idf.py flash` to upload firmware to ESP

# EvLamp

"Clean" implementation of Alex Guyver's FireLamp in pure C.

## Hardware

- ESP32 devkit
- WS2812B/SK6812/APA106 LED matrix
- Button (optional)

## How to build

- Setup latest ESP-IDF master
- `git clone --recursive https://github.com/UncleRus/EvLamp.git`
- `idf.py menuconfig`
- `idf.py build`
- `idf.py flash` to upload firmware to ESP


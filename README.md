# Raspberry Pi Pico + ILI9341 Display Project

Progressive tutorial from text display to speedometer

## Hardware Requirements
- Raspberry Pi Pico
- 2.4" ILI9341 TFT Display (240x320)
- Jumper wires

## Wiring Diagram

```
ILI9341 Pin → Pico Pin
------------------------
VCC         → 3.3V (Pin 36)
GND         → GND (Pin 38)
CS          → GP17 (Pin 22)
RESET       → GP20 (Pin 26)
DC/RS       → GP16 (Pin 21)
MOSI        → GP19 (Pin 25)
SCK         → GP18 (Pin 24)
LED         → 3.3V (Pin 36)
MISO        → GP16 (Pin 21) - optional
```

## Software Setup

1. Install Pico SDK
2. Set PICO_SDK_PATH environment variable
3. Build each example

## Examples Included

1. **01_text_demo** - Basic text display
2. **02_graphics_demo** - Shapes and colors
3. **03_image_demo** - Display bitmap images
4. **04_speedometer** - Animated speedometer gauge

## Building

```bash
cd example_folder
mkdir build
cd build
cmake ..
make
```

Then copy the `.uf2` file to your Pico in bootloader mode.

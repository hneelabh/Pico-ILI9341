# Building and Running the Examples

## Prerequisites

### 1. Install Pico SDK

```bash
# Clone Pico SDK
cd ~
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init

# Set environment variable (add to ~/.bashrc or ~/.zshrc)
export PICO_SDK_PATH=~/pico-sdk
```

### 2. Install Build Tools

**On Linux (Ubuntu/Debian):**
```bash
sudo apt update
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential
```

**On macOS:**
```bash
brew install cmake
brew tap ArmMbed/homebrew-formulae
brew install arm-none-eabi-gcc
```

**On Windows:**
Install the following:
- CMake: https://cmake.org/download/
- ARM GCC: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm
- Build Tools: Install Visual Studio Build Tools or MinGW

## Building an Example

### Example 1: Text Demo

```bash
cd pico_ili9341_project/01_text_demo
mkdir build
cd build
cmake ..
make
```

This will create `text_demo.uf2` in the build directory.

### Example 2: Graphics Demo

```bash
cd pico_ili9341_project/02_graphics_demo
mkdir build
cd build
cmake ..
make
```

### Example 3: Image Demo

```bash
cd pico_ili9341_project/03_image_demo
mkdir build
cd build
cmake ..
make
```

### Example 4: Speedometer

```bash
cd pico_ili9341_project/04_speedometer
mkdir build
cd build
cmake ..
make
```

## Uploading to Pico

1. Hold down the BOOTSEL button on your Pico
2. Connect the Pico to your computer via USB
3. Release the BOOTSEL button
4. The Pico will appear as a USB drive (RPI-RP2)
5. Copy the `.uf2` file to the drive
6. The Pico will automatically reboot and run your program

## Viewing Debug Output

Connect to the Pico's USB serial port:

**Linux/macOS:**
```bash
screen /dev/ttyACM0 115200
# Or
minicom -D /dev/ttyACM0 -b 115200
```

**Windows:**
Use PuTTY or TeraTerm to connect to the COM port at 115200 baud.

## Troubleshooting

### Display Not Working

1. **Check Wiring**: Verify all connections match the pinout in README.md
2. **Check Power**: Ensure VCC is connected to 3.3V (NOT 5V)
3. **Check SPI**: Verify MOSI and SCK are on the correct pins
4. **Reset Pin**: Make sure RST is connected and not floating

### Display Shows Garbage

1. **SPI Speed**: Try reducing baudrate in code (from 40MHz to 10MHz)
2. **Wire Length**: Use shorter wires if possible
3. **Power Supply**: Ensure stable 3.3V supply

### Build Errors

1. **SDK Not Found**: Make sure PICO_SDK_PATH is set correctly
2. **Compiler Not Found**: Install arm-none-eabi-gcc
3. **Missing Libraries**: Run `git submodule update --init` in pico-sdk directory

### Program Not Running

1. **Check USB Connection**: Try a different USB cable
2. **Verify .uf2**: Make sure the file copied completely
3. **LED Indicator**: Check if onboard LED is blinking (indicates program is running)

## Customizing Examples

### Changing Display Pins

Edit the pin definitions in each `main.c`:

```c
ili9341_config_t display_config = {
    .spi_port = spi0,        // or spi1
    .cs_pin = 17,            // Change to your CS pin
    .dc_pin = 16,            // Change to your DC pin
    .rst_pin = 20,           // Change to your RST pin
    .mosi_pin = 19,          // Must match SPI port
    .sck_pin = 18,           // Must match SPI port
    .baudrate = 40000000     // Adjust if needed
};
```

### Using SPI1 Instead of SPI0

SPI1 pins:
- MOSI: GP11 or GP15
- SCK: GP10 or GP14

Update config:
```c
.spi_port = spi1,
.mosi_pin = 15,  // or 11
.sck_pin = 14,   // or 10
```

### Adjusting Colors

Colors are defined in `ili9341.h`. You can create custom colors:

```c
uint16_t my_color = ili9341_color565(255, 128, 0); // Orange
ili9341_fill_screen(my_color);
```

### Changing Speedometer Range

In `04_speedometer/main.c`, modify:

```c
#define MAX_SPEED 180  // Change to your desired max (e.g., 240, 120)
```

## Adding Your Own Images

See `IMAGE_CONVERTER.md` for instructions on converting images to C arrays.

## Performance Tips

1. **Increase SPI Speed**: Try 62.5MHz if your wiring is good
2. **Batch Operations**: Use `ili9341_fill_rect()` instead of many `draw_pixel()` calls
3. **Reduce Redraws**: Only update changed areas of the screen
4. **Buffer Data**: Prepare data before sending to display

# 🚀 Getting Started with Your ILI9341 Display Project

## What You Have

A complete, ready-to-use project for your Raspberry Pi Pico and ILI9341 2.4" display with:

✅ **Full ILI9341 driver** - Optimized C library with all drawing functions
✅ **4 Progressive Examples** - From text to speedometer
✅ **Image converter tool** - Convert your photos to display format
✅ **Complete documentation** - Build guides and quick reference

## 📁 Project Structure

```
pico_ili9341_project/
├── README.md                 # Main project documentation
├── BUILD_GUIDE.md            # Detailed build instructions
├── QUICK_REFERENCE.md        # Function reference
├── IMAGE_CONVERTER.md        # Image conversion guide
├── CMakeLists.txt            # Root build file
├── build_all.sh             # Convenience build script
│
├── lib/                      # Core library
│   ├── ili9341.h            # Display driver header
│   ├── ili9341.c            # Display driver implementation
│   └── font.h               # 5x7 font data
│
├── tools/
│   └── image_converter.py   # Python image converter
│
├── 01_text_demo/            # Example 1: Text display
│   ├── CMakeLists.txt
│   └── main.c
│
├── 02_graphics_demo/        # Example 2: Shapes and colors
│   ├── CMakeLists.txt
│   └── main.c
│
├── 03_image_demo/           # Example 3: Images
│   ├── CMakeLists.txt
│   ├── main.c
│   └── image_data.h
│
└── 04_speedometer/          # Example 4: Animated speedometer
    ├── CMakeLists.txt
    └── main.c
```

## 🔌 First: Wire Your Display

**IMPORTANT: Use 3.3V, NOT 5V!**

```
ILI9341         Raspberry Pi Pico
────────        ─────────────────
VCC       →     3.3V (Pin 36)
GND       →     GND  (Pin 38)
CS        →     GP17 (Pin 22)
RESET     →     GP20 (Pin 26)
DC        →     GP16 (Pin 21)
MOSI      →     GP19 (Pin 25)
SCK       →     GP18 (Pin 24)
LED       →     3.3V (Pin 36)
```

## ⚙️ Second: Set Up Your Environment

### Linux/macOS:

```bash
# 1. Install Pico SDK
cd ~
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init

# 2. Set environment variable (add to ~/.bashrc or ~/.zshrc)
export PICO_SDK_PATH=~/pico-sdk

# 3. Install build tools
# Ubuntu/Debian:
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential

# macOS:
brew install cmake
brew tap ArmMbed/homebrew-formulae
brew install arm-none-eabi-gcc
```

### Windows:

1. Install CMake: https://cmake.org/download/
2. Install ARM GCC: https://developer.arm.com/downloads/-/gnu-rm
3. Install Pico SDK: https://github.com/raspberrypi/pico-sdk
4. Set PICO_SDK_PATH environment variable

## 🏗️ Third: Build Your First Example

### Option A: Build All Examples at Once

```bash
cd pico_ili9341_project
./build_all.sh
```

### Option B: Build One Example

```bash
cd pico_ili9341_project/01_text_demo
mkdir build
cd build
cmake ..
make
```

This creates `text_demo.uf2`

## 📤 Fourth: Upload to Your Pico

1. **Hold down** the BOOTSEL button on your Pico
2. **Connect** the Pico to your computer via USB
3. **Release** the BOOTSEL button
4. The Pico appears as a USB drive named "RPI-RP2"
5. **Drag and drop** the `.uf2` file onto the drive
6. The Pico automatically reboots and runs your program!

## 🎯 What Each Example Does

### 1. Text Demo (01_text_demo)
- Displays "Hello Pico!"
- Shows different colors and sizes
- Animated counter
- Multiple lines
- Centered text

**Try it first** - easiest to verify your wiring!

### 2. Graphics Demo (02_graphics_demo)
- Lines in various colors
- Rectangles (outlined and filled)
- Circles
- Color gradients
- Animated patterns

### 3. Image Demo (03_image_demo)
- Procedurally generated smiley faces
- Raspberry Pi logo
- Color gradients
- Checkerboard patterns
- Pixel art hearts

### 4. Speedometer (04_speedometer) ⭐
- Full animated gauge
- Color-coded speed zones (green/yellow/red)
- Digital speed display
- Smooth needle animation
- Realistic acceleration/deceleration

**This is the coolest one!** 🎉

## 🖼️ Adding Your Own Images

```bash
# 1. Install PIL
pip install Pillow

# 2. Convert your image
cd tools
python image_converter.py myimage.png myimage.h my_image 100 100

# 3. Include in your project
#include "myimage.h"
ili9341_draw_bitmap(x, y, MY_IMAGE_WIDTH, MY_IMAGE_HEIGHT, my_image);
```

See `IMAGE_CONVERTER.md` for details.

## 🐛 Troubleshooting

### Display shows nothing?
- Check power: VCC to 3.3V (NOT 5V!)
- Verify wiring matches the pinout
- Try a different USB cable
- Check that RESET pin is connected

### Display shows garbage?
- Lower SPI speed: Change `baudrate = 40000000` to `10000000`
- Use shorter wires
- Check connections are solid

### Build errors?
- Verify `PICO_SDK_PATH` is set: `echo $PICO_SDK_PATH`
- Install arm-none-eabi-gcc
- Run `git submodule update --init` in pico-sdk directory

## 📚 Next Steps

1. **Modify the speedometer** - Change max speed, colors, or layout
2. **Add buttons** - Use GPIO pins to control speed or switch displays
3. **Create a menu system** - Multiple screens with navigation
4. **Add sensors** - Read real data (temperature, distance, etc.)
5. **Make a game** - Simple arcade games work great!

## 💡 Quick Code Examples

### Display Text
```c
ili9341_draw_string(10, 10, "Hello!", WHITE, BLACK, 2);
```

### Draw a Circle
```c
ili9341_fill_circle(120, 160, 50, RED);
```

### Custom Color
```c
uint16_t purple = ili9341_color565(128, 0, 128);
ili9341_fill_screen(purple);
```

### Progress Bar
```c
int percent = 75;
int bar_width = (200 * percent) / 100;
ili9341_fill_rect(20, 150, bar_width, 20, GREEN);
```

## 📖 Documentation Files

- **BUILD_GUIDE.md** - Comprehensive build instructions
- **QUICK_REFERENCE.md** - All functions and examples
- **IMAGE_CONVERTER.md** - How to add your images
- **README.md** - Project overview

## 🎓 Learning Path

1. ✅ Start with **01_text_demo** - Verify setup
2. ✅ Try **02_graphics_demo** - Learn drawing
3. ✅ Run **03_image_demo** - See images
4. ✅ Experience **04_modern_speedometer** - Full application
5. 🚀 Build your own project!

## 🤝 Need Help?

- Check QUICK_REFERENCE.md for function details
- See BUILD_GUIDE.md for detailed troubleshooting
- Review example code for patterns
- All code is well-commented!

## 🎉 Have Fun!

You now have everything you need to create amazing displays with your Pico and ILI9341 screen. The speedometer is just the beginning - what will you create?

---

**Happy Making!** 🛠️✨

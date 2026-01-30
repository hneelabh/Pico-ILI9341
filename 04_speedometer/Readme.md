# Modern Motorcycle Speedometer for Raspberry Pi Pico

A modern, sport bike-inspired digital speedometer designed for the ILI9341 TFT display (320x240).

## Features

- 🏍️ **Segmented Arc Display** - 40 LED-style segments with dynamic colors
- 📊 **Color Zones** - Red (0-60) → Yellow (60-120) → Orange (120-160) → Red (160-200 km/h)
- ⚙️ **Gear Indicator** - Shows current gear (N, 1-6) with realistic transitions
- 🔄 **RPM Display** - Color-coded RPM indicator (x1000)
- 📱 **Info Panel** - Odometer, trip meter, fuel level, and temperature displays
- 🎨 **Modern Design** - Dark background with neon colors for high visibility

## Project Structure

```
├── main.c              # Main program with demo sequences
├── speedometer.h       # Header file with constants and function prototypes
├── speedometer.c       # Speedometer implementation
├── ili9341.h           # ILI9341 display driver header (user-provided)
├── ili9341.c           # ILI9341 display driver implementation (user-provided)
└── CMakeLists.txt      # CMake build configuration
```

## Hardware Requirements

- Raspberry Pi Pico
- ILI9341 TFT Display (320x240, SPI)

### Default Pin Configuration

| Display Pin | Pico GPIO |
|-------------|-----------|
| CS          | GPIO 17   |
| DC          | GPIO 16   |
| RST         | GPIO 20   |
| MOSI        | GPIO 19   |
| SCK         | GPIO 18   |

## Building the Project

### Prerequisites

1. Install the Raspberry Pi Pico SDK
2. Set the `PICO_SDK_PATH` environment variable

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

This will generate `modern_speedometer.uf2` file.

### Flashing

1. Hold the BOOTSEL button on your Pico
2. Connect it to your computer via USB
3. Copy `modern_speedometer.uf2` to the RPI-RP2 drive
4. The Pico will reboot and run the speedometer

## Color Configuration

### RGB565 Color Format

The display uses BGR (Blue-Green-Red) format. The RGB565 macro is defined as:

```c
#define RGB565(r, g, b) ((((b) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((r) >> 3))
```

### Speed Zone Colors

| Speed Range | Color  |
|-------------|--------|
| 0-60 km/h   | Red    |
| 60-120 km/h | Yellow |
| 120-160 km/h| Orange |
| 160-200 km/h| Red    |

## Customization

### Adjusting Gear Indicator Position

In `speedometer.c`, modify the Y-coordinates:

```c
// Move gear indicator up/down (smaller Y = higher on screen)
ili9341_fill_rect(CENTER_X - 25, 180, 50, 35, PANEL_BG);  // Change 180
```

### Changing Maximum Speed

In `speedometer.h`:

```c
#define MAX_SPEED 200  // Change to your desired max speed
```

### Modifying Gear Ratios

In `speedometer.c`, edit the `calculate_gear()` function:

```c
int calculate_gear(int speed) {
    if (speed == 0) return 0;
    if (speed < 25) return 1;  // Adjust these values
    if (speed < 50) return 2;
    // ... etc
}
```

## API Reference

### Main Functions

#### `void draw_modern_gauge_background(void)`
Draws the static background elements (arc, panels, labels).

#### `void update_modern_speed(int old_speed, int new_speed, int gear, int rpm)`
Updates the display with new speed, gear, and RPM values.

#### `int calculate_gear(int speed)`
Returns the appropriate gear based on speed.

#### `int calculate_rpm(int speed, int gear)`
Calculates realistic RPM based on speed and gear.

## Demo Sequences

The main program includes three demo sequences:

1. **Realistic Acceleration** - Smooth acceleration with automatic gear changes
2. **Quick Deceleration** - Fast braking simulation
3. **Sport Mode** - High-RPM aggressive acceleration

## License

This project is open source. Feel free to modify and use it in your projects.

## Credits

Inspired by modern motorcycle digital dashboards (Yamaha R1, Kawasaki Ninja, Ducati).
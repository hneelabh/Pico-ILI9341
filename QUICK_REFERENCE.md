# ILI9341 Display Quick Reference

## Pin Connections

```
ILI9341    Pico Pin    GPIO
--------------------------------
VCC    →   3.3V        Pin 36
GND    →   GND         Pin 38
CS     →   GP17        Pin 22
RESET  →   GP20        Pin 26
DC     →   GP16        Pin 21
MOSI   →   GP19        Pin 25 (SPI0 TX)
SCK    →   GP18        Pin 24 (SPI0 SCK)
LED    →   3.3V        Pin 36
MISO   →   (optional)  GP16
```

## Common Functions

### Initialization
```c
ili9341_config_t config = {
    .spi_port = spi0,
    .cs_pin = 17,
    .dc_pin = 16,
    .rst_pin = 20,
    .mosi_pin = 19,
    .sck_pin = 18,
    .baudrate = 40000000
};
ili9341_init(&config);
```

### Screen Operations
```c
ili9341_fill_screen(BLACK);                    // Clear screen
ili9341_set_window(x, y, x+w-1, y+h-1);       // Set drawing area
```

### Drawing Primitives
```c
ili9341_draw_pixel(x, y, color);              // Single pixel
ili9341_draw_line(x0, y0, x1, y1, color);     // Line
ili9341_draw_rect(x, y, w, h, color);         // Rectangle outline
ili9341_fill_rect(x, y, w, h, color);         // Filled rectangle
ili9341_draw_circle(x, y, r, color);          // Circle outline
ili9341_fill_circle(x, y, r, color);          // Filled circle
```

### Text
```c
// size: 1-5 (1=smallest, 5=largest)
ili9341_draw_char(x, y, 'A', WHITE, BLACK, 2);
ili9341_draw_string(x, y, "Hello", RED, BLACK, 2);
```

### Images
```c
ili9341_draw_bitmap(x, y, width, height, image_array);
```

### Colors
```c
// Predefined colors
BLACK, WHITE, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, ORANGE

// Custom colors
uint16_t color = ili9341_color565(r, g, b);  // r,g,b: 0-255
```

## Color Reference (RGB565)

| Color       | Hex Value | RGB (approx) |
|-------------|-----------|--------------|
| BLACK       | 0x0000    | 0,0,0        |
| WHITE       | 0xFFFF    | 255,255,255  |
| RED         | 0xF800    | 255,0,0      |
| GREEN       | 0x07E0    | 0,255,0      |
| BLUE        | 0x001F    | 0,0,255      |
| CYAN        | 0x07FF    | 0,255,255    |
| MAGENTA     | 0xF81F    | 255,0,255    |
| YELLOW      | 0xFFE0    | 255,255,0    |
| ORANGE      | 0xFD20    | 255,165,0    |
| DARKGREEN   | 0x03E0    | 0,128,0      |
| DARKGREY    | 0x7BEF    | 128,128,128  |
| LIGHTGREY   | 0xC618    | 192,192,192  |

## Display Dimensions

```c
ILI9341_WIDTH  = 240 pixels
ILI9341_HEIGHT = 320 pixels
```

## Common Patterns

### Center Text
```c
const char *msg = "Centered";
int text_width = strlen(msg) * 6 * size;
int x = (ILI9341_WIDTH - text_width) / 2;
int y = (ILI9341_HEIGHT - 8 * size) / 2;
ili9341_draw_string(x, y, msg, WHITE, BLACK, size);
```

### Progress Bar
```c
void draw_progress_bar(int percent) {
    int bar_width = (200 * percent) / 100;
    ili9341_draw_rect(20, 150, 200, 20, WHITE);
    ili9341_fill_rect(20, 150, bar_width, 20, GREEN);
}
```

### Analog Gauge
```c
void draw_gauge_needle(float value, float min, float max) {
    float angle = 135 + (value - min) * 270 / (max - min);
    float rad = (angle - 90) * PI / 180.0;
    int x = CENTER_X + RADIUS * cos(rad);
    int y = CENTER_Y + RADIUS * sin(rad);
    ili9341_draw_line(CENTER_X, CENTER_Y, x, y, RED);
}
```

### Animated Sprite
```c
void animate_sprite(int frame) {
    // Clear old position
    ili9341_fill_rect(old_x, old_y, width, height, BLACK);
    
    // Draw new position
    int new_x = old_x + speed;
    ili9341_draw_bitmap(new_x, old_y, width, height, sprite_data);
    
    old_x = new_x;
}
```

## Performance Tips

1. **Batch drawing**: Group operations to minimize SPI transactions
2. **Use fill_rect**: Much faster than many draw_pixel calls
3. **Optimize redraws**: Only update changed portions
4. **Increase SPI speed**: Try up to 62.5 MHz if stable
5. **Pre-calculate**: Do math before drawing loops

## Troubleshooting

| Problem | Solution |
|---------|----------|
| No display | Check wiring, especially VCC (3.3V not 5V!) |
| Garbage display | Lower SPI baudrate, check wire quality |
| Dim display | Connect LED pin to 3.3V |
| Slow updates | Increase SPI speed, optimize code |
| Colors wrong | Verify RGB565 format conversion |

## Build Command Quick Reference

```bash
# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build
make

# Upload
# 1. Hold BOOTSEL, connect Pico
# 2. Copy .uf2 file to RPI-RP2 drive
```

## Example Code Templates

### Basic Template
```c
#include "pico/stdlib.h"
#include "ili9341.h"

int main() {
    ili9341_config_t config = {
        .spi_port = spi0, .cs_pin = 17, .dc_pin = 16,
        .rst_pin = 20, .mosi_pin = 19, .sck_pin = 18,
        .baudrate = 40000000
    };
    
    ili9341_init(&config);
    ili9341_fill_screen(BLACK);
    
    // Your code here
    ili9341_draw_string(10, 10, "Hello!", WHITE, BLACK, 2);
    
    while(1) {
        tight_loop_contents();
    }
}
```

### Animation Loop Template
```c
int main() {
    // ... initialization ...
    
    while(1) {
        // Clear or update only changed areas
        
        // Draw new frame
        
        // Small delay for frame rate
        sleep_ms(33);  // ~30 FPS
    }
}
```

## Useful Resources

- ILI9341 Datasheet: Find online
- Pico SDK Docs: https://raspberrypi.github.io/pico-sdk-doxygen/
- SPI Reference: Pico SDK hardware_spi documentation

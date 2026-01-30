#ifndef ILI9341_H
#define ILI9341_H

#include "pico/stdlib.h"
#include "hardware/spi.h"

// ILI9341 Commands
#define ILI9341_NOP        0x00
#define ILI9341_SWRESET    0x01
#define ILI9341_SLPOUT     0x11
#define ILI9341_DISPOFF    0x28
#define ILI9341_DISPON     0x29
#define ILI9341_CASET      0x2A
#define ILI9341_PASET      0x2B
#define ILI9341_RAMWR      0x2C
#define ILI9341_MADCTL     0x36
#define ILI9341_PIXFMT     0x3A

// Screen dimensions
#define ILI9341_WIDTH  320
#define ILI9341_HEIGHT 240

// Color definitions (RGB565)
#define BLACK       0x0000
#define BLUE        0x001F
#define RED         0x40F8  // WAS F840
#define GREEN       0x07E0
#define YELLOW      0xE0FF  // WAS FFE0
#define WHITE       0xFFFF
#define DARKGREY    0x7BEF
#define LIGHTGREY   0xC618

// Pin configuration structure
typedef struct {
    spi_inst_t *spi_port;
    uint cs_pin;
    uint dc_pin;
    uint rst_pin;
    uint mosi_pin;
    uint sck_pin;
    uint baudrate;
} ili9341_config_t;

// Display rotation
typedef enum {
    ILI9341_ROTATION_0 = 0,
    ILI9341_ROTATION_90 = 1,
    ILI9341_ROTATION_180 = 2,
    ILI9341_ROTATION_270 = 3
} ili9341_rotation_t;

// Initialization
void ili9341_init(ili9341_config_t *config);
void ili9341_reset(void);

// Basic operations
void ili9341_write_command(uint8_t cmd);
void ili9341_write_data(uint8_t data);
void ili9341_write_data16(uint16_t data);

// Display control
void ili9341_set_rotation(uint8_t rotation);
void ili9341_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ili9341_fill_screen(uint16_t color);

// Drawing primitives
void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void ili9341_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void ili9341_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ili9341_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ili9341_draw_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void ili9341_fill_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);

// Text rendering
void ili9341_draw_char(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
void ili9341_draw_string(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t size);

// Image rendering
void ili9341_draw_bitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data);

// Helper functions
uint16_t ili9341_color565(uint8_t r, uint8_t g, uint8_t b);

// Global config pointer
extern ili9341_config_t *g_display_config;

#endif // ILI9341_H

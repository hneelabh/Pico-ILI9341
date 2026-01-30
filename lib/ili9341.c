#include "ili9341.h"
#include "font.h"
#include <string.h>
#include <math.h>

ili9341_config_t *g_display_config = NULL;

// Low-level SPI functions
static inline void cs_select() {
    gpio_put(g_display_config->cs_pin, 0);
}

static inline void cs_deselect() {
    gpio_put(g_display_config->cs_pin, 1);
}

static inline void dc_command() {
    gpio_put(g_display_config->dc_pin, 0);
}

static inline void dc_data() {
    gpio_put(g_display_config->dc_pin, 1);
}

void ili9341_write_command(uint8_t cmd) {
    dc_command();
    cs_select();
    spi_write_blocking(g_display_config->spi_port, &cmd, 1);
    cs_deselect();
}

void ili9341_write_data(uint8_t data) {
    dc_data();
    cs_select();
    spi_write_blocking(g_display_config->spi_port, &data, 1);
    cs_deselect();
}

void ili9341_write_data16(uint16_t data) {
    uint8_t buffer[2];
    buffer[0] = data >> 8;
    buffer[1] = data & 0xFF;
    dc_data();
    cs_select();
    spi_write_blocking(g_display_config->spi_port, buffer, 2);
    cs_deselect();
}

void ili9341_reset(void) {
    gpio_put(g_display_config->rst_pin, 1);
    sleep_ms(5);
    gpio_put(g_display_config->rst_pin, 0);
    sleep_ms(20);
    gpio_put(g_display_config->rst_pin, 1);
    sleep_ms(150);
}

void ili9341_init(ili9341_config_t *config) {
    g_display_config = config;
    
    // Initialize SPI
    spi_init(config->spi_port, config->baudrate);
    gpio_set_function(config->mosi_pin, GPIO_FUNC_SPI);
    gpio_set_function(config->sck_pin, GPIO_FUNC_SPI);
    
    // Initialize control pins
    gpio_init(config->cs_pin);
    gpio_set_dir(config->cs_pin, GPIO_OUT);
    gpio_put(config->cs_pin, 1);
    
    gpio_init(config->dc_pin);
    gpio_set_dir(config->dc_pin, GPIO_OUT);
    
    gpio_init(config->rst_pin);
    gpio_set_dir(config->rst_pin, GPIO_OUT);
    
    // Reset display
    ili9341_reset();
    
    // Initialization sequence
    ili9341_write_command(0xEF);
    ili9341_write_data(0x03);
    ili9341_write_data(0x80);
    ili9341_write_data(0x02);
    
    ili9341_write_command(0xCF);
    ili9341_write_data(0x00);
    ili9341_write_data(0xC1);
    ili9341_write_data(0x30);
    
    ili9341_write_command(0xED);
    ili9341_write_data(0x64);
    ili9341_write_data(0x03);
    ili9341_write_data(0x12);
    ili9341_write_data(0x81);
    
    ili9341_write_command(0xE8);
    ili9341_write_data(0x85);
    ili9341_write_data(0x00);
    ili9341_write_data(0x78);
    
    ili9341_write_command(0xCB);
    ili9341_write_data(0x39);
    ili9341_write_data(0x2C);
    ili9341_write_data(0x00);
    ili9341_write_data(0x34);
    ili9341_write_data(0x02);
    
    ili9341_write_command(0xF7);
    ili9341_write_data(0x20);
    
    ili9341_write_command(0xEA);
    ili9341_write_data(0x00);
    ili9341_write_data(0x00);
    
    ili9341_write_command(0xC0);    // Power control
    ili9341_write_data(0x23);       // VRH[5:0]
    
    ili9341_write_command(0xC1);    // Power control
    ili9341_write_data(0x10);       // SAP[2:0];BT[3:0]
    
    ili9341_write_command(0xC5);    // VCM control
    ili9341_write_data(0x3e);
    ili9341_write_data(0x28);
    
    ili9341_write_command(0xC7);    // VCM control2
    ili9341_write_data(0x86);
    
    ili9341_write_command(ILI9341_MADCTL);
    ili9341_write_data(0x88);
    
    ili9341_write_command(ILI9341_PIXFMT);
    ili9341_write_data(0x55);       // 16bit color
    
    ili9341_write_command(0xB1);
    ili9341_write_data(0x00);
    ili9341_write_data(0x18);
    
    ili9341_write_command(0xB6);    // Display Function Control
    ili9341_write_data(0x08);
    ili9341_write_data(0x82);
    ili9341_write_data(0x27);
    
    ili9341_write_command(0xF2);    // 3Gamma Function Disable
    ili9341_write_data(0x00);
    
    ili9341_write_command(0x26);    // Gamma curve selected
    ili9341_write_data(0x01);
    
    ili9341_write_command(0xE0);    // Set Gamma
    ili9341_write_data(0x0F);
    ili9341_write_data(0x31);
    ili9341_write_data(0x2B);
    ili9341_write_data(0x0C);
    ili9341_write_data(0x0E);
    ili9341_write_data(0x08);
    ili9341_write_data(0x4E);
    ili9341_write_data(0xF1);
    ili9341_write_data(0x37);
    ili9341_write_data(0x07);
    ili9341_write_data(0x10);
    ili9341_write_data(0x03);
    ili9341_write_data(0x0E);
    ili9341_write_data(0x09);
    ili9341_write_data(0x00);
    
    ili9341_write_command(0xE1);    // Set Gamma
    ili9341_write_data(0x00);
    ili9341_write_data(0x0E);
    ili9341_write_data(0x14);
    ili9341_write_data(0x03);
    ili9341_write_data(0x11);
    ili9341_write_data(0x07);
    ili9341_write_data(0x31);
    ili9341_write_data(0xC1);
    ili9341_write_data(0x48);
    ili9341_write_data(0x08);
    ili9341_write_data(0x0F);
    ili9341_write_data(0x0C);
    ili9341_write_data(0x31);
    ili9341_write_data(0x36);
    ili9341_write_data(0x0F);
    
    ili9341_write_command(ILI9341_SLPOUT);
    sleep_ms(120);
    
    ili9341_write_command(ILI9341_DISPON);
}

void ili9341_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    ili9341_write_command(ILI9341_CASET);
    ili9341_write_data(x0 >> 8);
    ili9341_write_data(x0 & 0xFF);
    ili9341_write_data(x1 >> 8);
    ili9341_write_data(x1 & 0xFF);
    
    ili9341_write_command(ILI9341_PASET);
    ili9341_write_data(y0 >> 8);
    ili9341_write_data(y0 & 0xFF);
    ili9341_write_data(y1 >> 8);
    ili9341_write_data(y1 & 0xFF);
    
    ili9341_write_command(ILI9341_RAMWR);
}

void ili9341_fill_screen(uint16_t color) {
    ili9341_fill_rect(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, color);
}

void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= ILI9341_WIDTH || y >= ILI9341_HEIGHT) return;
    
    ili9341_set_window(x, y, x, y);
    ili9341_write_data16(color);
}

// void ili9341_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
//     if (x >= ILI9341_WIDTH || y >= ILI9341_HEIGHT) return;
//     if (x + w > ILI9341_WIDTH) w = ILI9341_WIDTH - x;
//     if (y + h > ILI9341_HEIGHT) h = ILI9341_HEIGHT - y;
    
//     ili9341_set_window(x, y, x + w - 1, y + h - 1);
    
//     uint8_t color_buffer[2];
//     color_buffer[0] = color >> 8;
//     color_buffer[1] = color & 0xFF;
    
//     dc_data();
//     cs_select();
//     for (uint32_t i = 0; i < w * h; i++) {
//         spi_write_blocking(g_display_config->spi_port, color_buffer, 2);
//     }
//     cs_deselect();
// }

void ili9341_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    // 1. Validation & Clipping
    if (x >= ILI9341_WIDTH || y >= ILI9341_HEIGHT) return;
    if (x + w > ILI9341_WIDTH) w = ILI9341_WIDTH - x;
    if (y + h > ILI9341_HEIGHT) h = ILI9341_HEIGHT - y;
    
    // 2. Set the window
    ili9341_set_window(x, y, x + w - 1, y + h - 1);
    
    // 3. Prepare the color bytes
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    // 4. Create a "Chunk Buffer" (e.g., 64 bytes = 32 pixels)
    // This fits easily on the stack and speeds up transfer significantly.
    const int BATCH_SIZE = 64; 
    uint8_t buffer[BATCH_SIZE];

    // Fill the buffer with the color pattern once
    for (int i = 0; i < BATCH_SIZE; i += 2) {
        buffer[i] = hi;
        buffer[i+1] = lo;
    }

    dc_data();
    cs_select();

    // 5. Calculate total bytes needed
    uint32_t pixels_total = (uint32_t)w * h;
    uint32_t bytes_remaining = pixels_total * 2;

    // 6. Write in large chunks
    while (bytes_remaining > 0) {
        // If we have more than a full buffer left, send the whole buffer
        if (bytes_remaining >= BATCH_SIZE) {
            spi_write_blocking(g_display_config->spi_port, buffer, BATCH_SIZE);
            bytes_remaining -= BATCH_SIZE;
        } 
        // Otherwise, send only what is left
        else {
            spi_write_blocking(g_display_config->spi_port, buffer, bytes_remaining);
            bytes_remaining = 0;
        }
    }
    
    cs_deselect();
}

void ili9341_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    int16_t dx = abs(x1 - x0);
    int16_t dy = abs(y1 - y0);
    int16_t sx = (x0 < x1) ? 1 : -1;
    int16_t sy = (y0 < y1) ? 1 : -1;
    int16_t err = dx - dy;
    
    while (1) {
        ili9341_draw_pixel(x0, y0, color);
        
        if (x0 == x1 && y0 == y1) break;
        
        int16_t e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void ili9341_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    ili9341_draw_line(x, y, x + w - 1, y, color);
    ili9341_draw_line(x + w - 1, y, x + w - 1, y + h - 1, color);
    ili9341_draw_line(x + w - 1, y + h - 1, x, y + h - 1, color);
    ili9341_draw_line(x, y + h - 1, x, y, color);
}

void ili9341_draw_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    
    ili9341_draw_pixel(x0, y0 + r, color);
    ili9341_draw_pixel(x0, y0 - r, color);
    ili9341_draw_pixel(x0 + r, y0, color);
    ili9341_draw_pixel(x0 - r, y0, color);
    
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        
        ili9341_draw_pixel(x0 + x, y0 + y, color);
        ili9341_draw_pixel(x0 - x, y0 + y, color);
        ili9341_draw_pixel(x0 + x, y0 - y, color);
        ili9341_draw_pixel(x0 - x, y0 - y, color);
        ili9341_draw_pixel(x0 + y, y0 + x, color);
        ili9341_draw_pixel(x0 - y, y0 + x, color);
        ili9341_draw_pixel(x0 + y, y0 - x, color);
        ili9341_draw_pixel(x0 - y, y0 - x, color);
    }
}

void ili9341_fill_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
    for (int16_t y = -r; y <= r; y++) {
        for (int16_t x = -r; x <= r; x++) {
            if (x * x + y * y <= r * r) {
                ili9341_draw_pixel(x0 + x, y0 + y, color);
            }
        }
    }
}

void ili9341_draw_char(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size) {
    if (c < 32 || c > 126) c = '?';
    
    for (uint8_t i = 0; i < 5; i++) {
        uint8_t line = font[c - 32][i];
        for (uint8_t j = 0; j < 8; j++, line >>= 1) {
            if (line & 1) {
                if (size == 1) {
                    ili9341_draw_pixel(x + i, y + j, color);
                } else {
                    ili9341_fill_rect(x + i * size, y + j * size, size, size, color);
                }
            } else if (bg != color) {
                if (size == 1) {
                    ili9341_draw_pixel(x + i, y + j, bg);
                } else {
                    ili9341_fill_rect(x + i * size, y + j * size, size, size, bg);
                }
            }
        }
    }
}

void ili9341_draw_string(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t size) {
    while (*str) {
        ili9341_draw_char(x, y, *str++, color, bg, size);
        x += 6 * size;
    }
}

void ili9341_draw_bitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data) {
    ili9341_set_window(x, y, x + w - 1, y + h - 1);
    
    dc_data();
    cs_select();
    for (uint32_t i = 0; i < w * h; i++) {
        uint8_t buffer[2];
        buffer[0] = data[i] >> 8;
        buffer[1] = data[i] & 0xFF;
        spi_write_blocking(g_display_config->spi_port, buffer, 2);
    }
    cs_deselect();
}

uint16_t ili9341_color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

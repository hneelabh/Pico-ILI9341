#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "ili9341.h"

// Function to generate a smiley face procedurally
void draw_smiley(uint16_t x, uint16_t y, uint16_t size) {
    // Face circle
    ili9341_fill_circle(x, y, size, YELLOW);
    ili9341_draw_circle(x, y, size, ORANGE);
    
    // Left eye
    ili9341_fill_circle(x - size/3, y - size/4, size/8, BLACK);
    
    // Right eye
    ili9341_fill_circle(x + size/3, y - size/4, size/8, BLACK);
    
    // Smile (draw using small circles to approximate arc)
    for (int angle = 20; angle < 160; angle += 5) {
        float rad = angle * 3.14159 / 180.0;
        int sx = x + (size/2) * cos(rad);
        int sy = y + (size/2) * sin(rad);
        ili9341_fill_circle(sx, sy, 2, BLACK);
    }
}

// Function to draw Raspberry Pi logo (simplified)
void draw_raspi_logo(uint16_t x, uint16_t y, uint16_t size) {
    // Simplified Raspberry Pi logo
    // Top leaves
    ili9341_fill_circle(x - size/4, y - size/2, size/4, GREEN);
    ili9341_fill_circle(x + size/4, y - size/2, size/4, GREEN);
    
    // Main raspberry body
    ili9341_fill_circle(x, y, size/2, RED);
    
    // Seeds
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int sx = x - size/4 + (i * size/4);
            int sy = y - size/4 + (j * size/4);
            ili9341_fill_circle(sx, sy, 2, YELLOW);
        }
    }
}

// Function to create a gradient image
void draw_gradient_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            uint8_t r = (i * 255) / w;
            uint8_t g = (j * 255) / h;
            uint8_t b = 128;
            uint16_t color = ili9341_color565(r, g, b);
            ili9341_draw_pixel(x + i, y + j, color);
        }
    }
}

// Function to draw a checkerboard
void draw_checkerboard(uint16_t x, uint16_t y, uint16_t size, uint16_t squares) {
    uint16_t square_size = size / squares;
    for (int i = 0; i < squares; i++) {
        for (int j = 0; j < squares; j++) {
            uint16_t color = ((i + j) % 2 == 0) ? WHITE : BLACK;
            ili9341_fill_rect(x + i * square_size, y + j * square_size, 
                            square_size, square_size, color);
        }
    }
}

// Function to draw a simple pixel art heart
void draw_heart(uint16_t cx, uint16_t cy, uint16_t size) {
    // Heart shape using pixels
    uint8_t heart_pattern[7][7] = {
        {0,1,1,0,1,1,0},
        {1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1},
        {0,1,1,1,1,1,0},
        {0,0,1,1,1,0,0},
        {0,0,0,1,0,0,0}
    };
    
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (heart_pattern[j][i]) {
                ili9341_fill_rect(cx + i * size - 3 * size, 
                                cy + j * size - 3 * size, 
                                size, size, RED);
            }
        }
    }
}

int main() {
    stdio_init_all();
    sleep_ms(2000);
    
    printf("Image Demo Starting...\n");
    
    ili9341_config_t display_config = {
        .spi_port = spi0,
        .cs_pin = 17,
        .dc_pin = 16,
        .rst_pin = 20,
        .mosi_pin = 19,
        .sck_pin = 18,
        .baudrate = 40000000
    };
    
    ili9341_init(&display_config);
    printf("Display initialized\n");
    
    // Demo 1: Smiley Face
    ili9341_fill_screen(BLUE);
    ili9341_draw_string(40, 10, "Smiley Face", WHITE, BLUE, 2);
    draw_smiley(120, 140, 60);
    printf("Smiley drawn\n");
    sleep_ms(3000);
    
    // Demo 2: Raspberry Pi Logo
    ili9341_fill_screen(WHITE);
    ili9341_draw_string(30, 10, "Raspberry Pi", RED, WHITE, 2);
    draw_raspi_logo(120, 140, 80);
    printf("Raspi logo drawn\n");
    sleep_ms(3000);
    
    // Demo 3: Gradient Image
    ili9341_fill_screen(BLACK);
    ili9341_draw_string(30, 10, "Gradient Demo", WHITE, BLACK, 2);
    draw_gradient_image(20, 50, 200, 150);
    printf("Gradient drawn\n");
    sleep_ms(3000);
    
    // Demo 4: Checkerboard
    ili9341_fill_screen(DARKGREY);
    ili9341_draw_string(30, 10, "Checkerboard", WHITE, DARKGREY, 2);
    draw_checkerboard(40, 60, 160, 8);
    printf("Checkerboard drawn\n");
    sleep_ms(3000);
    
    // Demo 5: Multiple hearts
    ili9341_fill_screen(WHITE);
    ili9341_draw_string(50, 10, "Pixel Art", RED, WHITE, 2);
    
    draw_heart(60, 80, 4);
    draw_heart(120, 100, 6);
    draw_heart(180, 80, 4);
    draw_heart(90, 150, 5);
    draw_heart(150, 150, 5);
    
    printf("Pixel art drawn\n");
    sleep_ms(3000);
    
    // Demo 6: Image gallery (all at once)
    ili9341_fill_screen(BLACK);
    ili9341_draw_string(30, 5, "Image Gallery", YELLOW, BLACK, 2);
    
    // Small versions of each
    draw_smiley(40, 60, 25);
    draw_heart(120, 60, 3);
    draw_raspi_logo(180, 60, 30);
    draw_checkerboard(20, 120, 60, 6);
    draw_gradient_image(100, 120, 60, 60);
    draw_smiley(180, 150, 25);
    
    ili9341_draw_string(40, 220, "Gallery View", WHITE, BLACK, 2);
    printf("Gallery drawn\n");
    sleep_ms(5000);
    
    // Final screen
    ili9341_fill_screen(DARKGREEN);
    draw_smiley(120, 100, 40);
    ili9341_draw_string(30, 180, "Image Demo", WHITE, DARKGREEN, 2);
    ili9341_draw_string(40, 210, "Complete!", YELLOW, DARKGREEN, 3);
    
    printf("Demo complete!\n");
    
    const uint LED_PIN = 25;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    while (1) {
        gpio_put(LED_PIN, 1);
        sleep_ms(500);
        gpio_put(LED_PIN, 0);
        sleep_ms(500);
    }
    
    return 0;
}

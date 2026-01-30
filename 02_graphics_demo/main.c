#include <stdio.h>
#include "pico/stdlib.h"
#include "ili9341.h"

int main() {
    stdio_init_all();
    sleep_ms(2000);
    
    printf("Graphics Demo Starting...\n");
    
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
    
    // Demo 1: Lines
    ili9341_fill_screen(BLACK);
    ili9341_draw_string(50, 10, "Lines Demo", WHITE, BLACK, 2);
    
    for (int i = 0; i < 10; i++) {
        ili9341_draw_line(0, 50 + i * 10, ILI9341_WIDTH, 50 + i * 10, 
                         ili9341_color565(255, i * 25, 0));
    }
    
    for (int i = 0; i < 10; i++) {
        ili9341_draw_line(i * 24, 50, i * 24, ILI9341_HEIGHT, 
                         ili9341_color565(0, i * 25, 255));
    }
    
    printf("Lines drawn\n");
    sleep_ms(3000);
    
    // Demo 2: Rectangles
    ili9341_fill_screen(BLACK);
    ili9341_draw_string(30, 10, "Rectangles", WHITE, BLACK, 2);
    
    ili9341_draw_rect(20, 50, 80, 60, RED);
    ili9341_draw_rect(40, 70, 80, 60, GREEN);
    ili9341_draw_rect(60, 90, 80, 60, BLUE);
    
    ili9341_fill_rect(150, 50, 50, 50, YELLOW);
    ili9341_fill_rect(170, 70, 50, 50, CYAN);
    ili9341_fill_rect(150, 90, 50, 50, MAGENTA);
    
    printf("Rectangles drawn\n");
    sleep_ms(3000);
    
    // Demo 3: Circles
    ili9341_fill_screen(BLACK);
    ili9341_draw_string(40, 10, "Circles Demo", WHITE, BLACK, 2);
    
    ili9341_draw_circle(60, 100, 40, RED);
    ili9341_draw_circle(120, 100, 35, GREEN);
    ili9341_draw_circle(180, 100, 30, BLUE);
    
    ili9341_fill_circle(60, 220, 25, YELLOW);
    ili9341_fill_circle(120, 220, 25, CYAN);
    ili9341_fill_circle(180, 220, 25, MAGENTA);
    
    printf("Circles drawn\n");
    sleep_ms(3000);
    
    // Demo 4: Color gradient
    ili9341_fill_screen(BLACK);
    ili9341_draw_string(30, 10, "Color Gradient", WHITE, BLACK, 2);
    
    for (int y = 0; y < 200; y++) {
        uint16_t color = ili9341_color565(y, 255 - y, 128);
        ili9341_draw_line(20, 50 + y, 220, 50 + y, color);
    }
    
    printf("Gradient drawn\n");
    sleep_ms(3000);
    
    // Demo 5: Animated circles
    ili9341_fill_screen(BLACK);
    ili9341_draw_string(20, 10, "Animation Demo", WHITE, BLACK, 2);
    
    for (int radius = 5; radius < 80; radius += 5) {
        ili9341_draw_circle(120, 160, radius, 
                           ili9341_color565(radius * 3, 255 - radius * 3, 128));
        sleep_ms(100);
    }
    
    printf("Animation complete\n");
    sleep_ms(2000);
    
    // Demo 6: Pattern
    ili9341_fill_screen(BLACK);
    ili9341_draw_string(50, 10, "Pattern Demo", WHITE, BLACK, 2);
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 10; j++) {
            uint16_t color;
            if ((i + j) % 2 == 0) {
                color = RED;
            } else {
                color = BLUE;
            }
            ili9341_fill_rect(i * 30, 50 + j * 25, 25, 20, color);
        }
    }
    
    printf("Pattern drawn\n");
    sleep_ms(3000);
    
    // Final screen
    ili9341_fill_screen(DARKGREY);
    
    ili9341_fill_circle(120, 120, 60, RED);
    ili9341_fill_circle(120, 120, 45, YELLOW);
    ili9341_fill_circle(120, 120, 30, GREEN);
    
    ili9341_draw_string(30, 200, "Graphics Demo", WHITE, DARKGREY, 2);
    ili9341_draw_string(40, 230, "Complete!", YELLOW, DARKGREY, 3);
    
    printf("Demo complete!\n");
    
    const uint LED_PIN = 25;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    while (1) {
        gpio_put(LED_PIN, 1);
        sleep_ms(1000);
        gpio_put(LED_PIN, 0);
        sleep_ms(1000);
    }
    
    return 0;
}

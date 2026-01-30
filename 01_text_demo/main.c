#include <stdio.h>
#include "pico/stdlib.h"
#include "ili9341.h"

int main() {
    stdio_init_all();
    sleep_ms(2000); // Allow USB to enumerate
    
    printf("Text Demo Starting...\n");
    
    // Configure display pins
    ili9341_config_t display_config = {
        .spi_port = spi0,
        .cs_pin = 17,
        .dc_pin = 15,
        .rst_pin = 20,
        .mosi_pin = 18, // swap; was 19
        .sck_pin = 19, // swap; was 18
        .baudrate = 20000000  // 40 MHz
    };
    
    // Initialize display
    ili9341_init(&display_config);
    printf("Display initialized\n");
    
    // Clear screen to black
    ili9341_fill_screen(BLACK);
    sleep_ms(500);
    
    // Demo 1: Basic text
    ili9341_draw_string(10, 10, "Hello Pico!", WHITE, BLACK, 2);
    printf("Text 1 drawn\n");
    sleep_ms(2000);
    
    // Demo 2: Different colors
    ili9341_draw_string(10, 40, "Red Text", RED, BLACK, 2);
    ili9341_draw_string(10, 70, "Green Text", GREEN, BLACK, 2);
    ili9341_draw_string(10, 100, "Blue Text", BLUE, BLACK, 2);
    printf("Colored text drawn\n");
    sleep_ms(2000);
    
    // Demo 3: Different sizes
    ili9341_draw_string(10, 140, "Size 1", YELLOW, BLACK, 1);
    ili9341_draw_string(10, 160, "Size 2", CYAN, BLACK, 2);
    ili9341_draw_string(10, 190, "Size 3", MAGENTA, BLACK, 3);
    printf("Different sizes drawn\n");
    sleep_ms(2000);
    
    // Demo 4: Animated counter
    ili9341_fill_screen(BLACK);
    ili9341_draw_string(30, 50, "Counter Demo:", WHITE, BLACK, 2);
    
    char buffer[20];
    for (int i = 0; i <= 100; i++) {
        sprintf(buffer, "Count: %d  ", i);
        ili9341_draw_string(50, 100, buffer, GREEN, BLACK, 3);
        sleep_ms(50);
    }
    
    printf("Counter complete\n");
    sleep_ms(1000);
    
    // Demo 5: Multiple lines
    ili9341_fill_screen(BLUE);
    ili9341_draw_string(10, 10, "Line 1: Info", WHITE, BLUE, 2);
    ili9341_draw_string(10, 40, "Line 2: Data", WHITE, BLUE, 2);
    ili9341_draw_string(10, 70, "Line 3: Status", WHITE, BLUE, 2);
    ili9341_draw_string(10, 100, "Line 4: Ready!", WHITE, BLUE, 2);
    
    printf("Multiple lines drawn\n");
    sleep_ms(3000);
    
    // Demo 6: Centered text
    ili9341_fill_screen(BLACK);
    const char *msg = "CENTERED!";
    int text_width = strlen(msg) * 6 * 3; // 6 pixels per char * size
    int x = (ILI9341_WIDTH - text_width) / 2;
    int y = (ILI9341_HEIGHT - 8 * 3) / 2; // 8 pixels height * size
    ili9341_draw_string(x, y, msg, CYAN, BLACK, 3);
    
    printf("Centered text drawn\n");
    sleep_ms(3000);
    
    // Final message
    ili9341_fill_screen(DARKGREEN);
    ili9341_draw_string(20, 100, "Text Demo", WHITE, DARKGREEN, 3);
    ili9341_draw_string(30, 140, "Complete!", YELLOW, DARKGREEN, 3);
    
    printf("Demo complete!\n");
    
    // Blink LED to show we're done
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

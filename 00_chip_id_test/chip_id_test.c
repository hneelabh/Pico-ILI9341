#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

/*
 * DISPLAY CHIP IDENTIFICATION
 * This attempts to READ the display chip ID to identify what it really is
 * Requires MISO connection (optional on most displays)
 */

#define CS   17
#define DC   16
#define RST  20
#define MOSI 19
#define SCK  18
#define MISO 16  // Try using DC pin as MISO for reading
#define LED  25

void cs_low() { gpio_put(CS, 0); }
void cs_high() { gpio_put(CS, 1); }
void dc_low() { gpio_put(DC, 0); }
void dc_high() { gpio_put(DC, 1); }

void send_cmd(uint8_t cmd) {
    dc_low();
    cs_low();
    spi_write_blocking(spi0, &cmd, 1);
    cs_high();
}

void send_data(uint8_t data) {
    dc_high();
    cs_low();
    spi_write_blocking(spi0, &data, 1);
    cs_high();
}

void reset() {
    gpio_put(RST, 1);
    sleep_ms(10);
    gpio_put(RST, 0);
    sleep_ms(20);
    gpio_put(RST, 1);
    sleep_ms(150);
}

int main() {
    stdio_init_all();
    sleep_ms(3000);
    
    printf("\n\n");
    printf("================================================\n");
    printf("  DISPLAY CHIP IDENTIFICATION TEST\n");
    printf("================================================\n");
    printf("\n");
    printf("This will help identify your display chip.\n");
    printf("Even without MISO, we can test different chips.\n");
    printf("================================================\n\n");
    
    // Initialize
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    
    spi_init(spi0, 10000000);
    gpio_set_function(MOSI, GPIO_FUNC_SPI);
    gpio_set_function(SCK, GPIO_FUNC_SPI);
    
    gpio_init(CS);
    gpio_set_dir(CS, GPIO_OUT);
    gpio_put(CS, 1);
    
    gpio_init(DC);
    gpio_set_dir(DC, GPIO_OUT);
    
    gpio_init(RST);
    gpio_set_dir(RST, GPIO_OUT);
    
    reset();
    
    printf("=== CRITICAL QUESTION ===\n\n");
    printf("Look at your display module carefully.\n");
    printf("Can you see any of these chip markings?\n\n");
    
    printf("Common ILI9341 markings:\n");
    printf("  - ILI9341\n");
    printf("  - ILI9341V\n");
    printf("  - 9341\n\n");
    
    printf("Other common chips (NOT ILI9341):\n");
    printf("  - ST7789 / ST7789V\n");
    printf("  - ST7735 / ST7735S\n");
    printf("  - ILI9488\n");
    printf("  - ILI9342\n");
    printf("  - HX8357\n\n");
    
    printf("If you see ANY of these, let me know!\n");
    printf("Your display might not be ILI9341!\n\n");
    
    printf("================================================\n\n");
    
    // Test different chip initializations
    printf("Testing different chip types...\n\n");
    
    // Test 1: ILI9341 specific test
    printf("Test 1: ILI9341 Initialization\n");
    reset();
    send_cmd(0x11); // Sleep out
    sleep_ms(120);
    send_cmd(0x3A); // Pixel format
    send_data(0x55);
    send_cmd(0x36); // MADCTL
    send_data(0x48);
    send_cmd(0x29); // Display on
    sleep_ms(100);
    
    // Try to fill with red
    send_cmd(0x2A);
    send_data(0x00); send_data(0x00);
    send_data(0x00); send_data(0xEF);
    send_cmd(0x2B);
    send_data(0x00); send_data(0x00);
    send_data(0x01); send_data(0x3F);
    send_cmd(0x2C);
    
    dc_high();
    cs_low();
    uint8_t red[] = {0xF8, 0x00};
    for(int i = 0; i < 100; i++) {
        spi_write_blocking(spi0, red, 2);
    }
    cs_high();
    
    printf("  ILI9341 test sent - Look for ANY change!\n");
    gpio_put(LED, 1); sleep_ms(200); gpio_put(LED, 0);
    sleep_ms(3000);
    
    // Test 2: ST7789
    printf("\nTest 2: ST7789 Initialization\n");
    reset();
    send_cmd(0x11);
    sleep_ms(120);
    send_cmd(0x3A);
    send_data(0x55);
    send_cmd(0x36);
    send_data(0x00); // Different MADCTL
    send_cmd(0xB2); // Porch setting (ST7789 specific)
    send_data(0x0C); send_data(0x0C);
    send_data(0x00); send_data(0x33); send_data(0x33);
    send_cmd(0x29);
    sleep_ms(100);
    
    send_cmd(0x2A);
    send_data(0x00); send_data(0x00);
    send_data(0x00); send_data(0xEF);
    send_cmd(0x2B);
    send_data(0x00); send_data(0x00);
    send_data(0x01); send_data(0x3F);
    send_cmd(0x2C);
    
    dc_high();
    cs_low();
    uint8_t green[] = {0x07, 0xE0};
    for(int i = 0; i < 100; i++) {
        spi_write_blocking(spi0, green, 2);
    }
    cs_high();
    
    printf("  ST7789 test sent - Look for ANY change!\n");
    gpio_put(LED, 1); sleep_ms(200); gpio_put(LED, 0);
    sleep_ms(200);
    gpio_put(LED, 1); sleep_ms(200); gpio_put(LED, 0);
    sleep_ms(3000);
    
    // Test 3: ILI9488 (18-bit color)
    printf("\nTest 3: ILI9488 Initialization (18-bit)\n");
    reset();
    send_cmd(0x11);
    sleep_ms(120);
    send_cmd(0x3A);
    send_data(0x66); // 18-bit color for ILI9488
    send_cmd(0x36);
    send_data(0x48);
    send_cmd(0x29);
    sleep_ms(100);
    
    send_cmd(0x2A);
    send_data(0x00); send_data(0x00);
    send_data(0x01); send_data(0x3F); // ILI9488 is 320x480
    send_cmd(0x2B);
    send_data(0x00); send_data(0x00);
    send_data(0x01); send_data(0xDF);
    send_cmd(0x2C);
    
    dc_high();
    cs_low();
    uint8_t blue[] = {0x00, 0x00, 0xFF}; // 18-bit: 3 bytes per pixel
    for(int i = 0; i < 100; i++) {
        spi_write_blocking(spi0, blue, 3);
    }
    cs_high();
    
    printf("  ILI9488 test sent - Look for ANY change!\n");
    gpio_put(LED, 1); sleep_ms(200); gpio_put(LED, 0);
    sleep_ms(200);
    gpio_put(LED, 1); sleep_ms(200); gpio_put(LED, 0);
    sleep_ms(200);
    gpio_put(LED, 1); sleep_ms(200); gpio_put(LED, 0);
    sleep_ms(3000);
    
    printf("\n================================================\n");
    printf("  ALL CHIP TYPE TESTS COMPLETE\n");
    printf("================================================\n\n");
    
    printf("Did you see ANY color or change in ANY test?\n");
    printf("  - Test 1 (1 blink): ILI9341\n");
    printf("  - Test 2 (2 blinks): ST7789\n");
    printf("  - Test 3 (3 blinks): ILI9488\n\n");
    
    printf("If NOTHING changed:\n");
    printf("  1. Check display chip marking\n");
    printf("  2. Verify MOSI goes to SDI/MOSI/DIN pin\n");
    printf("  3. Verify SCK goes to SCK/CLK pin\n");
    printf("  4. Try swapping MOSI and SCK wires\n");
    printf("  5. Check if display needs 5V instead of 3.3V\n\n");
    
    printf("================================================\n\n");
    
    // Continuous LED blink
    printf("Test complete - LED will blink continuously\n");
    while(1) {
        gpio_put(LED, 1);
        sleep_ms(500);
        gpio_put(LED, 0);
        sleep_ms(500);
    }
    
    return 0;
}

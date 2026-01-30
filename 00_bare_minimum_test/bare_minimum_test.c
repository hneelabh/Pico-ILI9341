#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

/*
 * ABSOLUTE MINIMAL DISPLAY TEST
 * This bypasses all libraries and directly sends raw commands
 * If this doesn't work, it's definitely a hardware issue
 */

#define CS   17
#define DC   16
#define RST  20
#define MOSI 19
#define SCK  18
#define LED  25

// Inline functions for speed
static inline void cs_low() { gpio_put(CS, 0); }
static inline void cs_high() { gpio_put(CS, 1); }
static inline void dc_low() { gpio_put(DC, 0); }  // Command
static inline void dc_high() { gpio_put(DC, 1); } // Data

// Send one byte as command
void cmd(uint8_t c) {
    dc_low();
    cs_low();
    spi_write_blocking(spi0, &c, 1);
    cs_high();
    sleep_us(10);
}

// Send one byte as data
void dat(uint8_t d) {
    dc_high();
    cs_low();
    spi_write_blocking(spi0, &d, 1);
    cs_high();
    sleep_us(10);
}

int main() {
    stdio_init_all();
    sleep_ms(3000);
    
    printf("\n");
    printf("================================================\n");
    printf("  ABSOLUTE MINIMAL DISPLAY TEST\n");
    printf("================================================\n");
    printf("\n");
    printf("This sends RAW commands directly to display.\n");
    printf("No libraries, no initialization sequences.\n");
    printf("Just the absolute minimum to show SOMETHING.\n");
    printf("\n");
    printf("If this doesn't work, it's 100%% hardware issue!\n");
    printf("================================================\n\n");
    
    // Setup LED for status
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    
    // Setup SPI - VERY SLOW for reliability
    printf("Initializing SPI at 1 MHz (ultra slow)...\n");
    spi_init(spi0, 1000000); // 1 MHz - extremely slow
    gpio_set_function(MOSI, GPIO_FUNC_SPI);
    gpio_set_function(SCK, GPIO_FUNC_SPI);
    
    // Setup control pins
    gpio_init(CS);
    gpio_set_dir(CS, GPIO_OUT);
    gpio_put(CS, 1); // CS high = inactive
    
    gpio_init(DC);
    gpio_set_dir(DC, GPIO_OUT);
    
    gpio_init(RST);
    gpio_set_dir(RST, GPIO_OUT);
    
    printf("GPIO initialized\n\n");
    
    // Hardware reset - VERY SLOW
    printf("Step 1: Hardware Reset\n");
    gpio_put(RST, 1);
    sleep_ms(50);
    gpio_put(RST, 0);
    sleep_ms(100);
    gpio_put(RST, 1);
    sleep_ms(200);
    gpio_put(LED, 1);
    sleep_ms(200);
    gpio_put(LED, 0);
    printf("  Reset complete (1 LED blink)\n\n");
    
    // Wait
    sleep_ms(500);
    
    // Software reset
    printf("Step 2: Software Reset (0x01)\n");
    cmd(0x01);
    sleep_ms(200);
    gpio_put(LED, 1);
    sleep_ms(200);
    gpio_put(LED, 0);
    sleep_ms(200);
    gpio_put(LED, 1);
    sleep_ms(200);
    gpio_put(LED, 0);
    printf("  Software reset sent (2 LED blinks)\n\n");
    
    // Sleep out
    printf("Step 3: Sleep Out (0x11)\n");
    cmd(0x11);
    sleep_ms(150);
    gpio_put(LED, 1);
    sleep_ms(100);
    gpio_put(LED, 0);
    sleep_ms(100);
    gpio_put(LED, 1);
    sleep_ms(100);
    gpio_put(LED, 0);
    sleep_ms(100);
    gpio_put(LED, 1);
    sleep_ms(100);
    gpio_put(LED, 0);
    printf("  Sleep out sent (3 LED blinks)\n\n");
    
    // Display on
    printf("Step 4: Display ON (0x29)\n");
    cmd(0x29);
    sleep_ms(100);
    for(int i = 0; i < 4; i++) {
        gpio_put(LED, 1);
        sleep_ms(100);
        gpio_put(LED, 0);
        sleep_ms(100);
    }
    printf("  Display ON sent (4 LED blinks)\n\n");
    
    sleep_ms(500);
    
    // Now try to write a single pixel
    printf("Step 5: Writing single RED pixel at (0,0)\n");
    
    // Column address
    cmd(0x2A);
    dat(0x00);
    dat(0x00);
    dat(0x00);
    dat(0x00);
    
    // Row address
    cmd(0x2B);
    dat(0x00);
    dat(0x00);
    dat(0x00);
    dat(0x00);
    
    // Write to RAM
    cmd(0x2C);
    dat(0xF8); // Red high byte
    dat(0x00); // Red low byte
    
    gpio_put(LED, 1);
    sleep_ms(500);
    gpio_put(LED, 0);
    printf("  Pixel sent (long LED blink)\n\n");
    
    sleep_ms(1000);
    
    // Try to fill a small area with RED
    printf("Step 6: Filling 10x10 area with RED\n");
    
    cmd(0x2A);
    dat(0x00); dat(0x00);
    dat(0x00); dat(0x09); // 0 to 9
    
    cmd(0x2B);
    dat(0x00); dat(0x00);
    dat(0x00); dat(0x09); // 0 to 9
    
    cmd(0x2C);
    
    dc_high();
    cs_low();
    for(int i = 0; i < 100; i++) {
        uint8_t red[2] = {0xF8, 0x00};
        spi_write_blocking(spi0, red, 2);
    }
    cs_high();
    
    printf("  10x10 red area sent\n\n");
    sleep_ms(2000);
    
    // Try full screen black
    printf("Step 7: Attempting FULL SCREEN BLACK\n");
    printf("  This will take ~30 seconds at 1MHz...\n");
    
    cmd(0x2A);
    dat(0x00); dat(0x00);
    dat(0x00); dat(0xEF); // 239
    
    cmd(0x2B);
    dat(0x00); dat(0x00);
    dat(0x01); dat(0x3F); // 319
    
    cmd(0x2C);
    
    uint8_t black[2] = {0x00, 0x00};
    dc_high();
    cs_low();
    for(uint32_t i = 0; i < 76800; i++) {
        spi_write_blocking(spi0, black, 2);
        if(i % 10000 == 0) {
            printf(".");
            gpio_put(LED, !gpio_get(LED)); // Toggle LED
        }
    }
    cs_high();
    printf("\n  Black screen sent!\n\n");
    
    sleep_ms(2000);
    
    // Try full screen RED
    printf("Step 8: Attempting FULL SCREEN RED\n");
    printf("  This will take ~30 seconds...\n");
    
    cmd(0x2A);
    dat(0x00); dat(0x00);
    dat(0x00); dat(0xEF);
    
    cmd(0x2B);
    dat(0x00); dat(0x00);
    dat(0x01); dat(0x3F);
    
    cmd(0x2C);
    
    uint8_t red[2] = {0xF8, 0x00};
    dc_high();
    cs_low();
    for(uint32_t i = 0; i < 76800; i++) {
        spi_write_blocking(spi0, red, 2);
        if(i % 10000 == 0) {
            printf(".");
            gpio_put(LED, !gpio_get(LED));
        }
    }
    cs_high();
    printf("\n  Red screen sent!\n\n");
    
    sleep_ms(2000);
    
    // Color cycle
    printf("Step 9: COLOR CYCLE (infinite)\n");
    printf("  Black -> Red -> Green -> Blue -> White\n\n");
    
    uint16_t colors[] = {0x0000, 0xF800, 0x07E0, 0x001F, 0xFFFF};
    const char* names[] = {"BLACK", "RED", "GREEN", "BLUE", "WHITE"};
    
    int cycle = 0;
    while(1) {
        for(int c = 0; c < 5; c++) {
            printf("Cycle %d: %s (0x%04X)\n", cycle, names[c], colors[c]);
            
            cmd(0x2A);
            dat(0x00); dat(0x00);
            dat(0x00); dat(0xEF);
            
            cmd(0x2B);
            dat(0x00); dat(0x00);
            dat(0x01); dat(0x3F);
            
            cmd(0x2C);
            
            uint8_t hi = colors[c] >> 8;
            uint8_t lo = colors[c] & 0xFF;
            
            dc_high();
            cs_low();
            for(uint32_t i = 0; i < 76800; i++) {
                spi_write_blocking(spi0, &hi, 1);
                spi_write_blocking(spi0, &lo, 1);
                
                if(i % 20000 == 0) {
                    gpio_put(LED, !gpio_get(LED));
                }
            }
            cs_high();
            
            sleep_ms(3000);
        }
        cycle++;
    }
    
    return 0;
}

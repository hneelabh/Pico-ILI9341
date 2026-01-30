#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

#define CS   17
#define DC   16
#define RST  20
#define MOSI 19
#define SCK  18
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
    printf("===========================================\n");
    printf("  DC PIN DIAGNOSTIC TEST\n");
    printf("===========================================\n");
    printf("\n");
    printf("This test specifically checks the DC pin.\n");
    printf("DC pin MUST toggle for display to work!\n");
    printf("\n");
    printf("Current wiring:\n");
    printf("  DC = GP16 (Pin 21)\n");
    printf("\n");
    printf("We will:\n");
    printf("  1. Toggle DC manually and watch LED\n");
    printf("  2. Try different GPIO pins for DC\n");
    printf("  3. Test with DC forced HIGH/LOW\n");
    printf("===========================================\n\n");
    
    // Setup
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
    
    printf("Test 1: Manual DC Toggle Test\n");
    printf("Watch if DC pin actually changes voltage!\n");
    for(int i = 0; i < 10; i++) {
        printf("  DC LOW (should be 0V)\n");
        gpio_put(DC, 0);
        gpio_put(LED, 1);
        sleep_ms(1000);
        
        printf("  DC HIGH (should be 3.3V)\n");
        gpio_put(DC, 1);
        gpio_put(LED, 0);
        sleep_ms(1000);
    }
    printf("Test 1 complete!\n\n");
    
    printf("Test 2: Try DC on GP15 instead of GP16\n");
    printf("Disconnect DC from GP16, connect to GP15 (Pin 20)\n");
    printf("Press Ctrl+C when ready, then re-run\n");
    printf("Waiting 10 seconds...\n");
    sleep_ms(10000);
    
    printf("\nTest 3: Display test with DC forced HIGH\n");
    printf("(Treating all as DATA - wrong but tests connection)\n");
    reset();
    
    // Keep DC HIGH for everything
    gpio_put(DC, 1);
    
    send_cmd(0x11); // This will be sent as data (wrong!)
    sleep_ms(120);
    send_cmd(0x29);
    sleep_ms(100);
    
    // Try to write color
    send_cmd(0x2A);
    send_data(0x00); send_data(0x00);
    send_data(0x00); send_data(0xEF);
    send_cmd(0x2B);
    send_data(0x00); send_data(0x00);
    send_data(0x01); send_data(0x3F);
    send_cmd(0x2C);
    
    cs_low();
    uint8_t red[] = {0xF8, 0x00};
    for(int i = 0; i < 1000; i++) {
        spi_write_blocking(spi0, red, 2);
    }
    cs_high();
    
    printf("DC forced HIGH test done\n");
    gpio_put(LED, 1); sleep_ms(500); gpio_put(LED, 0);
    sleep_ms(2000);
    
    printf("\nTest 4: Display test with DC forced LOW\n");
    printf("(Treating all as COMMANDS - wrong but tests connection)\n");
    reset();
    
    // Keep DC LOW for everything
    gpio_put(DC, 0);
    
    send_cmd(0x11);
    sleep_ms(120);
    send_cmd(0x29);
    sleep_ms(100);
    
    send_cmd(0x2A);
    send_data(0x00); send_data(0x00);
    send_data(0x00); send_data(0xEF);
    send_cmd(0x2B);
    send_data(0x00); send_data(0x00);
    send_data(0x01); send_data(0x3F);
    send_cmd(0x2C);
    
    cs_low();
    uint8_t green[] = {0x07, 0xE0};
    for(int i = 0; i < 1000; i++) {
        spi_write_blocking(spi0, green, 2);
    }
    cs_high();
    
    printf("DC forced LOW test done\n");
    gpio_put(LED, 1); sleep_ms(500); gpio_put(LED, 0);
    sleep_ms(2000);
    
    printf("\nTest 5: Normal operation with correct DC toggling\n");
    reset();
    
    send_cmd(0x11);
    sleep_ms(120);
    
    send_cmd(0x3A);
    send_data(0x55);
    
    send_cmd(0x36);
    send_data(0x48);
    
    send_cmd(0x29);
    sleep_ms(100);
    
    // Fill screen RED
    printf("Filling with RED...\n");
    send_cmd(0x2A);
    send_data(0x00); send_data(0x00);
    send_data(0x00); send_data(0xEF);
    
    send_cmd(0x2B);
    send_data(0x00); send_data(0x00);
    send_data(0x01); send_data(0x3F);
    
    send_cmd(0x2C);
    
    dc_high();
    cs_low();
    for(uint32_t i = 0; i < 76800; i++) {
        uint8_t red[] = {0xF8, 0x00};
        spi_write_blocking(spi0, red, 2);
        if(i % 10000 == 0) {
            gpio_put(LED, !gpio_get(LED));
        }
    }
    cs_high();
    
    printf("RED screen complete!\n");
    sleep_ms(2000);
    
    printf("\n===========================================\n");
    printf("  CRITICAL QUESTION:\n");
    printf("===========================================\n");
    printf("\n");
    printf("Did you see ANY change in ANY of these tests?\n");
    printf("  - Test 3 (DC forced HIGH)?\n");
    printf("  - Test 4 (DC forced LOW)?\n");
    printf("  - Test 5 (Normal DC toggle)?\n");
    printf("\n");
    printf("If NO change in ANY test:\n");
    printf("  -> DC pin is NOT connected properly!\n");
    printf("  -> Or DC wire is broken\n");
    printf("  -> Or wrong pin on display\n");
    printf("\n");
    printf("ACTION: Try connecting DC to GP15 instead!\n");
    printf("Or try GP14, GP13, or GP12\n");
    printf("\n");
    printf("===========================================\n");
    
    // Continuous test
    while(1) {
        printf("\nContinuous color cycle...\n");
        
        uint16_t colors[] = {0x0000, 0xF800, 0x07E0, 0x001F, 0xFFFF};
        const char* names[] = {"BLACK", "RED", "GREEN", "BLUE", "WHITE"};
        
        for(int c = 0; c < 5; c++) {
            printf("%s\n", names[c]);
            
            send_cmd(0x2A);
            send_data(0x00); send_data(0x00);
            send_data(0x00); send_data(0xEF);
            
            send_cmd(0x2B);
            send_data(0x00); send_data(0x00);
            send_data(0x01); send_data(0x3F);
            
            send_cmd(0x2C);
            
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
            
            sleep_ms(2000);
        }
    }
    
    return 0;
}

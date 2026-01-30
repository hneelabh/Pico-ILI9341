#include <stdio.h>
#include "pico/stdlib.h"
#include "ili9341.h"
#include "speedometer.h"

int main() {
    stdio_init_all();
    sleep_ms(2000);
    
    // Configure ILI9341 display
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
    printf("Modern speedometer initialized\n");
    
    // Draw the modern gauge background
    draw_modern_gauge_background();
    printf("Modern gauge background drawn\n");
    
    int current_speed = 0;
    int current_gear = 0;
    int current_rpm = 1;
    
    while (1) {
        // Start from neutral
        update_modern_speed(0, 0, 0, 1);
        sleep_ms(2000);
        
        // Demo: Realistic acceleration with gear changes
        printf("Accelerating with gear changes...\n");
        for (int speed = 0; speed <= 180; speed += 3) {
            current_gear = calculate_gear(speed);
            current_rpm = calculate_rpm(speed, current_gear);
            
            update_modern_speed(current_speed, speed, current_gear, current_rpm);
            current_speed = speed;
            
            // Slight pause during gear changes for realism
            if (speed == 25 || speed == 50 || speed == 80 || speed == 120 || speed == 160) {
                sleep_ms(200);
            }
            
            sleep_ms(40);
        }
        
        sleep_ms(2000);
        
        // Demo: Quick deceleration
        printf("Decelerating...\n");
        for (int speed = 180; speed >= 0; speed -= 5) {
            current_gear = calculate_gear(speed);
            current_rpm = calculate_rpm(speed, current_gear);
            
            update_modern_speed(current_speed, speed, current_gear, current_rpm);
            current_speed = speed;
            sleep_ms(30);
        }
        
        sleep_ms(1000);
        
        // Demo: Sport mode acceleration (fast)
        printf("Sport mode acceleration...\n");
        for (int speed = 0; speed <= 200; speed += 5) {
            current_gear = calculate_gear(speed);
            current_rpm = calculate_rpm(speed, current_gear);
            
            // Simulate high RPM
            if (current_rpm < 11) current_rpm = 11;
            
            update_modern_speed(current_speed, speed, current_gear, current_rpm);
            current_speed = speed;
            sleep_ms(25);
        }
        
        sleep_ms(2000);
        
        // Return to neutral
        printf("Stopping...\n");
        for (int speed = 200; speed >= 0; speed -= 4) {
            current_gear = calculate_gear(speed);
            current_rpm = calculate_rpm(speed, current_gear);
            
            update_modern_speed(current_speed, speed, current_gear, current_rpm);
            current_speed = speed;
            sleep_ms(30);
        }
        
        // Final neutral state
        update_modern_speed(0, 0, 0, 1);
        sleep_ms(3000);
    }
    
    return 0;
}
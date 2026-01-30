#ifndef SPEEDOMETER_H
#define SPEEDOMETER_H

#include <stdint.h>

// Display dimensions (landscape 320x240)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define CENTER_X 160
#define CENTER_Y 140

// Arc speedometer settings
#define ARC_RADIUS 95
#define ARC_THICKNESS 12
#define MAX_SPEED 200

// RGB565 color creator (swap R and B for BGR displays)
#define RGB565(r, g, b) ((((b) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((r) >> 3))

// Custom colors for modern look
#define NEON_RED RGB565(255, 0, 0)
#define NEON_YELLOW RGB565(255, 255, 0)
#define NEON_ORANGE RGB565(255, 140, 0)
#define NEON_GREEN RGB565(0, 255, 100)
#define NEON_BLUE RGB565(0, 200, 255)
#define DARK_BG RGB565(10, 10, 15)
#define PANEL_BG RGB565(20, 20, 30)
#define DARKGREY 0x7BEF

// Speed zones structure
typedef struct {
    int max_speed;
    uint16_t color;
} SpeedZone;

// Function prototypes
void draw_arc_segment(int cx, int cy, int radius, float start_angle, float end_angle, 
                      uint16_t color, int thickness);

void draw_segmented_arc(int cx, int cy, int radius, int segments, int thickness, 
                        int active_segments, uint16_t active_color, uint16_t inactive_color);

uint16_t get_speed_color(int speed);

void draw_modern_gauge_background(void);

void update_modern_speed(int old_speed, int new_speed, int gear, int rpm);

int calculate_gear(int speed);

int calculate_rpm(int speed, int gear);

#endif // SPEEDOMETER_H
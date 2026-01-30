#include "speedometer.h"
#include "ili9341.h"
#include <stdio.h>
#include <math.h>

#define PI 3.14159265359

// Speed zones with colors - GREEN, YELLOW, ORANGE, RED
SpeedZone speed_zones[] = {
    {60, NEON_GREEN},        // 0-60: Green
    {120, NEON_YELLOW},    // 60-120: Yellow
    {160, NEON_ORANGE},    // 120-160: Orange
    {200, NEON_RED}        // 160-200: Red
};

// Draw a thick arc segment
void draw_arc_segment(int cx, int cy, int radius, float start_angle, float end_angle, uint16_t color, int thickness) {
    for (float angle = start_angle; angle <= end_angle; angle += 0.5) {
        float rad = (angle - 90) * PI / 180.0;
        
        for (int t = 0; t < thickness; t++) {
            int r = radius - thickness/2 + t;
            int x = cx + (int)(r * cos(rad));
            int y = cy + (int)(r * sin(rad));
            ili9341_draw_pixel(x, y, color);
        }
    }
}

// Draw segmented arc (like modern bike displays)
void draw_segmented_arc(int cx, int cy, int radius, int segments, int thickness, int active_segments, uint16_t active_color, uint16_t inactive_color) {
    float start_angle = 135.0;  // Start from bottom left
    float end_angle = 45.0;     // End at bottom right (270 degree sweep)
    float total_sweep = 270.0;
    
    float segment_angle = total_sweep / segments;
    float gap_angle = 2.0;  // Gap between segments
    
    for (int i = 0; i < segments; i++) {
        float seg_start = start_angle + i * segment_angle;
        float seg_end = seg_start + segment_angle - gap_angle;
        
        uint16_t color = (i < active_segments) ? active_color : inactive_color;
        draw_arc_segment(cx, cy, radius, seg_start, seg_end, color, thickness);
    }
}

// Get color based on speed zone
uint16_t get_speed_color(int speed) {
    for (int i = 0; i < sizeof(speed_zones)/sizeof(SpeedZone); i++) {
        if (speed <= speed_zones[i].max_speed) {
            return speed_zones[i].color;
        }
    }
    return NEON_GREEN;
}

// Draw modern gauge background
void draw_modern_gauge_background(void) {
    // Fill with dark background
    ili9341_fill_screen(DARK_BG);
    
    // Draw top info panel
    ili9341_fill_rect(0, 0, SCREEN_WIDTH, 35, PANEL_BG);
    ili9341_draw_line(0, 35, SCREEN_WIDTH, 35, DARKGREY);
    
    // Top info labels (smaller icons/text)
    ili9341_draw_string(10, 12, "ODO", DARKGREY, PANEL_BG, 1);
    ili9341_draw_string(10, 22, "12345", WHITE, PANEL_BG, 1);
    
    ili9341_draw_string(70, 12, "TRIP", DARKGREY, PANEL_BG, 1);
    ili9341_draw_string(70, 22, "123.4", WHITE, PANEL_BG, 1);
    
    ili9341_draw_string(250, 12, "FUEL", DARKGREY, PANEL_BG, 1);
    ili9341_draw_string(260, 22, "[ ]", WHITE, PANEL_BG, 1);
    
    // Neutral indicator position (right side)
    ili9341_draw_string(285, 15, "N", DARKGREY, PANEL_BG, 2);
    
    // Draw background arc segments (inactive)
    draw_segmented_arc(CENTER_X, CENTER_Y, ARC_RADIUS, 40, ARC_THICKNESS, 0, NEON_BLUE, RGB565(30, 30, 40));
    
    // Draw speed tick marks (minimal, modern style)
    for (int speed = 0; speed <= MAX_SPEED; speed += 20) {
        float angle = 135 + (speed * 270.0 / MAX_SPEED);
        float rad = (angle - 90) * PI / 180.0;
        
        int x1 = CENTER_X + (int)((ARC_RADIUS - ARC_THICKNESS - 5) * cos(rad));
        int y1 = CENTER_Y + (int)((ARC_RADIUS - ARC_THICKNESS - 5) * sin(rad));
        int x2 = CENTER_X + (int)((ARC_RADIUS - ARC_THICKNESS - 12) * cos(rad));
        int y2 = CENTER_Y + (int)((ARC_RADIUS - ARC_THICKNESS - 12) * sin(rad));
        
        ili9341_draw_line(x1, y1, x2, y2, DARKGREY);
    }
    
    // Central digital speed display background
    ili9341_fill_rect(CENTER_X - 50, CENTER_Y - 25, 100, 50, PANEL_BG);
    ili9341_draw_rect(CENTER_X - 50, CENTER_Y - 25, 100, 50, DARKGREY);
    
    // Speed unit label (small, bottom of digital display)
    ili9341_draw_string(CENTER_X - 15, CENTER_Y + 15, "km/h", DARKGREY, PANEL_BG, 1);
    
    // Gear indicator (bottom center) - moved up to 180
    ili9341_fill_rect(CENTER_X - 25, 180, 50, 35, PANEL_BG);
    ili9341_draw_rect(CENTER_X - 25, 180, 50, 35, DARKGREY);
    ili9341_draw_string(CENTER_X - 18, 183, "GEAR", DARKGREY, PANEL_BG, 1);
    
    // RPM indicator (bottom left)
    ili9341_draw_string(20, 215, "RPM", DARKGREY, DARK_BG, 1);
    ili9341_draw_string(20, 225, "x1000", DARKGREY, DARK_BG, 1);
    
    // Temperature indicator (bottom right)
    ili9341_draw_string(260, 215, "TEMP", DARKGREY, DARK_BG, 1);
    ili9341_draw_string(265, 225, "---C", DARKGREY, DARK_BG, 1);
}

// Update the speed display with modern animation
void update_modern_speed(int old_speed, int new_speed, int gear, int rpm) {
    // Calculate active segments
    int total_segments = 40;
    int old_segments = (old_speed * total_segments) / MAX_SPEED;
    int new_segments = (new_speed * total_segments) / MAX_SPEED;
    
    // Get appropriate color for current speed
    uint16_t speed_color = get_speed_color(new_speed);
    
    // Update arc segments
    if (new_segments != old_segments) {
        draw_segmented_arc(CENTER_X, CENTER_Y, ARC_RADIUS, total_segments, ARC_THICKNESS, 
                          new_segments, speed_color, RGB565(30, 30, 40));
    }
    
    // Update digital speed display
    char speed_str[10];
    sprintf(speed_str, "%3d", new_speed);
    
    // Clear old speed with background
    ili9341_fill_rect(CENTER_X - 45, CENTER_Y - 20, 90, 30, PANEL_BG);
    
    // Draw new speed in large font with appropriate color
    uint16_t display_color = (new_speed > 160) ? NEON_RED : 
                            (new_speed > 120) ? NEON_ORANGE : 
                            (new_speed > 60) ? NEON_YELLOW : NEON_GREEN;
    
    // Large centered speed number
    int text_width = (new_speed >= 100) ? 45 : (new_speed >= 10) ? 30 : 15;
    ili9341_draw_string(CENTER_X - text_width/2, CENTER_Y - 15, speed_str, display_color, PANEL_BG, 3);
    
    // Update gear indicator (moved up to 180 region)
    char gear_str[5];
    if (gear == 0) {
        sprintf(gear_str, "N");
        ili9341_fill_rect(CENTER_X - 23, 193, 46, 20, PANEL_BG);
        ili9341_draw_string(CENTER_X - 8, 195, gear_str, NEON_GREEN, PANEL_BG, 3);
    } else {
        sprintf(gear_str, "%d", gear);
        ili9341_fill_rect(CENTER_X - 23, 193, 46, 20, PANEL_BG);
        ili9341_draw_string(CENTER_X - 8, 195, gear_str, display_color, PANEL_BG, 3);
    }
    
    // Update RPM display (simplified)
    char rpm_str[10];
    sprintf(rpm_str, "%2d", rpm);
    ili9341_fill_rect(50, 215, 30, 20, DARK_BG);
    
    uint16_t rpm_color = (rpm > 10) ? NEON_RED : 
                        (rpm > 7) ? NEON_ORANGE : NEON_GREEN;
    ili9341_draw_string(50, 220, rpm_str, rpm_color, DARK_BG, 2);
    
    // Update neutral indicator
    if (gear == 0) {
        ili9341_draw_string(285, 15, "N", NEON_GREEN, PANEL_BG, 2);
    } else {
        ili9341_draw_string(285, 15, "N", DARKGREY, PANEL_BG, 2);
    }
}

// Calculate gear based on speed (realistic motorcycle gearing)
int calculate_gear(int speed) {
    if (speed == 0) return 0;  // Neutral
    if (speed < 25) return 1;
    if (speed < 50) return 2;
    if (speed < 80) return 3;
    if (speed < 120) return 4;
    if (speed < 160) return 5;
    return 6;
}

// Calculate RPM based on speed and gear (realistic motorcycle RPM)
int calculate_rpm(int speed, int gear) {
    if (gear == 0 || speed == 0) return 1;  // Idle RPM
    
    // Base RPM calculation: each gear has a different ratio
    // Lower gears = higher RPM for same speed
    // Values tuned to give RPM range of 1-13 (representing 1000-13000 RPM)
    float base_rpm;
    
    switch(gear) {
        case 1: base_rpm = speed * 0.20; break;   // 1st gear: high RPM
        case 2: base_rpm = speed * 0.12; break;   // 2nd gear
        case 3: base_rpm = speed * 0.08; break;   // 3rd gear
        case 4: base_rpm = speed * 0.06; break;   // 4th gear
        case 5: base_rpm = speed * 0.05; break;   // 5th gear
        case 6: base_rpm = speed * 0.04; break;   // 6th gear (overdrive)
        default: base_rpm = speed * 0.06; break;
    }
    
    // Add minimum RPM to keep engine running
    int rpm = (int)(base_rpm + 2.0);  // Minimum ~2000 RPM when moving
    
    // Clamp to realistic range (1000-13000 RPM, displayed as 1-13)
    if (rpm < 1) rpm = 1;
    if (rpm > 13) rpm = 13;
    
    return rpm;
}
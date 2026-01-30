#ifndef IMAGE_DATA_H
#define IMAGE_DATA_H

#include <stdint.h>

// Simple 48x48 pixel smiley face image (RGB565)
// You can generate your own images using tools like:
// - GIMP: Export as C source code
// - Python: PIL/Pillow with RGB565 conversion
// - Online converters: image to C array

#define SMILEY_WIDTH 48
#define SMILEY_HEIGHT 48

// Yellow smiley face
const uint16_t smiley_face[SMILEY_WIDTH * SMILEY_HEIGHT] = {
    // This is a simplified representation - you would normally have 2304 values
    // For demonstration, I'll create a procedural smiley
    // In a real project, you'd use an image conversion tool
};

// Helper function to generate a simple smiley procedurally
void generate_smiley(uint16_t *buffer, int size);

// Raspberry Pi logo (simplified, 64x64)
#define RASPI_WIDTH 64
#define RASPI_HEIGHT 64

// Simple checkerboard pattern as placeholder
const uint16_t checkerboard[32 * 32] = {
    // 32x32 checkerboard pattern for demo
};

#endif // IMAGE_DATA_H

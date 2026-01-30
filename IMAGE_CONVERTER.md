# Converting Images for ILI9341 Display

## RGB565 Format

The ILI9341 display uses RGB565 format:
- 5 bits for Red (0-31)
- 6 bits for Green (0-63)  
- 5 bits for Blue (0-31)
- Total: 16 bits (2 bytes) per pixel

## Method 1: Using Python Script

Create a Python script to convert images:

```python
#!/usr/bin/env python3
from PIL import Image
import sys

def rgb888_to_rgb565(r, g, b):
    """Convert RGB888 to RGB565"""
    r5 = (r >> 3) & 0x1F
    g6 = (g >> 2) & 0x3F
    b5 = (b >> 3) & 0x1F
    return (r5 << 11) | (g6 << 5) | b5

def convert_image(input_file, output_file, var_name):
    """Convert image to C array"""
    img = Image.open(input_file)
    img = img.convert('RGB')
    
    width, height = img.size
    pixels = img.load()
    
    with open(output_file, 'w') as f:
        f.write(f"// Generated from {input_file}\n")
        f.write(f"#define {var_name.upper()}_WIDTH {width}\n")
        f.write(f"#define {var_name.upper()}_HEIGHT {height}\n\n")
        f.write(f"const uint16_t {var_name}[{width * height}] = {{\n")
        
        for y in range(height):
            f.write("    ")
            for x in range(width):
                r, g, b = pixels[x, y]
                rgb565 = rgb888_to_rgb565(r, g, b)
                f.write(f"0x{rgb565:04X}")
                
                if y < height - 1 or x < width - 1:
                    f.write(", ")
                
                if (x + 1) % 8 == 0:
                    f.write("\n    ")
            
            if (y + 1) % 10 == 0:
                f.write("\n")
        
        f.write("\n};\n")
    
    print(f"Converted {input_file} to {output_file}")
    print(f"Image size: {width}x{height} ({width*height} pixels)")
    print(f"Array size: {width*height*2} bytes")

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python image_converter.py input.png output.h variable_name")
        print("Example: python image_converter.py logo.png logo.h company_logo")
        sys.exit(1)
    
    convert_image(sys.argv[1], sys.argv[2], sys.argv[3])
```

### Usage:

```bash
# Install PIL if needed
pip install Pillow

# Convert an image
python image_converter.py myimage.png myimage.h my_image

# This creates myimage.h with the C array
```

## Method 2: Using GIMP

1. Open your image in GIMP
2. Resize to desired dimensions: Image → Scale Image
3. Convert to RGB: Image → Mode → RGB
4. Export as C source: File → Export As → Choose "C source code" format
5. Manually convert to RGB565 format (or use the Python script on the exported data)

## Method 3: Online Converter

Use online tools like:
- http://www.rinkydinkelectronics.com/t_imageconverter565.php
- https://javl.github.io/image2cpp/

## Recommended Image Sizes

For best performance:
- Icons: 32x32 or 48x48
- Small images: 64x64
- Medium images: 100x100
- Large images: 160x160
- Full screen: 240x320 (use sparingly - 150KB of data!)

## Example: Adding Your Image

1. Convert your image:
```bash
python image_converter.py logo.png logo_data.h logo_image
```

2. Include in your project:
```c
#include "logo_data.h"

// In your main function:
ili9341_draw_bitmap(50, 50, LOGO_IMAGE_WIDTH, LOGO_IMAGE_HEIGHT, logo_image);
```

3. Add to CMakeLists.txt if it's a separate file:
```cmake
add_executable(my_project
    main.c
    logo_data.c  # if you made it a .c file
    ../lib/ili9341.c
)
```

## Image Optimization Tips

1. **Resize Before Converting**: Scale images to exact display size
2. **Reduce Colors**: Fewer colors = better dithering in RGB565
3. **Avoid Gradients**: They may show banding in RGB565
4. **Test Small First**: Convert small test images before full images
5. **Compression**: For many images, consider storing on SD card and loading

## Memory Considerations

Flash memory on Pico: ~2MB
- 240x320 full screen image = 153,600 bytes (~150KB)
- 100x100 image = 20,000 bytes (~20KB)
- 48x48 icon = 4,608 bytes (~4.5KB)

You can store about 10-13 full screen images in flash, or hundreds of small icons.

## Advanced: Multiple Images

Create a header file with multiple images:

```c
#ifndef IMAGE_COLLECTION_H
#define IMAGE_COLLECTION_H

#include <stdint.h>

// Icon 1
#define ICON1_WIDTH 32
#define ICON1_HEIGHT 32
extern const uint16_t icon1_data[1024];

// Icon 2
#define ICON2_WIDTH 32
#define ICON2_HEIGHT 32
extern const uint16_t icon2_data[1024];

// Background
#define BG_WIDTH 240
#define BG_HEIGHT 320
extern const uint16_t background_data[76800];

#endif
```

## Example Images to Try

1. **Smiley Face**: 48x48 yellow circle with simple features
2. **Icons**: Weather icons, battery indicators, etc.
3. **Logos**: Company or project logos
4. **Graphs**: Pre-rendered charts and graphs
5. **Sprites**: Game characters or objects

## Drawing Bitmap Images

```c
// Simple bitmap draw
ili9341_draw_bitmap(x, y, width, height, image_data);

// Centered bitmap
int x = (ILI9341_WIDTH - width) / 2;
int y = (ILI9341_HEIGHT - height) / 2;
ili9341_draw_bitmap(x, y, width, height, image_data);

// Tiled pattern
for (int ty = 0; ty < ILI9341_HEIGHT; ty += height) {
    for (int tx = 0; tx < ILI9341_WIDTH; tx += width) {
        ili9341_draw_bitmap(tx, ty, width, height, image_data);
    }
}
```

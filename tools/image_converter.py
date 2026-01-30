#!/usr/bin/env python3
"""
Image to RGB565 C Array Converter
Converts images to RGB565 format for ILI9341 displays
"""

from PIL import Image
import sys
import os

def rgb888_to_rgb565(r, g, b):
    """Convert RGB888 (24-bit) to RGB565 (16-bit)"""
    r5 = (r >> 3) & 0x1F  # 5 bits for red
    g6 = (g >> 2) & 0x3F  # 6 bits for green
    b5 = (b >> 3) & 0x1F  # 5 bits for blue
    return (r5 << 11) | (g6 << 5) | b5

def convert_image(input_file, output_file, var_name, max_width=None, max_height=None):
    """
    Convert image to C array in RGB565 format
    
    Args:
        input_file: Input image file path
        output_file: Output .h file path
        var_name: Variable name for the array
        max_width: Maximum width (will scale if larger)
        max_height: Maximum height (will scale if larger)
    """
    try:
        # Open and convert image to RGB
        img = Image.open(input_file)
        img = img.convert('RGB')
        
        # Resize if needed
        if max_width or max_height:
            original_size = img.size
            if max_width and img.width > max_width:
                ratio = max_width / img.width
                new_height = int(img.height * ratio)
                img = img.resize((max_width, new_height), Image.LANCZOS)
            if max_height and img.height > max_height:
                ratio = max_height / img.height
                new_width = int(img.width * ratio)
                img = img.resize((new_width, max_height), Image.LANCZOS)
            
            if img.size != original_size:
                print(f"Resized from {original_size} to {img.size}")
        
        width, height = img.size
        pixels = img.load()
        
        # Write C header file
        with open(output_file, 'w') as f:
            f.write(f"// Auto-generated from {os.path.basename(input_file)}\n")
            f.write(f"// Image size: {width}x{height} pixels\n")
            f.write(f"// Data size: {width * height * 2} bytes\n\n")
            f.write(f"#ifndef {var_name.upper()}_H\n")
            f.write(f"#define {var_name.upper()}_H\n\n")
            f.write(f"#include <stdint.h>\n\n")
            f.write(f"#define {var_name.upper()}_WIDTH {width}\n")
            f.write(f"#define {var_name.upper()}_HEIGHT {height}\n\n")
            f.write(f"const uint16_t {var_name}[{width * height}] = {{\n")
            
            pixel_count = 0
            for y in range(height):
                f.write("    ")
                for x in range(width):
                    r, g, b = pixels[x, y]
                    rgb565 = rgb888_to_rgb565(r, g, b)
                    f.write(f"0x{rgb565:04X}")
                    
                    pixel_count += 1
                    if pixel_count < width * height:
                        f.write(", ")
                    
                    # Line break every 8 values for readability
                    if pixel_count % 8 == 0 and pixel_count < width * height:
                        f.write("\n    ")
            
            f.write("\n};\n\n")
            f.write(f"#endif // {var_name.upper()}_H\n")
        
        print(f"✓ Successfully converted {input_file}")
        print(f"  Output: {output_file}")
        print(f"  Image size: {width}x{height}")
        print(f"  Total pixels: {width * height}")
        print(f"  Array size: {width * height * 2} bytes ({(width * height * 2) / 1024:.2f} KB)")
        
        return True
        
    except FileNotFoundError:
        print(f"✗ Error: File '{input_file}' not found")
        return False
    except Exception as e:
        print(f"✗ Error: {str(e)}")
        return False

def print_usage():
    """Print usage information"""
    print("Image to RGB565 C Array Converter")
    print("=" * 50)
    print("\nUsage:")
    print("  python image_converter.py <input> <output> <varname> [max_width] [max_height]")
    print("\nArguments:")
    print("  input      - Input image file (PNG, JPG, BMP, etc.)")
    print("  output     - Output .h header file")
    print("  varname    - Variable name for the C array")
    print("  max_width  - Optional: Maximum width (will scale down if needed)")
    print("  max_height - Optional: Maximum height (will scale down if needed)")
    print("\nExamples:")
    print("  python image_converter.py logo.png logo.h company_logo")
    print("  python image_converter.py photo.jpg photo.h my_photo 100 100")
    print("  python image_converter.py icon.png icon.h icon_data 48 48")
    print("\nRecommended sizes:")
    print("  - Small icons: 32x32 or 48x48")
    print("  - Medium images: 64x64 or 100x100")
    print("  - Large images: 160x160 or 200x200")
    print("  - Full screen: 240x320 (not recommended - 150KB)")

def main():
    if len(sys.argv) < 4:
        print_usage()
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    var_name = sys.argv[3]
    
    max_width = None
    max_height = None
    
    if len(sys.argv) > 4:
        try:
            max_width = int(sys.argv[4])
        except ValueError:
            print(f"Error: max_width must be an integer")
            sys.exit(1)
    
    if len(sys.argv) > 5:
        try:
            max_height = int(sys.argv[5])
        except ValueError:
            print(f"Error: max_height must be an integer")
            sys.exit(1)
    
    success = convert_image(input_file, output_file, var_name, max_width, max_height)
    sys.exit(0 if success else 1)

if __name__ == "__main__":
    main()

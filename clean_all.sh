#!/bin/bash

# Clean script for all ILI9341 examples
# Usage: ./clean_all.sh

set -e  # Exit on error

echo "=========================================="
echo "Cleaning all ILI9341 Display Examples"
echo "=========================================="
echo ""

# Function to clean a directory
clean_directory() {
    local dir=$1
    if [ -d "$dir/build" ]; then
        echo "  Cleaning $dir/build..."
        rm -rf "$dir/build"
        echo "    ✓ Removed"
    else
        echo "  $dir/build - not found (already clean)"
    fi
}

# Clean main build directory
if [ -d "build" ]; then
    echo "Cleaning main build directory..."
    rm -rf build
    echo "  ✓ Removed"
    echo ""
fi

# Clean each example
echo "Cleaning examples..."
echo ""

clean_directory "00_bare_minimum_test"
clean_directory "00_chip_id_test"
clean_directory "00_dc_pin_test"
clean_directory "01_text_demo"
clean_directory "02_graphics_demo"
clean_directory "03_image_demo"
clean_directory "04_speedometer"

echo ""
echo "=========================================="
echo "Clean Complete!"
echo "=========================================="
echo ""
echo "All build directories have been removed."
echo "Run ./build_all.sh to rebuild everything."
echo ""
#!/bin/bash

# Build script for all ILI9341 examples
# Usage: ./build_all.sh

set -e  # Exit on error

echo "=========================================="
echo "Building all ILI9341 Display Examples"
echo "=========================================="
echo ""

# Check if PICO_SDK_PATH is set
if [ -z "$PICO_SDK_PATH" ]; then
    echo "Error: PICO_SDK_PATH environment variable is not set!"
    echo "Please set it to your Pico SDK location:"
    echo "  export PICO_SDK_PATH=~/pico-sdk"
    exit 1
fi

echo "Using Pico SDK: $PICO_SDK_PATH"
echo ""

# Create build directory
if [ -d "build" ]; then
    echo "Cleaning old build directory..."
    rm -rf build
fi

echo "Creating build directory..."
mkdir build
cd build

# Run CMake
echo ""
echo "Running CMake configuration..."
cmake ..

# Build all
echo ""
echo "Building all examples..."
make -j4

echo ""
echo "=========================================="
echo "Build Complete!"
echo "=========================================="
echo ""
echo "Generated .uf2 files:"
find . -name "*.uf2" -type f | while read file; do
    size=$(ls -lh "$file" | awk '{print $5}')
    echo "  - $(basename $file) ($size)"
done

echo ""
echo "To upload to Pico:"
echo "  1. Hold BOOTSEL button and connect Pico"
echo "  2. Copy desired .uf2 file to RPI-RP2 drive"
echo ""
echo "Build files are in: ./build/"
echo ""

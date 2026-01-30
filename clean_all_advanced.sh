#!/bin/bash

# Advanced clean script for ILI9341 project
# Usage: 
#   ./clean_all.sh           - Clean all build directories
#   ./clean_all.sh --deep    - Deep clean (includes CMake cache files)
#   ./clean_all.sh --help    - Show help

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

show_help() {
    echo "Clean Script for ILI9341 Display Project"
    echo ""
    echo "Usage: ./clean_all.sh [OPTION]"
    echo ""
    echo "Options:"
    echo "  (no option)    Clean all build directories"
    echo "  --deep         Deep clean: removes build dirs + CMake cache + .uf2 files"
    echo "  --dry-run      Show what would be deleted without deleting"
    echo "  --help         Show this help message"
    echo ""
    echo "Examples:"
    echo "  ./clean_all.sh              # Normal clean"
    echo "  ./clean_all.sh --deep       # Deep clean everything"
    echo "  ./clean_all.sh --dry-run    # See what would be deleted"
    echo ""
}

# Parse arguments
DEEP_CLEAN=false
DRY_RUN=false

while [[ $# -gt 0 ]]; do
    case $1 in
        --deep)
            DEEP_CLEAN=true
            shift
            ;;
        --dry-run)
            DRY_RUN=true
            shift
            ;;
        --help|-h)
            show_help
            exit 0
            ;;
        *)
            echo -e "${RED}Error: Unknown option '$1'${NC}"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

echo "=========================================="
if [ "$DEEP_CLEAN" = true ]; then
    echo "Deep Cleaning ILI9341 Project"
else
    echo "Cleaning ILI9341 Project"
fi
if [ "$DRY_RUN" = true ]; then
    echo "(DRY RUN - nothing will be deleted)"
fi
echo "=========================================="
echo ""

# Function to remove directory
remove_dir() {
    local dir=$1
    if [ -d "$dir" ]; then
        if [ "$DRY_RUN" = true ]; then
            echo -e "${YELLOW}  [DRY RUN] Would remove: $dir${NC}"
        else
            echo "  Removing: $dir"
            rm -rf "$dir"
            echo -e "${GREEN}    ✓ Removed${NC}"
        fi
    else
        echo "  $dir - not found (already clean)"
    fi
}

# Function to remove files matching pattern
remove_files() {
    local pattern=$1
    local description=$2
    
    local files=$(find . -name "$pattern" 2>/dev/null)
    if [ -n "$files" ]; then
        echo "  Found $description:"
        echo "$files" | while read -r file; do
            if [ "$DRY_RUN" = true ]; then
                echo -e "${YELLOW}    [DRY RUN] Would remove: $file${NC}"
            else
                echo "    Removing: $file"
                rm -f "$file"
            fi
        done
        if [ "$DRY_RUN" = false ]; then
            echo -e "${GREEN}    ✓ Removed${NC}"
        fi
    fi
}

# Clean main build directory
echo "Cleaning main build directory..."
remove_dir "build"
echo ""

# Clean each example
echo "Cleaning example build directories..."
echo ""

EXAMPLES=("00_bare_minimum_test" "00_chip_id_test" "00_dc_pin_test" "01_text_demo" "02_graphics_demo" "03_image_demo" "04_speedometer")

for example in "${EXAMPLES[@]}"; do
    if [ -d "$example" ]; then
        echo "Cleaning $example..."
        remove_dir "$example/build"
        
        if [ "$DEEP_CLEAN" = true ]; then
            # Remove CMake generated files
            remove_dir "$example/CMakeFiles"
            if [ -f "$example/CMakeCache.txt" ]; then
                if [ "$DRY_RUN" = true ]; then
                    echo -e "${YELLOW}  [DRY RUN] Would remove: $example/CMakeCache.txt${NC}"
                else
                    rm -f "$example/CMakeCache.txt"
                    echo "  Removed CMakeCache.txt"
                fi
            fi
            if [ -f "$example/cmake_install.cmake" ]; then
                if [ "$DRY_RUN" = true ]; then
                    echo -e "${YELLOW}  [DRY RUN] Would remove: $example/cmake_install.cmake${NC}"
                else
                    rm -f "$example/cmake_install.cmake"
                    echo "  Removed cmake_install.cmake"
                fi
            fi
        fi
        echo ""
    fi
done

# Deep clean additional files
if [ "$DEEP_CLEAN" = true ]; then
    echo "Deep clean: Removing additional files..."
    echo ""
    
    # Remove all .uf2 files
    echo "Removing .uf2 files..."
    remove_files "*.uf2" ".uf2 files"
    echo ""
    
    # Remove all CMake generated files in root
    echo "Removing root CMake files..."
    remove_dir "CMakeFiles"
    if [ -f "CMakeCache.txt" ]; then
        if [ "$DRY_RUN" = true ]; then
            echo -e "${YELLOW}  [DRY RUN] Would remove: CMakeCache.txt${NC}"
        else
            rm -f "CMakeCache.txt"
            echo "  Removed CMakeCache.txt"
        fi
    fi
    if [ -f "cmake_install.cmake" ]; then
        if [ "$DRY_RUN" = true ]; then
            echo -e "${YELLOW}  [DRY RUN] Would remove: cmake_install.cmake${NC}"
        else
            rm -f "cmake_install.cmake"
            echo "  Removed cmake_install.cmake"
        fi
    fi
    echo ""
    
    # Remove Makefile if it exists
    if [ -f "Makefile" ]; then
        if [ "$DRY_RUN" = true ]; then
            echo -e "${YELLOW}  [DRY RUN] Would remove: Makefile${NC}"
        else
            rm -f "Makefile"
            echo "  Removed Makefile"
        fi
    fi
fi

echo "=========================================="
if [ "$DRY_RUN" = true ]; then
    echo -e "${YELLOW}Dry Run Complete!${NC}"
    echo ""
    echo "No files were actually deleted."
    echo "Run without --dry-run to perform the clean."
else
    echo -e "${GREEN}Clean Complete!${NC}"
    echo ""
    if [ "$DEEP_CLEAN" = true ]; then
        echo "Deep clean finished. All build artifacts removed."
    else
        echo "Build directories removed."
        echo "Run with --deep for more thorough cleaning."
    fi
    echo ""
    echo "Run ./build_all.sh to rebuild everything."
fi
echo "=========================================="
echo ""
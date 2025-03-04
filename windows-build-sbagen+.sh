#!/bin/bash

# SBaGen+ Windows build script
# Builds 32-bit and 64-bit Windows binaries with MP3 and OGG support using MinGW

# Source common library
. ./lib.sh

section_header "Building SBaGen+ for Windows (32-bit and 64-bit) with MP3 and OGG support..."

# Check for MinGW cross-compilers
if ! command -v i686-w64-mingw32-gcc &> /dev/null || ! command -v x86_64-w64-mingw32-gcc &> /dev/null; then
    error "MinGW cross-compilers not found. Please install them."
    info "On Debian/Ubuntu: sudo apt-get install mingw-w64"
    info "On Fedora: sudo dnf install mingw64-gcc mingw32-gcc"
    info "On Arch: sudo pacman -S mingw-w64-gcc"
    exit 1
fi

# Create libs directory if it doesn't exist
create_dir_if_not_exists "libs"

# Check distribution directory
create_dir_if_not_exists "dist"

# Define paths for libraries
LIB_PATH_32="libs/windows-win32-libmad.a"
LIB_PATH_64="libs/windows-win64-libmad.a"
OGG_LIB_PATH_32="libs/windows-win32-libogg.a"
OGG_LIB_PATH_64="libs/windows-win64-libogg.a"
TREMOR_LIB_PATH_32="libs/windows-win32-libvorbisidec.a"
TREMOR_LIB_PATH_64="libs/windows-win64-libvorbisidec.a"

# Build 32-bit version
section_header "Building 32-bit version..."

# Set up compilation flags for 32-bit
CFLAGS_32="-DT_MINGW -Wall -O3 -I. -Ilibs"
LIBS_32="-lwinmm"

# Check for MP3 support (32-bit)
if [ -f "$LIB_PATH_32" ]; then
    info "Including MP3 support for 32-bit using: $LIB_PATH_32"
    CFLAGS_32="$CFLAGS_32 -DMP3_DECODE"
    LIBS_32="$LIBS_32 $LIB_PATH_32"
else
    warning "MP3 library not found at $LIB_PATH_32"
    warning "MP3 support will not be included in 32-bit build"
    warning "Run ./windows-build-libs.sh to build the required libraries"
fi

# Check for OGG support (32-bit)
if [ -f "$OGG_LIB_PATH_32" ] && [ -f "$TREMOR_LIB_PATH_32" ]; then
    info "Including OGG support for 32-bit using: $OGG_LIB_PATH_32 and $TREMOR_LIB_PATH_32"
    CFLAGS_32="$CFLAGS_32 -DOGG_DECODE"
    # Ordem é importante: primeiro tremor, depois ogg
    LIBS_32="$LIBS_32 $TREMOR_LIB_PATH_32 $OGG_LIB_PATH_32"
else
    warning "OGG libraries not found at $OGG_LIB_PATH_32 or $TREMOR_LIB_PATH_32"
    warning "OGG support will not be included in 32-bit build"
    warning "Run ./windows-build-libs.sh to build the required libraries"
fi

# Compile 32-bit version
info "Compiling 32-bit version with flags: $CFLAGS_32"
info "Libraries: $LIBS_32"

i686-w64-mingw32-gcc -w $CFLAGS_32 sbagen+.c -o dist/sbagen+-win32.exe $LIBS_32

if [ $? -eq 0 ]; then
    success "32-bit compilation successful! Created 32-bit binary: dist/sbagen+-win32.exe"
else
    error "32-bit compilation failed!"
fi

# Build 64-bit version
section_header "Building 64-bit version..."

# Set up compilation flags for 64-bit
CFLAGS_64="-DT_MINGW -Wall -O3 -I. -Ilibs"
LIBS_64="-lwinmm"

# Check for MP3 support (64-bit)
if [ -f "$LIB_PATH_64" ]; then
    info "Including MP3 support for 64-bit using: $LIB_PATH_64"
    CFLAGS_64="$CFLAGS_64 -DMP3_DECODE"
    LIBS_64="$LIBS_64 $LIB_PATH_64"
else
    warning "MP3 library not found at $LIB_PATH_64"
    warning "MP3 support will not be included in 64-bit build"
    warning "Run ./windows-build-libs.sh to build the required libraries"
fi

# Check for OGG support (64-bit)
if [ -f "$OGG_LIB_PATH_64" ] && [ -f "$TREMOR_LIB_PATH_64" ]; then
    info "Including OGG support for 64-bit using: $OGG_LIB_PATH_64 and $TREMOR_LIB_PATH_64"
    CFLAGS_64="$CFLAGS_64 -DOGG_DECODE"
    # Ordem é importante: primeiro tremor, depois ogg
    LIBS_64="$LIBS_64 $TREMOR_LIB_PATH_64 $OGG_LIB_PATH_64"
else
    warning "OGG libraries not found at $OGG_LIB_PATH_64 or $TREMOR_LIB_PATH_64"
    warning "OGG support will not be included in 64-bit build"
    warning "Run ./windows-build-libs.sh to build the required libraries"
fi

# Compile 64-bit version
info "Compiling 64-bit version with flags: $CFLAGS_64"
info "Libraries: $LIBS_64"

x86_64-w64-mingw32-gcc -w $CFLAGS_64 sbagen+.c -o dist/sbagen+-win64.exe $LIBS_64

if [ $? -eq 0 ]; then
    success "64-bit compilation successful! Created 64-bit binary: dist/sbagen+-win64.exe"
else
    error "64-bit compilation failed!"
fi

section_header "Build process completed!" 
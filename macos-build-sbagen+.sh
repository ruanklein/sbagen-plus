#!/bin/bash

# SBaGen+ macOS build script
# Builds a universal binary (ARM64 + x86_64) with MP3 and OGG support

# Source common library
. ./lib.sh

section_header "Building SBaGen+ universal binary (ARM64 + x86_64) with MP3 and OGG support..."

# Create libs directory if it doesn't exist
create_dir_if_not_exists "libs"

# Check distribution directory
create_dir_if_not_exists "dist"

# Check if libraries exist instead of building them automatically
LIB_PATH="libs/macos-universal-libmad.a"
OGG_LIB_PATH="libs/macos-universal-libogg.a"
TREMOR_LIB_PATH="libs/macos-universal-libvorbisidec.a"

# Define compilation flags
CFLAGS="-DT_MACOSX -arch arm64 -arch x86_64 -mmacosx-version-min=11.0 -DNO_CARBON -Wno-deprecated-declarations -I."
LIBS="-framework CoreAudio"

# Check for MP3 support
if [ -f "$LIB_PATH" ]; then
    info "Including MP3 support using: $LIB_PATH"
    CFLAGS="$CFLAGS -DMP3_DECODE"
    LIBS="$LIBS $LIB_PATH"
else
    warning "MP3 library not found at $LIB_PATH"
    warning "MP3 support will not be included"
    warning "Run ./macos-build-libs.sh to build the required libraries"
fi

# Check for OGG support (need both libogg and libvorbisidec)
if [ -f "$OGG_LIB_PATH" ] && [ -f "$TREMOR_LIB_PATH" ]; then
    info "Including OGG support using: $OGG_LIB_PATH and $TREMOR_LIB_PATH"
    CFLAGS="$CFLAGS -DOGG_DECODE"
    # Order is important: first tremor, then ogg
    LIBS="$LIBS $TREMOR_LIB_PATH $OGG_LIB_PATH"
else
    warning "OGG libraries not found at $OGG_LIB_PATH or $TREMOR_LIB_PATH"
    warning "OGG support will not be included"
    warning "Run ./macos-build-libs.sh to build the required libraries"
fi

# Compile sbagen+
section_header "Starting sbagen+ compilation..."
info "Compilation flags: $CFLAGS"
info "Libraries: $LIBS"

gcc -w $CFLAGS sbagen+.c -o dist/sbagen+-macos-universal $LIBS

if [ $? -eq 0 ]; then
    success "Compilation successful! Universal binary created: dist/sbagen+-macos-universal"
    # info "Supported architectures:"
    # lipo -info dist/sbagen+-macOS
else
    error "Compilation failed!"
fi

section_header "Build process completed!" 
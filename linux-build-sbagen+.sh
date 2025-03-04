#!/bin/bash

# SBaGen+ Linux build script
# Builds 32-bit, 64-bit and ARM64 binaries with MP3, OGG and ALSA support

# Source common library
. ./lib.sh

section_header "Building SBaGen+ for Linux (32-bit, 64-bit and ARM64) with MP3, OGG and ALSA support..."

# Create libs directory if it doesn't exist
create_dir_if_not_exists "libs"

# Check for required tools
check_required_tools gcc

# Detect host architecture
HOST_ARCH=$(uname -m)
info "Detected host architecture: $HOST_ARCH"

# Define paths for libraries
LIB_PATH_32="libs/linux-x86-libmad.a"
LIB_PATH_64="libs/linux-x86_64-libmad.a"
LIB_PATH_ARM64="libs/linux-arm64-libmad.a"
OGG_LIB_PATH_32="libs/linux-x86-libogg.a"
OGG_LIB_PATH_64="libs/linux-x86_64-libogg.a"
OGG_LIB_PATH_ARM64="libs/linux-arm64-libogg.a"
TREMOR_LIB_PATH_32="libs/linux-x86-libvorbisidec.a"
TREMOR_LIB_PATH_64="libs/linux-x86_64-libvorbisidec.a"
TREMOR_LIB_PATH_ARM64="libs/linux-arm64-libvorbisidec.a"

# Skip 32-bit build on ARM64
if [ "$HOST_ARCH" = "aarch64" ]; then
    SKIP_32BIT=1
    warning "32-bit compilation is not supported on ARM64, skipping..."
fi

# Build 32-bit version
if [ -z "$SKIP_32BIT" ]; then
    section_header "Building 32-bit version..."

    # Set up compilation flags for 32-bit
    CFLAGS_32="-DT_LINUX_ALSA -m32 -Wall -O3 -I."
    LIBS_32="-lm -lpthread -lasound"

    # Check for MP3 support (32-bit)
    if [ -f "$LIB_PATH_32" ]; then
        info "Including MP3 support for 32-bit using: $LIB_PATH_32"
        CFLAGS_32="$CFLAGS_32 -DMP3_DECODE"
        LIBS_32="$LIBS_32 $LIB_PATH_32"
    else
        warning "MP3 library not found at $LIB_PATH_32"
        warning "MP3 support will not be included in 32-bit build"
        warning "Run ./linux-build-libs.sh to build the required libraries"
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
        warning "Run ./linux-build-libs.sh to build the required libraries"
    fi

    # Compile 32-bit version
    info "Compiling 32-bit version with flags: $CFLAGS_32"
    info "Libraries: $LIBS_32"

    # Try to compile with 32-bit support
    gcc $CFLAGS_32 sbagen+.c -o sbagen+-linux32 $LIBS_32

    if [ $? -eq 0 ]; then
        success "32-bit compilation successful! Binary created: sbagen+-linux32"
    else
        error "32-bit compilation failed! You may need to install 32-bit development libraries."
        info "On Debian/Ubuntu: sudo apt-get install gcc-multilib g++-multilib libc6-dev-i386 libasound2-dev:i386"
        info "On Fedora: sudo dnf install glibc-devel.i686 libstdc++-devel.i686 alsa-lib-devel.i686"
        info "On Arch: sudo pacman -S multilib-devel lib32-alsa-lib"
    fi
else
    warning "Skipping 32-bit build..."
fi

# Build 64-bit version
section_header "Building 64-bit version..."

# Set up compilation flags for 64-bit
if [ "$HOST_ARCH" = "aarch64" ]; then
    # On ARM64, don't use -m64 flag as it's not supported
    CFLAGS_64="-DT_LINUX_ALSA -Wall -O3 -I."
    info "Running on ARM64, using native gcc for 64-bit compilation"
else
    CFLAGS_64="-DT_LINUX_ALSA -m64 -Wall -O3 -I."
fi
LIBS_64="-lm -lpthread -lasound"

# Check for MP3 support (64-bit)
if [ -f "$LIB_PATH_64" ]; then
    info "Including MP3 support for 64-bit using: $LIB_PATH_64"
    CFLAGS_64="$CFLAGS_64 -DMP3_DECODE"
    LIBS_64="$LIBS_64 $LIB_PATH_64"
else
    warning "MP3 library not found at $LIB_PATH_64"
    warning "MP3 support will not be included in 64-bit build"
    warning "Run ./linux-build-libs.sh to build the required libraries"
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
    warning "Run ./linux-build-libs.sh to build the required libraries"
fi

# Compile 64-bit version
info "Compiling 64-bit version with flags: $CFLAGS_64"
info "Libraries: $LIBS_64"

gcc $CFLAGS_64 sbagen+.c -o sbagen+-linux64 $LIBS_64

if [ $? -eq 0 ]; then
    if [ "$HOST_ARCH" = "aarch64" ]; then
        success "64-bit compilation successful! Created ARM64 binary: sbagen+-linux64"
    else
        success "64-bit compilation successful! Binary created: sbagen+-linux64"
    fi
else
    error "64-bit compilation failed!"
    info "On Debian/Ubuntu: sudo apt-get install libasound2-dev"
    info "On Fedora: sudo dnf install alsa-lib-devel"
    info "On Arch: sudo pacman -S alsa-lib"
fi

# Build ARM64 version
if [ "$HOST_ARCH" != "aarch64" ]; then
    section_header "Building ARM64 version..."

    # Check if we have cross-compilation tools
    ARM64_CC="gcc"
    SKIP_ARM64=0

    if command_exists "aarch64-linux-gnu-gcc"; then
        info "Using aarch64-linux-gnu-gcc for cross-compilation"
        ARM64_CC="aarch64-linux-gnu-gcc"
    else
        warning "ARM64 cross-compilation tools not found."
        warning "On Debian/Ubuntu: sudo apt-get install gcc-aarch64-linux-gnu"
        warning "On Fedora: sudo dnf install gcc-aarch64-linux-gnu"
        warning "On Arch: sudo pacman -S aarch64-linux-gnu-gcc"
        warning "Skipping ARM64 build."
        SKIP_ARM64=1
    fi

    if [ "$SKIP_ARM64" -eq 0 ]; then
        # Set up compilation flags for ARM64
        CFLAGS_ARM64="-DT_LINUX_ALSA -Wall -O3 -I."
        LIBS_ARM64="-lm -lpthread -lasound"

        # Check for MP3 support (ARM64)
        if [ -f "$LIB_PATH_ARM64" ]; then
            info "Including MP3 support for ARM64 using: $LIB_PATH_ARM64"
            CFLAGS_ARM64="$CFLAGS_ARM64 -DMP3_DECODE"
            LIBS_ARM64="$LIBS_ARM64 $LIB_PATH_ARM64"
        else
            warning "MP3 library not found at $LIB_PATH_ARM64"
            warning "MP3 support will not be included in ARM64 build"
            warning "Run ./linux-build-libs.sh to build the required libraries"
        fi

        # Check for OGG support (ARM64)
        if [ -f "$OGG_LIB_PATH_ARM64" ] && [ -f "$TREMOR_LIB_PATH_ARM64" ]; then
            info "Including OGG support for ARM64 using: $OGG_LIB_PATH_ARM64 and $TREMOR_LIB_PATH_ARM64"
            CFLAGS_ARM64="$CFLAGS_ARM64 -DOGG_DECODE"
            # Ordem é importante: primeiro tremor, depois ogg
            LIBS_ARM64="$LIBS_ARM64 $TREMOR_LIB_PATH_ARM64 $OGG_LIB_PATH_ARM64"
        else
            warning "OGG libraries not found at $OGG_LIB_PATH_ARM64 or $TREMOR_LIB_PATH_ARM64"
            warning "OGG support will not be included in ARM64 build"
            warning "Run ./linux-build-libs.sh to build the required libraries"
        fi

        # Compile ARM64 version
        info "Compiling ARM64 version with flags: $CFLAGS_ARM64"
        info "Libraries: $LIBS_ARM64"

        $ARM64_CC $CFLAGS_ARM64 sbagen+.c -o sbagen+-linux-arm64 $LIBS_ARM64

        if [ $? -eq 0 ]; then
            success "ARM64 compilation successful! Binary created: sbagen+-linux-arm64"
        else
            error "ARM64 compilation failed!"
            warning "You may need to install ARM64 ALSA development libraries for cross-compilation"
        fi
    else
        warning "Skipping ARM64 build due to missing tools or libraries."
    fi
fi

section_header "Build process completed!" 
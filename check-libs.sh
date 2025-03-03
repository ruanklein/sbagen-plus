#!/bin/bash

# Source common library
. ./lib.sh

section_header "Checking for required libraries..."

# Check for macOS libraries
if [ "$(uname)" = "Darwin" ]; then
    section_header "Checking macOS libraries..."
    
    # Check for MP3 support
    if [ -f "libs/macos-universal-libmad.a" ]; then
        success "MP3 support: Found libs/macos-universal-libmad.a"
    else
        warning "MP3 support: Missing libs/macos-universal-libmad.a"
        warning "Run ./macos-build-libs.sh to build the required libraries"
    fi
    
    # Check for OGG support
    if [ -f "libs/macos-universal-libogg.a" ]; then
        success "OGG support (libogg): Found libs/macos-universal-libogg.a"
    else
        warning "OGG support (libogg): Missing libs/macos-universal-libogg.a"
        warning "Run ./macos-build-libs.sh to build the required libraries"
    fi
    
    if [ -f "libs/macos-universal-libvorbisidec.a" ]; then
        success "OGG support (Tremor): Found libs/macos-universal-libvorbisidec.a"
    else
        warning "OGG support (Tremor): Missing libs/macos-universal-libvorbisidec.a"
        warning "Run ./macos-build-libs.sh to build the required libraries"
    fi
    
    # Check architectures
    if [ -f "libs/macos-universal-libmad.a" ]; then
        info "Checking architectures in libmad.a:"
        lipo -info "libs/macos-universal-libmad.a"
    fi
    
    if [ -f "libs/macos-universal-libogg.a" ]; then
        info "Checking architectures in libogg.a:"
        lipo -info "libs/macos-universal-libogg.a"
    fi
    
    if [ -f "libs/macos-universal-libvorbisidec.a" ]; then
        info "Checking architectures in libvorbisidec.a:"
        lipo -info "libs/macos-universal-libvorbisidec.a"
    fi
fi

# Check for Linux libraries
if [ "$(uname)" = "Linux" ]; then
    section_header "Checking Linux libraries..."
    
    # Check for MP3 support (32-bit)
    if [ -f "libs/linux-libmad.a" ]; then
        success "MP3 support (32-bit): Found libs/linux-libmad.a"
    else
        warning "MP3 support (32-bit): Missing libs/linux-libmad.a"
        warning "Run ./linux-build-libs.sh to build the required libraries"
    fi
    
    # Check for OGG support (32-bit)
    if [ -f "libs/linux-libogg.a" ]; then
        success "OGG support (32-bit, libogg): Found libs/linux-libogg.a"
    else
        warning "OGG support (32-bit, libogg): Missing libs/linux-libogg.a"
        warning "Run ./linux-build-libs.sh to build the required libraries"
    fi
    
    if [ -f "libs/linux-libvorbisidec.a" ]; then
        success "OGG support (32-bit, Tremor): Found libs/linux-libvorbisidec.a"
    else
        warning "OGG support (32-bit, Tremor): Missing libs/linux-libvorbisidec.a"
        warning "Run ./linux-build-libs.sh to build the required libraries"
    fi
    
    # Check for MP3 support (64-bit)
    if [ -f "libs/linux-x86_64-libmad.a" ]; then
        success "MP3 support (64-bit): Found libs/linux-x86_64-libmad.a"
    else
        warning "MP3 support (64-bit): Missing libs/linux-x86_64-libmad.a"
        warning "Run ./linux-build-libs.sh to build the required libraries"
    fi
    
    # Check for OGG support (64-bit)
    if [ -f "libs/linux-x86_64-libogg.a" ]; then
        success "OGG support (64-bit, libogg): Found libs/linux-x86_64-libogg.a"
    else
        warning "OGG support (64-bit, libogg): Missing libs/linux-x86_64-libogg.a"
        warning "Run ./linux-build-libs.sh to build the required libraries"
    fi
    
    if [ -f "libs/linux-x86_64-libvorbisidec.a" ]; then
        success "OGG support (64-bit, Tremor): Found libs/linux-x86_64-libvorbisidec.a"
    else
        warning "OGG support (64-bit, Tremor): Missing libs/linux-x86_64-libvorbisidec.a"
        warning "Run ./linux-build-libs.sh to build the required libraries"
    fi
fi

# Check for Windows libraries (if cross-compiling)
if [ -f "libs/windows-win32-libmad.a" ] || [ -f "libs/windows-win64-libmad.a" ]; then
    section_header "Checking Windows libraries..."
    
    # Check for MP3 support (32-bit)
    if [ -f "libs/windows-win32-libmad.a" ]; then
        success "MP3 support (32-bit): Found libs/windows-win32-libmad.a"
    else
        warning "MP3 support (32-bit): Missing libs/windows-win32-libmad.a"
        warning "Run ./windows-build-libs.sh to build the required libraries"
    fi
    
    # Check for OGG support (32-bit)
    if [ -f "libs/windows-win32-libogg.a" ]; then
        success "OGG support (32-bit, libogg): Found libs/windows-win32-libogg.a"
    else
        warning "OGG support (32-bit, libogg): Missing libs/windows-win32-libogg.a"
        warning "Run ./windows-build-libs.sh to build the required libraries"
    fi
    
    if [ -f "libs/windows-win32-libvorbisidec.a" ]; then
        success "OGG support (32-bit, Tremor): Found libs/windows-win32-libvorbisidec.a"
    else
        warning "OGG support (32-bit, Tremor): Missing libs/windows-win32-libvorbisidec.a"
        warning "Run ./windows-build-libs.sh to build the required libraries"
    fi
    
    # Check for MP3 support (64-bit)
    if [ -f "libs/windows-win64-libmad.a" ]; then
        success "MP3 support (64-bit): Found libs/windows-win64-libmad.a"
    else
        warning "MP3 support (64-bit): Missing libs/windows-win64-libmad.a"
        warning "Run ./windows-build-libs.sh to build the required libraries"
    fi
    
    # Check for OGG support (64-bit)
    if [ -f "libs/windows-win64-libogg.a" ]; then
        success "OGG support (64-bit, libogg): Found libs/windows-win64-libogg.a"
    else
        warning "OGG support (64-bit, libogg): Missing libs/windows-win64-libogg.a"
        warning "Run ./windows-build-libs.sh to build the required libraries"
    fi
    
    if [ -f "libs/windows-win64-libvorbisidec.a" ]; then
        success "OGG support (64-bit, Tremor): Found libs/windows-win64-libvorbisidec.a"
    else
        warning "OGG support (64-bit, Tremor): Missing libs/windows-win64-libvorbisidec.a"
        warning "Run ./windows-build-libs.sh to build the required libraries"
    fi
fi

section_header "Library check completed!" 
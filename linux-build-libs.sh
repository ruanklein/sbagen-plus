#!/bin/bash

# Source common library
. ./lib.sh

# Store the original directory
ORIGINAL_DIR="$PWD"

# Define variables for temporary paths
TEMP_DIR="/tmp/cross_compile"
INSTALL_DIR_X86="$TEMP_DIR/x86"
INSTALL_DIR_X86_64="$TEMP_DIR/x86_64"
SRC_DIR="$PWD/build"
OUTPUT_DIR="$TEMP_DIR/linux"
LIBOGG_VERSION="1.3.5"         # Adjust as per available version
LIBVORBISIDEC_VERSION="sezero" # Using branch name as version identifier
LIBMAD_VERSION="0.15.1b"       # Adjust as per available version

# Check for required tools
check_required_tools gcc g++ curl make automake autoconf libtool unzip

# Create temporary directories
section_header "Creating temporary directories..."
create_dir_if_not_exists "$INSTALL_DIR_X86"
create_dir_if_not_exists "$INSTALL_DIR_X86_64"
create_dir_if_not_exists "$SRC_DIR"
create_dir_if_not_exists "$OUTPUT_DIR"
create_dir_if_not_exists "libs"

# Toolchain settings for cross-compilation
X86_CC="gcc -m32"
X86_64_CC="gcc -m64"

# Check if 32-bit compilation is supported
section_header "Checking for 32-bit compilation support..."
if command_exists "gcc"; then
    # Create a simple test program - using single line to avoid newline issues
    echo 'int main() { return 0; }' > /tmp/test32.c
    
    # Try to compile with -m32
    if gcc -m32 -o /tmp/test32 /tmp/test32.c >/dev/null 2>&1; then
        success "32-bit compilation is supported!"
        rm -f /tmp/test32.c /tmp/test32
    else
        warning "32-bit compilation is not supported. Installing multilib packages might be required."
        warning "On Debian/Ubuntu: sudo apt-get install gcc-multilib g++-multilib"
        warning "On Fedora: sudo dnf install glibc-devel.i686 libstdc++-devel.i686"
        warning "On Arch: sudo pacman -S multilib-devel"
        
        # Ask if the user wants to continue with 64-bit only
        echo -e "${YELLOW}==> Do you want to continue with 64-bit compilation only? (y/n)${NC}"
        read -r response
        if [[ "$response" =~ ^([nN][oO]|[nN])$ ]]; then
            error "Aborting compilation. Please install the required packages and try again."
            exit 1
        else
            warning "Continuing with 64-bit compilation only..."
            # Set a flag to skip 32-bit compilation
            SKIP_32BIT=1
        fi
    fi
else
    error "GCC not found. Please install GCC and try again."
    exit 1
fi

# Download libraries if not present
cd "$SRC_DIR"
section_header "Downloading libogg..."
if [ ! -f "libogg-$LIBOGG_VERSION.tar.gz" ]; then
    curl -L -O "https://downloads.xiph.org/releases/ogg/libogg-$LIBOGG_VERSION.tar.gz"
    tar -xzf "libogg-$LIBOGG_VERSION.tar.gz"
fi
section_header "Downloading libvorbisidec (Tremor)..."
if [ ! -f "tremor-$LIBVORBISIDEC_VERSION.zip" ]; then
    curl -L -o "tremor-$LIBVORBISIDEC_VERSION.zip" "https://github.com/sezero/tremor/archive/refs/heads/sezero.zip"
    unzip "tremor-$LIBVORBISIDEC_VERSION.zip"
    mv "tremor-sezero" "libvorbisidec-$LIBVORBISIDEC_VERSION"
fi
section_header "Downloading libmad..."
if [ ! -f "libmad-$LIBMAD_VERSION.tar.gz" ]; then
    curl -L -O "https://downloads.sourceforge.net/project/mad/libmad/$LIBMAD_VERSION/libmad-$LIBMAD_VERSION.tar.gz"
    tar -xzf "libmad-$LIBMAD_VERSION.tar.gz"
fi

# Update config.sub and config.guess for libmad to support modern Linux
cd "$SRC_DIR/libmad-$LIBMAD_VERSION"
section_header "Updating config.sub and config.guess for libmad..."
# Copy config files from libs directory instead of downloading
cp "$PWD/../../libs/config.sub" ./config.sub
cp "$PWD/../../libs/config.guess" ./config.guess
chmod +x config.sub config.guess
check_error "Failed to update config.sub and config.guess for libmad"

# Patch libmad configure to remove unsupported gcc flags
section_header "Patching libmad configure script..."
sed -i 's/-fforce-mem//g' configure
sed -i 's/-fthread-jumps//g' configure
sed -i 's/-fcse-follow-jumps//g' configure
sed -i 's/-fcse-skip-blocks//g' configure
sed -i 's/-fregmove//g' configure
sed -i 's/-fexpensive-optimizations//g' configure
sed -i 's/-fschedule-insns2//g' configure
check_error "Failed to patch libmad configure script"

# Compile libogg for x86
cd "$SRC_DIR/libogg-$LIBOGG_VERSION"
if [ -z "$SKIP_32BIT" ]; then
    info "Configuring libogg for x86..."
    ./configure CC="$X86_CC" --prefix="$INSTALL_DIR_X86" --enable-static --disable-shared > "$TEMP_DIR/libogg-x86.log" 2>&1
    check_error "libogg configuration for x86 failed" "$TEMP_DIR/libogg-x86.log"
    info "Compiling libogg for x86..."
    make clean >> "$TEMP_DIR/libogg-x86.log" 2>&1 && make -j$(nproc) >> "$TEMP_DIR/libogg-x86.log" 2>&1 && make install >> "$TEMP_DIR/libogg-x86.log" 2>&1
    check_error "libogg compilation for x86 failed" "$TEMP_DIR/libogg-x86.log"
else
    warning "Skipping 32-bit compilation for libogg..."
fi

# Compile libogg for x86_64
info "Configuring libogg for x86_64..."
./configure CC="$X86_64_CC" --prefix="$INSTALL_DIR_X86_64" --enable-static --disable-shared > "$TEMP_DIR/libogg-x86_64.log" 2>&1
check_error "libogg configuration for x86_64 failed" "$TEMP_DIR/libogg-x86_64.log"
info "Compiling libogg for x86_64..."
make clean >> "$TEMP_DIR/libogg-x86_64.log" 2>&1 && make -j$(nproc) >> "$TEMP_DIR/libogg-x86_64.log" 2>&1 && make install >> "$TEMP_DIR/libogg-x86_64.log" 2>&1
check_error "libogg compilation for x86_64 failed" "$TEMP_DIR/libogg-x86_64.log"

# Generate configure for libvorbisidec (Tremor) using autogen.sh
cd "$SRC_DIR/libvorbisidec-$LIBVORBISIDEC_VERSION"
info "Generating configure script for libvorbisidec..."
if [ ! -f "configure" ]; then
    ./autogen.sh > "$TEMP_DIR/libvorbisidec.log" 2>&1
    check_error "Failed to generate configure script for libvorbisidec (autogen.sh)" "$TEMP_DIR/libvorbisidec.log"
fi

# Compile libvorbisidec (Tremor) for x86
if [ -z "$SKIP_32BIT" ]; then
    info "Configuring libvorbisidec for x86..."
    ./configure CC="$X86_CC" --prefix="$INSTALL_DIR_X86" --enable-static --disable-shared --with-ogg="$INSTALL_DIR_X86" > "$TEMP_DIR/libvorbisidec-x86.log" 2>&1
    check_error "libvorbisidec configuration for x86 failed" "$TEMP_DIR/libvorbisidec-x86.log"
    info "Compiling libvorbisidec for x86..."
    make clean >> "$TEMP_DIR/libvorbisidec-x86.log" 2>&1 && make -j$(nproc) >> "$TEMP_DIR/libvorbisidec-x86.log" 2>&1 && make install >> "$TEMP_DIR/libvorbisidec-x86.log" 2>&1
    check_error "libvorbisidec compilation for x86 failed" "$TEMP_DIR/libvorbisidec-x86.log"
else
    warning "Skipping 32-bit compilation for libvorbisidec..."
fi

# Compile libvorbisidec (Tremor) for x86_64
info "Configuring libvorbisidec for x86_64..."
./configure CC="$X86_64_CC" --prefix="$INSTALL_DIR_X86_64" --enable-static --disable-shared --with-ogg="$INSTALL_DIR_X86_64" > "$TEMP_DIR/libvorbisidec-x86_64.log" 2>&1
check_error "libvorbisidec configuration for x86_64 failed" "$TEMP_DIR/libvorbisidec-x86_64.log"
info "Compiling libvorbisidec for x86_64..."
make clean >> "$TEMP_DIR/libvorbisidec-x86_64.log" 2>&1 && make -j$(nproc) >> "$TEMP_DIR/libvorbisidec-x86_64.log" 2>&1 && make install >> "$TEMP_DIR/libvorbisidec-x86_64.log" 2>&1
check_error "libvorbisidec compilation for x86_64 failed" "$TEMP_DIR/libvorbisidec-x86_64.log"

# Compile libmad for x86
cd "$SRC_DIR/libmad-$LIBMAD_VERSION"
if [ -z "$SKIP_32BIT" ]; then
    info "Configuring libmad for x86..."
    ./configure CC="$X86_CC" --prefix="$INSTALL_DIR_X86" --enable-static --disable-shared --disable-debugging > "$TEMP_DIR/libmad-x86.log" 2>&1
    check_error "libmad configuration for x86 failed" "$TEMP_DIR/libmad-x86.log"
    info "Compiling libmad for x86..."
    make clean >> "$TEMP_DIR/libmad-x86.log" 2>&1 && make -j$(nproc) >> "$TEMP_DIR/libmad-x86.log" 2>&1 && make install >> "$TEMP_DIR/libmad-x86.log" 2>&1
    check_error "libmad compilation for x86 failed" "$TEMP_DIR/libmad-x86.log"
else
    warning "Skipping 32-bit compilation for libmad..."
fi

# Compile libmad for x86_64
info "Configuring libmad for x86_64..."
./configure CC="$X86_64_CC" --prefix="$INSTALL_DIR_X86_64" --enable-static --disable-shared --disable-debugging > "$TEMP_DIR/libmad-x86_64.log" 2>&1
check_error "libmad configuration for x86_64 failed" "$TEMP_DIR/libmad-x86_64.log"
info "Compiling libmad for x86_64..."
make clean >> "$TEMP_DIR/libmad-x86_64.log" 2>&1 && make -j$(nproc) >> "$TEMP_DIR/libmad-x86_64.log" 2>&1 && make install >> "$TEMP_DIR/libmad-x86_64.log" 2>&1
check_error "libmad compilation for x86_64 failed" "$TEMP_DIR/libmad-x86_64.log"

# Copy libraries to output directory
section_header "Copying libraries to output directory..."
cp "$INSTALL_DIR_X86/lib/libogg.a" "$OUTPUT_DIR/libogg-x86.a"
cp "$INSTALL_DIR_X86/lib/libvorbisidec.a" "$OUTPUT_DIR/libvorbisidec-x86.a"
cp "$INSTALL_DIR_X86/lib/libmad.a" "$OUTPUT_DIR/libmad-x86.a"
cp "$INSTALL_DIR_X86_64/lib/libogg.a" "$OUTPUT_DIR/libogg-x86_64.a"
cp "$INSTALL_DIR_X86_64/lib/libvorbisidec.a" "$OUTPUT_DIR/libvorbisidec-x86_64.a"
cp "$INSTALL_DIR_X86_64/lib/libmad.a" "$OUTPUT_DIR/libmad-x86_64.a"
check_error "Failed to copy libraries to output directory"

# Copy libraries to the project's libs directory
section_header "Copying libraries to project's libs directory..."
cd "$ORIGINAL_DIR"
copy_libs "$OUTPUT_DIR" "libs" "linux"

# Cleanup: Remove temporary directories and logs, keeping only the libraries
section_header "Cleaning up temporary directories and logs..."
rm -rf "$INSTALL_DIR_X86" "$INSTALL_DIR_X86_64" "$SRC_DIR" "$TEMP_DIR"/*.log
success "Compilation completed! Libraries have been copied to the libs folder." 
# SBaGen+ - Sequenced Brainwave Generator

SBaGen+ is a command-line tool for generating binaural beats and isochronic tones, designed to assist with meditation, relaxation, and altering states of consciousness.

## Table of Contents

- [About This Project](#about-this-project)
- [Features and Bug Fixes](#features-and-bug-fixes)
- [Compilation](#compilation)
  - [Build Scripts Structure](#build-scripts-structure)
  - [Building with Docker](#building-with-docker-recommended-for-linux-and-windows-builds)
  - [Building Natively](#building-natively)
- [Installation](#installation)
  - [Download Pre-built Binaries](#download-pre-built-binaries)
  - [Installing on Linux](#installing-on-linux)
  - [Installing on macOS](#installing-on-macos)
  - [Installing on Windows](#installing-on-windows)
- [Basic Usage](#basic-usage)
- [Documentation](#documentation)
- [License](#license)
- [Credits](#credits)

## About This Project

SBaGen+ is a fork of the original SBaGen (Sequenced Binaural Beat Generator) created by Jim Peters. The original project was classified as "bitrotted" by its author, and this fork aims to continue its development by adding new features while maintaining compatibility with the original.

SBaGen+ is not intended to be a significantly different software from the original SBaGen. Instead, it's a project dedicated to keeping SBaGen alive and functional. The primary goal is to ensure that SBaGen continues to run on modern operating systems, with occasional additions of features that users have requested in the past.

The name was changed from "Sequenced Binaural Beat Generator" to "Sequenced Brainwave Generator" to better reflect the expanded capabilities of SBaGen+. With the addition of isochronic tones, which are not binaural beats, the original name would be too limiting.

## Features and Bug Fixes

- All features of original SBaGen
- Added support for ALSA output device (Linux)
- Added build scripts for Linux native ARM64/AArch64
- Isochronic tone generation (works with or without headphones)
- White noise and brown noise generation
- Support for Apple Silicon (ARM64) and Intel (x86_64)
- Fixed bug with WAV file duration when using -W option with sequence files
- Added validation for total amplitude to prevent audio distortion when exceeding 100%

## Compilation

SBaGen+ can be compiled for macOS, Linux and Windows. The build process is divided into two steps:

1. **Building the libraries**: This step is only necessary if you want MP3 and OGG support
2. **Building the main program**: This step compiles SBaGen+ using the libraries built in the previous step

### Build Scripts Structure

- **Library build scripts**:

  - `macos-build-libs.sh`: Builds libraries for macOS (universal binary - ARM64 + x86_64)
  - `linux-build-libs.sh`: Builds libraries for Linux (32-bit and 64-bit)
  - `windows-build-libs.sh`: Builds libraries for Windows using MinGW (cross-compilation)

- **Main program build scripts**:
  - `macos-build-sbagen+.sh`: Builds SBaGen+ for macOS
  - `linux-build-sbagen+.sh`: Builds SBaGen+ for Linux (with ALSA support)
  - `windows-build-sbagen+.sh`: Builds SBaGen+ for Windows using MinGW (cross-compilation)

### Building with Docker (Recommended for Linux and Windows builds)

Using Docker is the recommended method for building SBaGen+ for Linux and Windows platforms. This approach ensures a consistent build environment with all necessary dependencies.

**Note**: macOS users will still need to use the native build scripts (`macos-build-*.sh`) on their local machine. Docker is only used to compile Linux and Windows versions, not macOS versions.

#### Option 1: Using Docker Compose (Simplest Method)

The easiest way to build SBaGen+ for Linux and Windows is using Docker Compose:

```bash
# Build all Linux and Windows binaries with a single command
docker compose up
```

This will automatically build the Docker image and run all necessary build scripts to generate the binaries for Linux and Windows. All compiled binaries will be placed in the `dist` directory.

#### Option 2: Manual Docker Build

If you prefer more control over the build process, you can use the following steps:

##### 1. Build the Docker image:

```bash
# Build the Docker image
docker build . --no-cache -t build-sbagen-plus:latest

# Force build an x86_64 image on Apple Silicon or Linux ARM64:
docker build --platform linux/amd64 . --no-cache -t build-sbagen-plus:x86_64
```

##### 2. Run the Docker container:

```bash
# Start a container with the current directory mounted
docker run --rm -v .:/sbagen-plus -w /sbagen-plus -it build-sbagen-plus:latest bash

# Start a container in x86_64 mode (for Linux ARM64 users and Apple Silicon users)
docker run --rm --platform linux/amd64 -v .:/sbagen-plus -w /sbagen-plus -it build-sbagen-plus:x86_64 bash
```

##### 3. Inside the container, run the build scripts:

```bash
# Build libraries and binaries for Linux
./linux-build-libs.sh     # Compiles libraries for x86/x86_64 or ARM64 (if native)
./linux-build-sbagen+.sh  # Compiles SBaGen+ for x86/x86_64 or ARM64 (if native)

# Build libraries and binaries for Windows
./windows-build-libs.sh     # Compiles libraries for win32/win64
./windows-build-sbagen+.sh  # Compiles SBaGen+ for win32/win64
```

##### 4. After compilation, you'll find the following executables in the `dist` directory:

- `sbagen+-linux32`
- `sbagen+-linux64`
- `sbagen+-win32.exe`
- `sbagen+-win64.exe`

For Linux ARM64 users, only one executable will be generated:

- `sbagen+-linux-arm64`

### Building Natively

If you prefer to build without Docker, you can use the build scripts directly on your system, provided you have all the necessary dependencies installed.

#### Building for macOS

```bash
# Build the libraries (only needed for MP3 and OGG support)
./macos-build-libs.sh

# Build SBaGen+
./macos-build-sbagen+.sh
```

After compilation, you'll find the universal binary (works on both Intel and Apple Silicon) in the `dist` directory:

- `sbagen+-macos-universal`

#### Building for Linux

```bash
# Build the libraries (only needed for MP3 and OGG support)
./linux-build-libs.sh

# Build SBaGen+
./linux-build-sbagen+.sh
```

#### Building for Windows (cross-compilation on Linux/macOS)

```bash
# Build the libraries (only needed for MP3 and OGG support)
./windows-build-libs.sh

# Build SBaGen+
./windows-build-sbagen+.sh
```

## Installation

You can either compile SBaGen+ from source as described above or download pre-built binaries from the [releases page](https://github.com/ruanklein/sbagen-plus/releases).

### Download Pre-built Binaries

The latest release (v1.5.0) can be downloaded directly from the following links:

- Linux ARM64: [sbagen+-linux-arm64](https://github.com/ruanklein/sbagen-plus/releases/download/v1.5.0/sbagen+-linux-arm64)
- Linux 32-bit: [sbagen+-linux32](https://github.com/ruanklein/sbagen-plus/releases/download/v1.5.0/sbagen+-linux32)
- Linux 64-bit: [sbagen+-linux64](https://github.com/ruanklein/sbagen-plus/releases/download/v1.5.0/sbagen+-linux64)
- macOS (Universal): [sbagen+-macos-universal](https://github.com/ruanklein/sbagen-plus/releases/download/v1.5.0/sbagen+-macos-universal)
- Windows 32-bit: [sbagen+-win32.exe](https://github.com/ruanklein/sbagen-plus/releases/download/v1.5.0/sbagen+-win32.exe)
- Windows 64-bit: [sbagen+-win64.exe](https://github.com/ruanklein/sbagen-plus/releases/download/v1.5.0/sbagen+-win64.exe)

**Important**: Always verify the SHA256 checksum of downloaded binaries against those listed on the [releases page](https://github.com/ruanklein/sbagen-plus/releases) to ensure file integrity and security.

### Installing on Linux

1. Download the appropriate binary for your system:

   ```bash
   # For 64-bit systems
   wget https://github.com/ruanklein/sbagen-plus/releases/download/v1.5.0/sbagen+-linux64

   # For 32-bit systems
   wget https://github.com/ruanklein/sbagen-plus/releases/download/v1.5.0/sbagen+-linux32

   # For ARM64 systems
   wget https://github.com/ruanklein/sbagen-plus/releases/download/v1.5.0/sbagen+-linux-arm64
   ```

2. Verify the SHA256 checksum:

   ```bash
   sha256sum sbagen+-linux64  # Replace with your downloaded file
   # Compare the output with the checksum on the releases page
   ```

3. Make the binary executable:

   ```bash
   chmod +x sbagen+-linux64  # Replace with your downloaded file
   ```

4. Move the binary to a directory in your PATH:

   ```bash
   sudo mv sbagen+-linux64 /usr/local/bin/sbagen+  # Replace with your downloaded file
   ```

5. Verify the installation:

   ```bash
   sbagen+ -h
   ```

### Installing on macOS

1. Download the macOS universal binary:

   ```bash
   curl -L -o sbagen+-macos-universal https://github.com/ruanklein/sbagen-plus/releases/download/v1.5.0/sbagen+-macos-universal
   ```

2. Verify the SHA256 checksum:

   ```bash
   shasum -a 256 sbagen+-macos-universal
   # Compare the output with the checksum on the releases page
   ```

3. Make the binary executable:

   ```bash
   chmod +x sbagen+-macos-universal
   ```

4. Move the binary to a directory in your PATH:

   ```bash
   sudo mv sbagen+-macos-universal /usr/local/bin/sbagen+
   ```

5. Verify the installation:

   ```bash
   sbagen+ -h
   ```

### Installing on Windows

1. Download the appropriate binary for your system from the [releases page](https://github.com/ruanklein/sbagen-plus/releases):

   - [sbagen+-win32.exe](https://github.com/ruanklein/sbagen-plus/releases/download/v1.5.0/sbagen+-win32.exe) for 32-bit systems
   - [sbagen+-win64.exe](https://github.com/ruanklein/sbagen-plus/releases/download/v1.5.0/sbagen+-win64.exe) for 64-bit systems

2. Verify the SHA256 checksum (using PowerShell):

   ```powershell
   Get-FileHash -Algorithm SHA256 .\sbagen+-win64.exe  # Replace with your downloaded file
   # Compare the output with the checksum on the releases page
   ```

3. Create a directory for the application (if it doesn't exist):

   ```powershell
   mkdir -Force "C:\Program Files\sbagen+"
   ```

4. Move the executable to this directory:

   ```powershell
   Move-Item .\sbagen+-win64.exe "C:\Program Files\sbagen+\sbagen+.exe"  # Replace with your downloaded file
   ```

5. Add the directory to your system PATH:

   ```powershell
   # Add to system PATH (requires administrator privileges)
   [Environment]::SetEnvironmentVariable("Path", $env:Path + ";C:\Program Files\sbagen+", "Machine")

   # Refresh the current PowerShell session's PATH
   $env:Path = [Environment]::GetEnvironmentVariable("Path", "Machine")
   ```

6. Verify the installation:

   ```powershell
   sbagen+ -h
   ```

## Basic Usage

After installation, you can use SBaGen+ from the command line. If you've installed it to your PATH as suggested, you can simply use the `sbagen+` command. Otherwise, you'll need to specify the path to the executable.

```bash
# Play a simple sequence with brown noise
sbagen+ -i brown/80 200@10/08

# Play a simple sequence with white noise
sbagen+ -i white/20 200@10/08

# Play a simple sequence with pink noise
sbagen+ -i pink/80 200@10/08

# Play with an MP3 background file
sbagen+ -m background.mp3 -i mix/80 200@10/08

# Play with an OGG background file
sbagen+ -m background.ogg -i mix/80 200@10/08
```

If you're running the executable directly from the `dist` directory without installing it, use the appropriate binary for your system:

```bash
# For Linux 64-bit
./dist/sbagen+-linux64 -i brown/80 200@10/08

# For macOS
./dist/sbagen+-macos-universal -i brown/80 200@10/08

# For Windows 64-bit
.\dist\sbagen+-win64.exe -i brown/80 200@10/08
```

See [USAGE.md](USAGE.md) for more information on how to use SBaGen+.

## Documentation

For detailed information on all features, see the [SBAGEN+.txt](docs/SBAGEN+.txt) file.

## License

SBaGen+ is distributed under the GPL license. See the [COPYING.txt](COPYING.txt) file for details.

## Credits

Original SBaGen was developed by Jim Peters. See [SBaGen project](https://uazu.net/sbagen/).

ALSA support is based from this [patch](https://github.com/jave/sbagen-alsa/blob/master/sbagen.c).

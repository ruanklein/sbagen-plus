# SBaGen+ - Sequenced Brainwave Generator

SBaGen+ is a command-line tool for generating binaural beats and isochronic tones, designed to assist with meditation, relaxation, and altering states of consciousness.

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

#### 1. Build the Docker image:

```bash
# Build the Docker image
docker build . --no-cache -t build-sbagen-plus:latest

# Tip for Mac users with Apple Silicon - build an x86_64 image:
# (This is only for building Linux and Windows versions, not macOS versions)
docker build --platform linux/amd64 . --no-cache -t build-sbagen-plus:x86_64
```

#### 2. Run the Docker container:

```bash
# Start a container with the current directory mounted
docker run --rm -v .:/app -w /app -it build-sbagen-plus:latest bash

# For Mac users with Apple Silicon who need an x86_64 container:
# (Remember: This is only for building Linux and Windows versions)
docker run --rm --platform linux/amd64 -v .:/app -w /app -it build-sbagen-plus:x86_64 bash
```

#### 3. Inside the container, run the build scripts:

```bash
# Build libraries and binaries for Linux
./linux-build-libs.sh     # Compiles libraries for x86/x86_64 or ARM64 (if native)
./linux-build-sbagen+.sh  # Compiles SBaGen+ for x86/x86_64 or ARM64 (if native)

# Build libraries and binaries for Windows
./windows-build-libs.sh     # Compiles libraries for win32/win64
./windows-build-sbagen+.sh  # Compiles SBaGen+ for win32/win64
```

#### 4. After compilation, you'll have the following executables:

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

## Usage

Example with macOS:

```bash
# Play a simple sequence with brown noise
./sbagen+-macOS -i brown/80 200@10/08

# Play a simple sequence with white noise
./sbagen+-macOS -i white/20 200@10/08

# Play a simple sequence with pink noise
./sbagen+-macOS -i pink/80 200@10/08

# Play with an MP3 background file
./sbagen+-macOS -m background.mp3 -i mix/80 200@10/08

# Play with an OGG background file
./sbagen+-macOS -m background.ogg -i mix/80 200@10/08
```

## Documentation

For detailed information on all features, see the [SBAGEN+.txt](docs/SBAGEN+.txt) file.

## License

SBaGen+ is distributed under the GPL license. See the [COPYING.txt](COPYING.txt) file for details.

## Credits

Original SBaGen was developed by Jim Peters. See [SBaGen project](https://uazu.net/sbagen/).

ALSA support is based from this [patch](https://github.com/jave/sbagen-alsa/blob/master/sbagen.c).

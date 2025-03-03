# SBaGen+ - Sequenced Brainwave Generator

SBaGen+ is a command-line tool for generating binaural beats and isochronic tones, designed to assist with meditation, relaxation, and altering states of consciousness.

## About This Project

SBaGen+ is a fork of the original SBaGen (Sequenced Binaural Beat Generator) created by Jim Peters. The original project was classified as "bitrotted" by its author, and this fork aims to continue its development by adding new features while maintaining compatibility with the original.

SBaGen+ is not intended to be a significantly different software from the original SBaGen. Instead, it's a project dedicated to keeping SBaGen alive and functional. The primary goal is to ensure that SBaGen continues to run on modern operating systems, with occasional additions of features that users have requested in the past.

The name was changed from "Sequenced Binaural Beat Generator" to "Sequenced Brainwave Generator" to better reflect the expanded capabilities of SBaGen+. With the addition of isochronic tones, which are not binaural beats, the original name would be too limiting.

## Features and Bug Fixes

- All features of original SBaGen
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
  - `linux-build-sbagen+.sh`: Builds SBaGen+ for Linux
  - `windows-build-sbagen+.sh`: Builds SBaGen+ for Windows using MinGW (cross-compilation)

### Building for macOS

```bash
# Build the libraries (only needed for MP3 and OGG support)
./macos-build-libs.sh

# Build SBaGen+
./macos-build-sbagen+.sh
```

### Building for Linux

```bash
# Build the libraries (only needed for MP3 and OGG support)
./linux-build-libs.sh

# Build SBaGen+
./linux-build-sbagen+.sh
```

### Building for Windows (cross-compilation on Linux/macOS)

```bash
# Build the libraries (only needed for MP3 and OGG support)
./windows-build-libs.sh

# Build SBaGen+
./windows-build-sbagen+.sh
```

## Usage

After compilation, you will have the following binaries:

- macOS: `sbagen+-macOS`
- Linux: `sbagen+-linux32` and `sbagen+-linux64`
- Windows: `sbagen+-win32.exe` and `sbagen+-win64.exe`

### Example usage:

```bash
# Play a simple sequence with brown noise
./sbagen+-macOS -i brown/80 200@10/08

# Play with an MP3 background file
./sbagen+-macOS -m background.mp3 -i mix/80 200@10/08
```

## Notes

- MP3 and OGG support is optional and requires building the corresponding libraries.
- If you don't build the libraries, SBaGen+ will still work, but without support for these audio formats.
- The build scripts check for the existence of the libraries and use them if available.

## Troubleshooting

### Issues with MP3 or OGG

If you encounter issues with MP3 or OGG files, check if the libraries were built correctly:

```bash
# Check if the libraries exist
./check-libs.sh
```

This script will check for the presence of all required libraries and provide information about which ones are missing.

If the libraries don't exist, run the library build script for your platform:

```bash
# For macOS
./macos-build-libs.sh

# For Linux
./linux-build-libs.sh

# For Windows (cross-compilation)
./windows-build-libs.sh
```

## Documentation

For detailed information on all features, see the [SBAGEN+.txt](docs/SBAGEN+.txt) file.

## License

SBaGen+ is distributed under the GPL license. See the [COPYING.txt](COPYING.txt) file for details.

## Credits

Original SBaGen was developed by Jim Peters. See [SBaGen project](https://uazu.net/sbagen/).

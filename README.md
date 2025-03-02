# SBaGen+ - Sequenced Brainwave Generator

SBaGen+ is a command-line tool for generating binaural beats and isochronic tones, designed to assist with meditation, relaxation, and altering states of consciousness.

## About This Project

SBaGen+ is a fork of the original SBaGen (Sequenced Binaural Beat Generator) created by Jim Peters. The original project was classified as "bitrotted" by its author, and this fork aims to continue its development by adding new features while maintaining compatibility with the original.

## Features

- Binaural beat generation (requires headphones)
- Isochronic tone generation (works with or without headphones)
- Support for programmable sequences of tones
- Mixing with external audio files (MP3/OGG, when compiled with support)
- Cross-platform (macOS, Windows, Linux)
- Support for Apple Silicon (ARM64) and Intel (x86_64)

## Isochronic Tones

Version 1.5.0 adds support for isochronic tones, which use amplitude modulation of a single frequency to create a pulsing effect. Unlike binaural beats, isochronic tones:

- Do not require headphones
- May be more effective for some people
- Are defined in the format `<carrier>@<pulse>/<amplitude>`

## Installation

### macOS (including Apple Silicon)

Summary:

```bash
# Basic compilation (without MP3/OGG support)
./mk-macos

# Compilation with MP3 and OGG support for ARM64
./mk-macos -mp3 -ogg

# Compilation for Intel x86_64
./mk-macos -x86
```

#### Compiling with MP3/OGG Support

To compile SBaGen+ with MP3 and OGG support, you need to first compile the required libraries:

1. **Compiling libmad (MP3 support)**:

   ```bash
   # First, download the libmad source code
   git clone https://github.com/underbit/mad.git libmad-0.15.1b

   # Make the script executable
   chmod +x mk-libmad-macos

   # For ARM64 (Apple Silicon)
   ./mk-libmad-macos

   # For Intel x86_64
   ./mk-libmad-macos -x86
   ```

2. **Compiling Tremor (OGG support)**:

   ```bash
   # First, clone the Tremor repository
   git clone https://gitlab.xiph.org/xiph/tremor.git

   # Make the script executable
   chmod +x mk-tremor-macos

   # For ARM64 (Apple Silicon)
   ./mk-tremor-macos

   # For Intel x86_64
   ./mk-tremor-macos -x86
   ```

3. **Compiling SBaGen+ with the libraries**:

   ```bash
   # For ARM64 with MP3 and OGG support
   ./mk-macos -mp3 -ogg

   # For Intel x86_64 with MP3 and OGG support
   ./mk-macos -x86 -mp3 -ogg

   # For ARM64 with only MP3 support
   ./mk-macos -mp3

   # For ARM64 with only OGG support
   ./mk-macos -ogg
   ```

The compiled binary will be created in the current directory as `sbagen+`.

### Linux and Windows

See the platform-specific README files.

## Basic Usage

```bash
# Generate a simple binaural beat (carrier 200Hz, beat 12hz, amplitude 10%)
./sbagen -i 200+12/10

# Generate an isochronic tone (carrier 300Hz, pulse 10Hz, amplitude 20%)
./sbagen -i 300@10/20

# Run a sequence file
./sbagen examples/basics/ts-brain-isochronic-alpha.sbg
```

## Examples

The `examples/` directory contains various usage examples.

## File Overview

Here is a brief overview of the important files in this project:

- `SBAGEN+.txt`: Full user documentation and installation notes (**PLEASE READ THIS**)
- `COPYING.txt`: License (GNU General Public License version 2)
- `*.sbg`: Various sequences that can be run through sbagen
- `ts-*.sbg`: Single tone-sets
- `prog-*.sbg`: Longer sequences of tone-sets
- `p-*`: Some Perl-scripts that generate and run sequences
- `focus.txt` / `wave.txt`: Some notes on the scripts that were derived from reported Monroe Institute focus levels
- `holosync.txt`: Some notes on the CenterPointe Holosync techniques
- `theory*.txt`: Some notes from experimentation
- `river*.ogg`: Loopable background river sound OGG file (under CC license); note that for Linux and macOS these are distributed in a separate TGZ archive
- `sbagen+.c`, `*+.c`: The SBaGen+ source code
- `sbagen.c`, `*.c`: The original SBaGen source code
- `mk`: A short script to build using GCC on Linux
- `mk-*`: Scripts to build on other platforms -- see the comments in files

## Documentation

For detailed information on all features, see the [SBAGEN+.txt](SBAGEN+.txt) file.

## License

SBaGen+ is distributed under the GPL license. See the [COPYING.txt](COPYING.txt) file for details.

## Credits

Original SBaGen was developed by Jim Peters. See [SBaGen project](https://uazu.net/sbagen/).

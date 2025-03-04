# SBaGen+ Usage Guide

This guide will help you get started with SBaGen+, a powerful tool for generating binaural beats and isochronic tones to assist with meditation, relaxation, and altering states of consciousness.

## Table of Contents

1. [Introduction to Brainwave Entrainment](#introduction-to-brainwave-entrainment)
2. [Basic Concepts](#basic-concepts)
3. [Command Line Basics](#command-line-basics)
4. [Creating Simple Sequences](#creating-simple-sequences)
5. [Example Sequences for Different Purposes](#example-sequences-for-different-purposes)
6. [Using Background Sounds](#using-background-sounds)
7. [Advanced Tips](#advanced-tips)

## Introduction to Brainwave Entrainment

Brainwave entrainment is a method to stimulate the brain into entering a specific state by using a pulsing sound, light, or electromagnetic field. The pulses elicit the brain's 'frequency following' response, encouraging the brainwaves to align to the frequency of the given beat.

SBaGen+ supports two main types of brainwave entrainment:

1. **Binaural Beats**: When two slightly different frequencies are played in each ear, the brain perceives a third "beat" frequency equal to the difference between the two tones. For example, if 200Hz is played in one ear and 210Hz in the other, the brain perceives a 10Hz beat. Binaural beats **require headphones** to be effective.

2. **Isochronic Tones**: These are single tones that are turned on and off at regular intervals. The brain responds to this rhythmic stimulation and begins to resonate with the frequency. Isochronic tones can be effective **with or without headphones**.

### Brainwave Frequency Bands

Different frequency ranges correspond to different mental states:

- **Delta (0.5-4 Hz)**: Deep sleep, healing, deep meditation
- **Theta (4-8 Hz)**: Dreaming sleep, meditation, creativity
- **Alpha (8-13 Hz)**: Relaxed alertness, calm, learning
- **Beta (13-30 Hz)**: Active thinking, focus, alertness
- **Gamma (30+ Hz)**: Higher mental activity, peak concentration

## Basic Concepts

### Understanding the Syntax

SBaGen+ uses a specific syntax to define tones:

- **Binaural beats**: `[carrier frequency]+[beat frequency]/[amplitude]`

  - Example: `200+10/20` - A 200Hz carrier with a 10Hz beat at 20% amplitude

- **Isochronic tones**: `[carrier frequency]@[pulse frequency]/[amplitude]`

  - Example: `300@10/20` - A 300Hz carrier pulsing at 10Hz at 20% amplitude

- **Noise**: `[type]/[amplitude]`
  - Examples: `pink/40`, `white/30`, `brown/50`

### Combining Elements

You can combine multiple elements to create complex soundscapes:

```
pink/40 200+10/20 300@8/15
```

This combines pink noise at 40% amplitude, a binaural beat with a 200Hz carrier and 10Hz beat at 20% amplitude, and an isochronic tone with a 300Hz carrier pulsing at 8Hz at 15% amplitude.

## Command Line Basics

Here are some basic commands to get started:

```bash
# Play a simple binaural beat in the alpha range (10Hz)
sbagen+ -i pink/40 200+10/20

# Play an isochronic tone in the theta range (6Hz)
sbagen+ -i pink/40 300@6/20

# Play a combination of brown noise and delta binaural beat for deep relaxation
sbagen+ -i brown/50 100+3/25

# Play a sequence file
sbagen+ my-sequence.sbg
```

### Common Options

- `-i [tones]`: Play the specified tones immediately
- `-m [file]`: Mix with a background sound file (MP3, OGG, WAV)
- `-o [file]`: Output to a file instead of playing
- `-L [time]`: Limit playback to the specified time (e.g., 0:30 for 30 minutes)

## Creating Simple Sequences

Sequences allow you to program changes in tones over time. Here's how to create a simple sequence file:

1. Create a text file with a `.sbg` extension
2. Define your tone sets
3. Specify when each tone set should play

### Example: Simple Meditation Sequence

```
## Simple 30-minute meditation sequence

-SE

# Define tone sets
ts-start: pink/40 200+10/15
ts-deep: pink/40 200+6/20
ts-end: pink/40 200+10/15
off: -

# Timeline
00:00:00 off ->
00:00:15 ts-start
00:10:00 ts-start ->
00:15:00 ts-deep
00:20:00 ts-deep ->
00:25:00 ts-end
00:29:00 ts-end ->
00:30:00 off
```

Save this as `meditation.sbg` and run it with:

```bash
sbagen+ meditation.sbg
```

This sequence will:

1. Start with alpha waves (10Hz) for 10 minutes
2. Transition to theta waves (6Hz) for 15 minutes
3. Return to alpha waves (10Hz) for 5 minutes
4. Turn off after 30 minutes

## Example Sequences for Different Purposes

Here are some example sequences for various purposes. You can save these as `.sbg` files and run them with SBaGen+.

- [Deep Sleep Aid](examples/plus/deep-sleep-aid.sbg) - Gradually transitions from alpha to delta to help you fall asleep
- [Focus and Concentration (Using Isochronic Tones)](examples/plus/focus-and-concentration.sbg) - Helps you focus and concentrate
- [Creativity Boost (Mixed Approach)](examples/plus/creativity-boost.sbg) - Helps you get creative
- [Stress Relief with White Noise](examples/plus/stress-relief.sbg) - Helps you relax and reduce stress
- [Morning Energizer with Isochronic Tones](examples/plus/morning-energizer.sbg) - Helps you wake up and get energized

## Using Background Sounds

You can enhance your experience by adding background sounds like nature recordings or ambient music. SBaGen+ supports MP3, OGG, and WAV files.

### Command Line Example

```bash
sbagen+ -m forest-sounds.mp3 -i pink/20 200+8/15
```

### In Sequence Files

```
## Meditation with background sounds

-SE
-m river1.ogg

# Define tone sets
ts-start: mix/80 200+10/15
ts-deep: mix/80 200+6/20
ts-end: mix/80 200+10/15
off: -

# Timeline
00:00:00 off ->
00:00:15 ts-start
00:10:00 ts-start ->
00:15:00 ts-deep
00:20:00 ts-deep ->
00:25:00 ts-end
00:29:00 ts-end ->
00:30:00 off
```

Note the use of `mix/80` instead of `pink/40`. This tells SBaGen+ to mix the background sound at 80% amplitude.

## Advanced Tips

### Finding Your Optimal Frequencies

Everyone responds differently to brainwave entrainment. Experiment with different frequencies to find what works best for you:

- If 10Hz alpha doesn't feel relaxing, try 9Hz or 11Hz
- If you're not falling asleep with delta frequencies, try adjusting between 1-4Hz
- Experiment with different carrier frequencies (100-400Hz range)

### Amplitude Considerations

- Keep binaural beats subtle (10-25% amplitude)
- Isochronic tones can be slightly louder (15-30% amplitude)
- Background noise should usually be louder than the tones

### Session Duration

- For beginners, start with 15-20 minute sessions
- Gradually increase to 30-60 minutes as you become more comfortable
- For sleep aid sequences, 45-90 minutes can help you through the initial sleep cycles

### Creating a Practice

For best results, use brainwave entrainment regularly:

- Daily practice helps your brain become more responsive
- Try different sequences for different times of day
- Keep notes on which frequencies and durations work best for you

Remember that brainwave entrainment is a tool to help you achieve certain mental states, but the experience is ultimately personal. Experiment, adjust, and find what works best for you.

## Conclusion

SBaGen+ is a powerful tool for exploring altered states of consciousness, enhancing meditation, improving focus, and aiding relaxation. This guide covers the basics to get you started, but there's much more to explore. As you become more familiar with the program, you can create increasingly sophisticated sequences tailored to your specific needs.

Happy exploring!

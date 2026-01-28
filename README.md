# Stutter

**Stutter** is an audio effect plugin that implements a MIDI-triggered stuttering/repeat effect. When enabled, the incoming audio is captured and repeatedly played back at a fixed rate, creating a rhythmic, glitch-style stutter effect with optional ping-pong panning.

The effect is designed to be **sample-accurate**, making it suitable for precise manual edits as well as live performance.

---

## Features

- MIDI-triggered stutter effect (note on = enable, note off = disable)
- Sample-accurate enabling and disabling
- Adjustable repeat time
- Optional ping-pong panning with adjustable depth
- Lightweight user interface

---

## Controls

### Repeat Time [ms]

Sets the length of the audio segment that is captured and repeated while the effect is enabled.

As the repeat time approaches 0, the repeated audio segment becomes indiscernible and the output takes on an oscillator-like quality.

---

### Panning Strength

Controls the amount of ping-pong panning applied to the repeated audio.

- `0` — No panning
- `> 0` — Adds attenuation to one of the output channels
- `1` — Fully panned left or right (one channel muted)

With each repeat, the polarity of the panning reverses.

The following table illustrates how the L/R channel gains change with each repeat, with Panning Strength set to 1.

| Repeat # | L Gain | R Gain |
|---------:|:------:|:------:|
| 1        | 1.0    | 0.0    |
| 2        | 0.0    | 1.0    |
| 3        | 1.0    | 0.0    |
| 4        | 0.0    | 1.0    |

---

## MIDI Control

The stutter effect is controlled entirely via MIDI note events:

- **Note On** → Effect enabled
- **Note Off** → Effect disabled

MIDI triggering is **sample-accurate**, allowing precise rhythmic control and deterministic output independent of the block size of the user's audio device.

> Note: Any MIDI note can be used to trigger the effect. Velocity is irrelevant.

---

## Installation

### Prebuilt Binary (Windows Only)

Currently, prebuilt binaries are provided **for Windows only**.

1. Download the latest **VST3** binary from the **Releases** page.
2. Copy the `.vst3` file to: `C:\Program Files\Common Files\VST3`
3. Rescan plugins in your DAW if necessary.

### macOS / Linux

Prebuilt binaries are not currently available for macOS or Linux.

To use Stutter on these platforms, you must build the plugin from source.

---

## Building from Source

JUCE is included with the respository as a git submodule, therefore building the plugin is possible without an existing JUCE installation.

### Prerequisites

- **Projucer** executable
  - The JUCE repository does **not** contain a prebuilt Projucer executable; you must build from source or download it separately in a JUCE release.
- A supported IDE (e.g. Visual Studio or Xcode)

### Steps

1. Clone the repository **with submodules**:

   - ```git clone --recurse-submodules https://github.com/dpezdirc/Stutter.git```

2. Open the `.jucer` file in **Projucer**.
   - The project is preconfigured to reference the JUCE modules from the included submodule directory; no modification of the `.jucer` file is required.

4. Generate the project files for your platform (e.g. Visual Studio / Xcode).

5. Build the plugin in your IDE of choice.

---

## License

This project is licensed under the **GNU Affero General Public License v3.0 (AGPLv3)**.  
See the `LICENSE` file for full license terms.

Stutter is built using the **JUCE** framework, which is included in this repository as a git submodule.  
JUCE is dual-licensed under the **AGPLv3** and a commercial license. This project uses JUCE under the terms of the **AGPLv3**, in accordance with JUCE’s licensing requirements.  
See the JUCE submodule and its accompanying license file for detailed information.

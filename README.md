# Feature Relative Notation
```
[
{
  /baseChord: C11/ -- C13 contains all notes of CMaj
  I II III IV V VI VII I' -- cmaj 
  /baseChord: C11/ -- C-13 contains all notes of CMin
  I II III IV V VI VII I' -- cmin
}
]
```


![Linux Build](https://github.com/werckme/werckmeister/workflows/Linux%20Build/badge.svg)
![Windows Build](https://github.com/werckme/werckmeister/workflows/Windows%20Build/badge.svg)
![Mac Build](https://github.com/werckme/werckmeister/workflows/Mac%20Build/badge.svg)

<img src="https://raw.githubusercontent.com/SambaGodschynski/werckmeister/main/assets/Logo_med.png" style="display: block; margin: auto">


## An open source Sheet Music MIDI Compiler

Werckmeister compiles sheet music source code into a MIDI file.

Vision
------
Consider the beauty of lead sheet music: you write a melody and some chords, add some remarks, which instruments and which style.

Thats it.

This is the idea behind Werckmeister. Of course it can not replace a musician or a band. But it offers you a tool to write melodies and chords then you can define how to interpret the chords along your melody or even define your own interpretation templates.


## What it wants to be
* a fast and easy way to create lead sheet music
* that tool in your toolchain, between the initial idea of an piece and developing the details in your favourite Sequencer/DAW
* a way to create music without a Graphical User Interface
* a tool which relies only on human readable files


## What it doesn't want to be
* a replacement to your favourite Sequencer/DAW
* a scorewriter software
* a programming language


## Syntax
### Tracks and Voices
A Werckmeister piece is built of tracks and voices. A track can have an arbitray number of voices, but at least there
has to be one voice.
A track is embraced by brackets (`[]`), a voice by curled braces (`{}`).
The shortest valid Werckmeister piece would be: `[{}]`
### Notation
The syntax is loosely inspired by the [lilypond](http://lilypond.org) notaion syntax. A note is written by letter followed by a number for its duration. If a note has no duration, the last given duration will be used.



![a simple scale with quartes and eights](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/main/assets/example1.gif)
<br>*a simple scale with quartes and eights*




### Templates
Instead of writing absolute notes, you use relative degrees. For example `I4` means the first degree as a quarter note.



![This template playes the Ist IIIrd Vth and the VIIth degree as quarter note. It will be played two bars. Starting with a C7 followed by a C minor 7 chord](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/main/assets/example2.gif)
<br>*This template playes the Ist, IIIrd, Vth and the VIIth degree as quarter note. It will be used for two bars. Starting with a C7 followed by a C minor 7 chord.*

# Installation

## Build from sources
## Dependencies
- boost library, links against: 
  - unit_test_framework
  - filesystem 
  - program_options 
  - locale
  - asio
- rtmidi
- rapidjson

## Prerequisites
- install the [boost](https://boost.org) library
- install cmake
- clone the repository https://github.com/SambaGodschynski/werckmeister.git
- cd into `werckmeister`

*If your boost library is not located at a common search path (e.g. "/usr/local/lib"), make sure that the 
environment variable "BOOST_ROOT" is set. 
<br>(see https://cmake.org/cmake/help/v3.15/module/FindBoost.html)*
## Run cmake
- make a `build` directory
- cd into it
### *Nix
- run `cmake ..`
- run `make`
### Windows
- run `cmake --help' 
- look for your "generator" e.g. "Visual Studio 12 2013"
- run `cmake .. -G "Visual Studio 12 2013"`
- open the generated .sln file with Visual Studio
- Start Building
## After build
there should be at least 3 executables:
- `sheetc` -> a cli ".sheet"-file compiler
- `sheetp` -> a cli ".sheet"-file player
- `rehearse` -> runs the unit tests

# Compiler
run `./sheetc asheetfile.sheet` to compile a sheet file into a MIDI file.

# Player
run `./sheetp asheetfile.sheet`.

There are several optional arguments:
- `--watch` -> on the fly change detection/recompile
- `--begin` -> start position as quarter position
- `--end` -> end position as quarter position
- `--loop` -> play as loop
- `--list` -> lists your detected MIDI devices


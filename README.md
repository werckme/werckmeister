# Werckmeister's Protoyper For Music 

![the editor](https://raw.githubusercontent.com/SambaGodschynski/werckmeister-editor/master/assets/sheet-example.png)

## What it wants to be
* a fast and easy way to create sheet music
* that tool in your toolchain, between the initial idea and the details work with your favourite Sequencer/DAW
* a sequencer which can be used without a Graphical User Interface
* a tool which relies on human readable files


## What it doesn't want to be
* a replacement for your favourite Sequencer/DAW
* a score writer
* a programming language

## Editor
There is an editor for sheet files: https://github.com/SambaGodschynski/werckmeister-editor


## Syntax
### Tracks and Voices
A Werckmeister piece is built of tracks and voices. A track can have an arbitray number of voices, but at least there
has to be one voice.
A track is embraced by brackets (`[]`), a voice by curled braces (`{}`).
The shortest valid Werckmeister piece would be: `[{}]`
### Notation
The syntax is loosely inspired by the [lilypond](http://lilypond.org) notaion syntax. A note is written by letter followed by a number for its duration. If a note has no duration, the last given duration will be used.



![a simple scale with quartes and eights](https://raw.githubusercontent.com/SambaGodschynski/werckmeister-editor/master/assets/example1.gif)
<br>*a simple scale with quartes and eights*




### Templates
Instead of writing absolute notes, you use relative degrees. For example `I4` means the first degree as a quarter note.



![an arpeggio template played with C7 and C-7](https://raw.githubusercontent.com/SambaGodschynski/werckmeister-editor/master/assets/example2.gif)
<br>*an arpeggio template played (very slow) with C7 and C-7*

# How to build 
## Dependencies
- boost library, links against: 
  - unit_test_framework
  - filesystem 
  - program_options 
  - locale
- rtmidi

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


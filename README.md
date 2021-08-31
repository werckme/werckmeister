![Linux Build](https://github.com/werckme/werckmeister/workflows/Linux%20Build/badge.svg)
![Windows Build](https://github.com/werckme/werckmeister/workflows/Windows%20Build/badge.svg)
![Mac Build](https://github.com/werckme/werckmeister/workflows/Mac%20Build/badge.svg)

<img src="https://raw.githubusercontent.com/SambaGodschynski/werckmeister/main/assets/Logo_med.png" style="display: block; margin: auto">

## An open source Sheet Music MIDI Compiler

Werckmeister compiles sheet music source code into a MIDI file.

[Try it online](https://werckme.github.io/editor)

### Use Werckmeister if you want to
* prototype a song
* transcript sheet music
* experiment with chord progressions or different accompaniment styles
* compose using readable source code instead of proprietary files formats

### Features
* open source
* fast sheet music MIDI compiler
* sheet file MIDI player
* vscode extension
* accompaniment template rendering engine
* Lua mods

## Motivation
* [SuperCollider](https://supercollider.github.io/)
* [CSound](https://en.wikipedia.org/wiki/Csound)
* [alda](https://alda.io/)
* [...](https://en.wikipedia.org/wiki/List_of_audio_programming_languages)

Why oh why does the world needs another musical programming language?

TBD.:
* searching a new scorewriter program
* usage of my new SC-8850
* pevious program (overture) outdated
* files more then less useless because of proprietary file format 
* btw.: unawareness of MuseScore
* eventually found LilyPond
* loved the idea having text based score
* BUT
* made for music engraving
* rudimental MIDI generation
* long rendering time
* bloated soure code with visual engraving informations
* so the idea of having a tool like LilyPond but focused on creating MIDI music

TODO:
 * cool stuff in a nutshell
 * buzzword icons (visual studio code extension)

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


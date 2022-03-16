![Linux Build](https://github.com/werckme/werckmeister/workflows/Linux%20Build/badge.svg)
![Windows Build](https://github.com/werckme/werckmeister/workflows/Windows%20Build/badge.svg)
![Mac Build](https://github.com/werckme/werckmeister/workflows/Mac%20Build/badge.svg)

<img src="https://raw.githubusercontent.com/SambaGodschynski/werckmeister/main/assets/Logo2.png" style="margin: -90px auto; height: 500px">

## An open source Sheet Music MIDI Compiler

Werckmeister compiles sheet music source code into a MIDI file.

## [Try it online](https://werckme.github.io/editor)

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
* [Alda](https://alda.io/)
* [...](https://en.wikipedia.org/wiki/List_of_audio_programming_languages)

### Why oh why does the world need another musical programming language?

It begun with the fact, that I was searching for new scorewriter tool. I was using [Overture](https://sonicscores.com/overture/) for a long time but it got outdated and did'nt run very well on my new computer. It anoyed me that I have to pay several hundred dollars just to get it to run on a new system.

That and the fact that all music I have written is "locked in" in a proprietary file format, led me to the decision that the new solution has to be open source.

> Fun Fact: If I had known [MuseScore](https://musescore.org/) during that time, maybe there would be no werckmeister today

So eventually I found [LilypPond](https://lilypond.org/). I loved the idea behind it: writing music just with text. It addressed the problem having useless (score) files, because having no program that can read it. 

Unfortunately lilypond dosen't fullfill all my needs. It is made with a focus on "music engraving", not creating music in a way I would do with a regulary scorewriter *(in terms of trying things out, experimenting with ideas)*. 

So, for me, it had following disadvantages:
* rudimental MIDI generation
* long rendering time
* bloated soure code with visual engraving informations

so that was original idea:

#### having a tool like LilyPond, but 
* with a focus on MIDI
* fast compile time
* tight source code

So I searched more in this "music via source code" direction. I found great software there, such as CSound and SuperCollider. But, for my opinion, these programs are made to have a **new way** of creating music, less writing music in a more traditional way.

> Alda comes very close to that, what I was searching for, but again, I didn't know it back then. 

So finally, I decided to create werckmeister. 

Somewhat later, I played around with a Yamaha keyboard and its "Auto Accompaniment" feature and I was wondering how it comes, that when I press a Cmaj7 chord the virtual band plays the correct notes to that. And that was the beginning of the "accompaniment template" system.


## Syntax
### Tracks and Voices
A Werckmeister piece is built of tracks and voices. A track can have an arbitray number of voices, but at least there
has to be one voice.
A track is embraced by brackets (`[]`), a voice by curled braces (`{}`).
The shortest valid Werckmeister piece would be: `[{}]`
### Notation
The syntax is loosely inspired by the [lilypond](http://lilypond.org) notation syntax. A note is written by letter followed by a number for its duration. If a note has no duration, the last given duration will be used.



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
- run `cmake --help`
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

# Architecture
![Werckmeister Architecture](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/main/assets/architecture.png)

# Other Cool Projects
* https://welle.live/
* https://asdfg.me/welle/


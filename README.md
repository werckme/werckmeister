<img src="https://raw.githubusercontent.com/SambaGodschynski/werckmeister/master/assets/Logo_med.png" style="display: block; margin: auto">

# Werckmeister's protoyping for Music 



Vision
------
Consider the beauty of lead sheet music: you write a melody and some chords. Thats it. 
<br>Later you can tell your band (or whoever is crazy enough to play your music) in which style the piece has to be played. You can say, after 4 beats play it as a bossa nova or a blues or whatever.

This is the idea behind Werckmeister. Of course it can not replace a band or a musician but it offers you a tool to write melodies and chords then you can define how to interpret the chords along your melody or even define your own interpretation templates.

Why text only and no GUI
------------------------
The motivation behind this decision cames with an experience which I had in the past with a professional scorewriter software I was using back in the early 2000.

This software was a bit clunky but I've got used to it. At some point the developer discontinued the support for my version and I've got more and more problems to get the software running on a modern system. The other problem was, that my project files were written in a proprietary format. So I realized: **not I am the owner of my created music, the company behind the scorewriter software owns it
 and if I want to continue using my projects I have to pay money.**
<br>So I've came to the conclusion, that readable text files are the way to go.

*What is so hard writing notes in a text edior such as `c d e f g` in comparison to an overloaded clunky editor software.
<br>(actually notes in a score have a lot advantages in comparison to a text file regarding readability etc., but that aside)*

I've discovered that [LilyPond](http://lilypond.org/) is a good way to write music using textfiles.
But for me there was a problem: LilyPond's main purpose is make score visible. So the LilyPond language is designed to describe how notes have to look on paper in the first place.
<br>Basically, I wanted the same but with the focus on how the notes sounds played by MIDI instruments.




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



![a simple scale with quartes and eights](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/master/assets/example1.gif)
<br>*a simple scale with quartes and eights*




### Templates
Instead of writing absolute notes, you use relative degrees. For example `I4` means the first degree as a quarter note.



![This template playes the Ist IIIrd Vth and the VIIth degree as quarter note. It will be played two bars. Starting with a C7 followed by a C minor 7 chord](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/master/assets/example2.gif)
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


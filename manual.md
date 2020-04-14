Werckmeister
===================
## An [open source](https://github.com/werckme/werckmeister) Sheet Music MIDI Compiler

Werckmeister compiles sheet music source code into a MIDI file.
<br>It allows you to write sheet music a lá "Real Book" and Werckmeister compiles it into MIDI music.

```language=Werckmeister,type=single,tempo=140
   c'4   g8 g   a4 g~  |   g  b c'
```

## Features

 * open source
 * a fast sheet music MIDI compiler
 * a sheet music player
 * accompaniment template rendering engine
 * Lua script extensions
 * Lua modifications
 * Lua voicing strategies

## What it wants to be
* a fast and easy way to create lead sheet music
* that tool in your toolchain, between the initial idea and polishing work with your favourite DAW
* a way to create music without a Graphical User Interface
* a tool which relies only on human readable files


## What it doesn't want to be
* a replacement to your favourite Sequencer/DAW
* a scorewriter software
* a programming language

Vision
------
Consider the beauty of lead sheet music: you write a melody and some chords, add some remarks, which instruments and which style.

Thats it.

This is the idea behind Werckmeister. Of course it can not replace a musician or a band. But it offers you a tool to write melodies and chords then you can define how to interpret the chords along your melody or even define your own interpretation templates.

## Try it out online

### Autumn Leaves
This is the jazz standard "Autumn Leaves" written in werckmeister notation.
<br>Click play to start the song.

For fun sake try to replace the bass template 
```
I,,2. V,,4 |
```
with this 
```
I,,4 I,8 I,8   I,,4 I,8 I,8 |
````

```language=Werckmeister   
using "chords/default.chords";
tempo: 140;
device: MyDevice  midi 0;
instrumentDef:lead  MyDevice  0 0 0;
instrumentDef:piano  MyDevice  0 0 0;
instrumentDef:bass  MyDevice  0 0 0;

-- melody track
[
instrument: piano;
{
    \fff
    r4 e f# g | c'1~ | c'4 d e f# | b2 b2~ | b4 c d e | a1~ | a4 b, c# d# | g1 |
    -- wiederholung
    r4 e f# g | c'1~ | c'4 d e f# | b2 b2~ | b4 c d e | a1~ | 
    -- 2.
    a4 f# a g | e1~ | e4 r d# e | f# b, f#2~ | f#4 f# e f# | g1~ | g4 g f# g | a1~ |
    a4 d d' c' | b1~ | b4 r a# b | c' c' a a | f#2. c'4 | b2 b2~ | b2. e4 | a2. g4 | 
    f#2 g4 b, | e1 
}
]

-- accomp track
[
type: accomp;
{   
    r | A-7 | D7 | Gmaj7 | Cmaj7 | F#-7b5 | B7 | E- | E- |
    -- wiederholung
    A-7 | D7 | Gmaj7 | Cmaj7 | F#-7b5  | 
    -- 2.
    B7 | E- | E- | F#-7b5  | B7b9 | E- | E- | A-7 | D7 | Gmaj7 | Gmaj7 |
    F#-7b5  | B7b9 | E-7 A7 | D-7 G7 | F#-7b5 | B7b9 | E- |
}
]

-- the templates defines what the accomp track has to play
[
type: template;
name: myAccomp;
instrument: piano;
{
    \p
    <III, V, VII, II>1 | 
}
]

[
type: template;
name: myAccomp;
instrument: bass;
{
    \p
    I,,2. V,,4 |
}
]







```

Getting Started
============

Go to the page https://github.com/werckme/werckmeister/releases and download the most recent version of Werckmeister.

On Mac and on Linux you can install the binaries by executing the installer script.

## Mac
```
sudo sh werckmeister-x.x.x-Darwin.sh --prefix=/usr/local --exclude-subdir
```

## Linux
```
sudo sh werckmeister-x.x.x-Linux.sh --prefix=/usr/local --exclude-subdir
```

## Windows
Just execute the installer.

## All done, what now?
After the installation, two executables should be available:
* `sheetc` a sheet file compiler  
* `sheetp` a sheet file player

Open a command line shell: `cmd` on windows or `terminal` on Mac.

Type into the terminal:

```
sheetc --help
```

You should get an output like this:
```
  --help                produce help message
  --input arg           input file
  --output arg          output file
  ...
```
If not, please make sure that the werckmeister `bin` path of your installation is written into your `PATH` environment variable.

## Compiler
Run `sheetc aSheetFile.sheet` to compile a sheet file into a MIDI file.

## Player
To playback a sheet file you can do this by executing `sheetp`.
Just as with the compiler type in `sheetp` and then the file you want to playback.

```
sheetp aSheetFile.sheet
```

In order to get an sheet file played via MIDI, you have to configure set your MIDI ports. 

Have a look [here](/manual#adding-a-device) to see how it's done.

## Examples

To get a good impression of what is possible and how it can be achieved, it is recommended to have a look at the example sheets.

To can find them in your installation folder under `shared/werckmeister/examples`. Or download them from [here](https://github.com/werckme/werckmeister/tree/master/examples).

You find the examples here:

| System  | Location |
| ------  | -------- |
| Windows |  C:\Program Files (x86)\werckmeister x.x.x\share\werckmeister\examples  |
| Mac |  /usr/local/share/werckmeister/examples  |
| Linux |  /usr/local/share/werckmeister/examples  |
*(it may differs to your machine, depending on where you installed werckmeister)*

## The Werckmeister Visual Studio Code Extension
The recommended way to edit Werckmeister sheet files, is using Visual Studio Code with the
 Werckmeister extension. It supports playback right from the editor. With visual feedback and error/warning highlighting.

Have a look [here](/code-extension).

## Using Werckmeister along with a DAW
All you need is to connect the `sheetp` player with the MIDI inputs of your DAW.

On Windows there is some third party software for that purpose. In my opinion [loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html) does a pretty good job.

Manual
======
I want to ...
--------------
[... get an impression of what I have to face if I want to learn Werckmeister](#its-just-text)

[... setup a piece](#setup-a-piece)

[... write melodies](#write-melodies)

[... write for drums](#write-for-drums)

[... accomp my melodies](#accomp-my-melodies)

[... setup my own chord symbols](#chords)

## It's just text
The Werckmeister language knows only a few statement types. It's all about tracks, voices, and events. That's it. 
<br>The remaining statements are just for configuration purposes.

A basic Werckmeister document will look like this:

```language=Werckmeister
-- CONFIG STUFF:
-- 1) configure your MIDI device
-- 2) define an instrument with the name piano
-- 3) set the tempo

-- 1)
--      name         type portnumber
device: MyMidiDevice midi 1;

-- 2)
--             name  device       ch  cc pc
instrumentDef: piano MyMidiDevice  0   0  0;
-- (ch= channel, pc= programm change, cc= control change)

-- 3)
tempo: 130;

-- TRACKS, VOICES & EVENTS
-- now the actual musical statement, which is a track containing one voice with some note events
-- (the track is set up to use our piano instrument from above)
[
instrument: piano;
{
    c4 d e f | g1 --note events
}
]
```
## Tracks and Voices

A track in Werckmeister has excactly the same meanig as it has in a MIDI file.
It is related to an instrument and contains events.

A track begins with a `[` and ends with a `]`.
```language=Werckmeister,type=partial
-- a track with one voice and an instrument setup

[
{
    c d e f | g a b c'
}
]
```

All events of a track are releated at least to one voice. A voice starts with `{` and ends with `}`.
If you want to write polyphonic melody lines, you can add more voices to your track. 



![a polyphonic melody](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/master/assets/poly.png)

The melody above would be written like this:

```language=Werckmeister,type=partial
-- a track with two voices

[
{
    g4  f#8 g  a4   g
}
{
    d4   d     d    d
}
]

```

## Events
There are a few event types:

### Note Events
have a pitch and a duration, to be performed by a MIDI instrument.
```language=Werckmeister,type=mute
c d e f g
```

### Degree Events
have a relative pitch degree and a duration. In combination with a chord, degree events can be transformed into regular note events.
```language=Werckmeister,type=mute
I II III IV V VI VII
```

### Chord Events
Events which are related to a name. They also store the information which interval is used for a degree. These informations are stored in a [chord definiton file](#chords).
```language=Werckmeister,type=mute
C7 D-7 E-7 F7 Gmaj7 A-7
```


### Meta Events
Events to set up something or loading a mod script.
<br>You can recognize meta events by its slashes at the beginning and the end.
```language=Werckmeister,type=single
/tempo: 100/ -- meta event: set tempo to 100
c d e f |
/tempo: 50/
g a b c'


```

## Setup A Piece
### Using 
The first thing you want to do in a Werckmeister document, is to load include other files to your project. This may be chord definitions, accomp. templates or modulation scripts.

To include a file use the `using` command.
```language=Werckmeister,type=mute
using "./chords/default.chords";
```

The path of your file is written between double quotes (`"`).

The path can be absolute or relative to the current document.
*(It is recommended to use relative path values)*

### Devices & Instruments
#### Adding a device

To add a MIDI device to your project, use the device statement. With that statement, you have to choose a name and set a MIDI port for your new device.

device statement format:

`device: aName midi aMidiPort [offset millis]`

To see which devices with what port are connected to your device run the player with the `--list` argument: 

```
sheetp --list
```

you will get a output like this:

```
0: Midi Through
1: SC-8850 Part A
2: SC-8850 Part B
3: SC-8850 Part C
4: SC-8850 Part D
5: USB-Midi Port
```

where the first number means the port number.

So lets say we want to add the `SC-8850 Part A` to our project, we write:

```language=Werckmeister,type=mute
device: MyRoland midi 1;
```

To add a further MIDI port, for example a DX7 on the USB-Midi Port we add another line:
```language=Werckmeister,type=mute
device: MyRoland midi 1;
device: MyDX7 midi 5;
```

Imagine your USB-Midi Port is a bit cheap and laggy. Every signal to your USB-Midi port has a delay of about 20 milliseconds. We can compensate this by adding a 20 milliseconds delay to the `MyRoland` device, so that this device and your laggy USB-Midi Port should be in sync. We do this by using an optional `offset` argument.

```language=Werckmeister,type=mute
device: MyRoland midi 1 offset 20;
device: MyDX7 midi 5;
```

#### Adding instruments
To adding instruments we using the `instrumentDef` statement, which is constructed like this:

`instrumentDef: aInstrumentName aMidiDevice midiChannel midiControlChange midiProgramChange`

Let's add an organ instrument which uses our Roland device from above:

The Roland SC-8850 offers, for example, a `Farf Organ`. So if I want to use this instrument on channel 0, I have to send a `cc=19 pc=17` sequence to the channel on this device.   
A suitable `instrumentDef` statement would be look as follows: 

```language=Werckmeister,type=mute
--             name      device   ch   cc   pc
instrumentDef: myFarfisa MyRoland  0   19   17;
```


#### Configuring instruments
With an instrument added there are further options to configure it.
A config statement has as first argument the instrument name you want to configure followed by several optional config aruments.

`instrumentConf: aInstrumentName [pan aValue] [volume aValue] [voicingStrategy aStrategyName] [mod aModName];`

  option                            |  value                  | purpose                         
  ----------------------------------| ----------------------- | --------------------------------------
  volume                            | 0..100                  | set the volume
  pan                               | 0..100                  | set the pan, 0=left, 50=center, 100=right 
  mod                               | aName [option value]    | set a [mod](#mods) 
  voicingStrategy                   | aName [option value]    | set a [voicingStrategy](#voicing-strategies)
  dynamics expression (ppppp-fffff) | 0..100                  | overwrite default velocity mapping  

Now, as an example we set a volume value of 80 to our Farfisa organ, also we set the pan more to the left.

*(the final result of these settings is strongly dependent by your actual MIDI device)* 

```language=Werckmeister,type=mute
instrumentConf: myFarfisa volume 80 pan 20;
```

### Tempo
To set the tempo of your piece, write `tempo: aNumber`.


```language=Werckmeister,type=partial
tempo 120;
[
{
    c d e f | g a b c'
}
]
```

Sometimes you want to setup diffent tempi for for several voices.
Then you have to add a tempo [Meta Event](#meta-events) to your voice.

*since MIDI doesn't support simultaneous tempi, we achive this effect by streching or shrinking the note durations in relation to the main tempo (Augmentation/Diminution). As an simple example: on double time we pay eights instead of fourths.*

Here we play four voices on one track, with four different tempo configurations:

**There seems to be a bug on the webplayer (first notes are missing)**
```language=Werckmeister,type=partial
tempo: 120;
[
{
    /tempo: double/
    c d e f | g a b c' | c d e f | g a b c'
}
{
    /tempo: half/
    c, d, e, f |
}
{
    /signature: 3 4/
    /tempo: 96/
   c e f | g b c'
}
{
    c,, d,, e,, f,, | g,, a,, b,, c,
}
]

```

Also it is possible to change the tempo in between events:
```language=Werckmeister,type=partial
tempo: 120;
[
{
    /tempo: double/
    c d e f | g a b c' |
    /tempo: half/
    c d e f | g a b c' |
    /signature: 3 4/
    /tempo: 96/
    c d e | f g a |
    /signature: 4 4/
    /tempo: normal/
    c d e f | g a b c' |
}
]

```


### Time Signatures
The time signature 4/4 by default.
If you want to setup a different time signature than the default, you can achieve this by using the signature [meta event](#Meta-Events).
With this you are able to setup different time signatures during a piece.
```language=Werckmeister,type=partial
[
{
    /signature: 3 4/
    c e e | c e e |
    /signature: 4 4/
    c d e f | g a b c'
}
]
```

### Key Signatures
key signatures are not supported.

## Write Melodies
### Notes
The syntax is loosely inspired by the [lilypond](http://lilypond.org) notation syntax. A note is just a letter followed by a number for its duration.

*If a note has no duration, the last assigned duration will be used.*

```language=Werckmeister,type=single
c4 d e8 f
```

<br>*c and d will be played as quarter notes, e and f as eighth notes*

the end of a bar is marked by a pipe: `|`

```language=Werckmeister,type=single
c d e f | g a b c'
```


### Octaves

Lower octaves are marked by a comma (`,`) after the note, upper octaves by a single quote (`'`).

```language=Werckmeister,type=single
c,,  e,,  g,, b,, | c, e, g, b, | c e g b | c' e' g' b' | c'' e'' g'' b''
```

### Accidentals

`#` raises a pitch by a half step.
<br> A `b` lowers it. 

```language=Werckmeister,type=single
c# d# f# g# | bb db' eb' gb'
```

### Durations

A numeric value after a note marks its duration.

```language=Werckmeister,type=single
c2 c4 c8 c16 c32 c64 c128 r
```

### Intervals and Chords

Notes that have to sound simultaneous are written in angle brackets. The duration is written after the brackets.

![two chords](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/master/assets/chords.png)

```language=Werckmeister,type=single,tempo=80
<g,, c e g bb>4   <c, c eb g bb>2.
```

### Augmentation Dots & Ties

To write a augmentation dot (`.`) append a dot after a note duration value.
<br>For ties use a tilde (`~`) instead. 

![dots and ties](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/master/assets/dotsties.png)


```language=Werckmeister,type=single
c2. c4 | c2~ c4 c
```

### Tuplets

To write tuplets of any kind you just have to write notes encloded by parentheses followed by a duration value. For example: `(c c c)4`.
These three notes will now be played in the time of one quarter.

It doesn't matter if you write `(c2 c2 c2)4` or `(c8 c8 c8)4` because you have three notes with equal durations to be played in the time of one quarter.

*The relations between the notes are vital, not their absolute durations.*

![n-tole examples with 3,7 and 9](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/master/assets/tuplets.png)

```language=Werckmeister,type=single,tempo=60
(c8 c c)4   (c8 c c c c)4  (c8 c c c c c c)4  (c8 c c c c c c c c)4
```

### Nested Tuplets

You are also able to write nested tuplets:

![a example of nested tuplets](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/master/assets/blackpages.png)

```language=Werckmeister,type=single,tempo=60
b2 ((b c#' c#' b d')  (f' a a bb c')  (e' a' bb' f' c' g))2
```

## Write for drums
### Pitchmaps

tbd.


## Accomp My Melodies

Imagine you give your bass player some chords, and he or she asks you how to play these.
<br>You would say something like: play the 1st degree on one and the 5th degree on four. 

And this is exactly what you doing when writing style templates.
<br>So in our example above you would write:

```language=Werckmeister,type=full
-- make sure that you have a chord definition loaded
using "chords/default.chords";

device: MyDevice  midi 0;
--             name  device       ch pc cc
instrumentDef:bass  MyDevice      0  0  0;

-- here is the template:
[
type: template;
name: simpleBass;
instrument: bass;
{
    I,,2. V,,4 |
}
]

-- here are some chords:
[
type: accomp;
{
    /template: simpleBass/
    C | F | G | C |
}
]

```

Instead of writing absolute notes, you use relative degrees. For example `I4` means the first degree as a quarter note. 

*If a degree does not appear in a chord, the degree will not played.*

There are the same rules as for writing absolute [notes](#notes):

such as:

* use `#` or `b` for sharp or flat
* to perform inervals use angle brackes. For example a triad: `<I II III>`
* use `~` for tied and `.` for dotted notes
* create tuplets using parantheses

To spiece up our example a bit, we add a second bar which plays an interval with I and III

```language=Werckmeister,type=full
-- make sure that you have a chord definition loaded
using "chords/default.chords";

device: MyDevice  midi 0;
--             name  device       ch pc cc
instrumentDef:bass  MyDevice      0  0  0;

[
type: template;
name: simpleBass;
instrument: bass;
{
    I,,2. V,,4 | <I,, III,>1
}
]

[
type: accomp;
{
    /template: simpleBass/
    C | F | G | C |
}
]
```


## Advanced techniques
(tbd.)
##### Chords
##### Voicing strategies
##### Mods
##### Event Tags
##### Lua scripts

Visual Studio Code Extension
======

## Features

* MIDI Playback from source
* Playback visualization
* Syntax highlighting
* Error & Warning highlighting
* Start playback at random position

**Playback visualization**

<img src="https://raw.githubusercontent.com/werckme/werckmeister-codext/master/assets/features/playbackvis.gif">


## Installation

At first you need to install [Visual Studio Code](https://code.visualstudio.com/). You should find a version for MacOs, Windows and Linux.
Is the installation done, open Visual Studio Code. On the left sidebar you find a button which opens the extensions dialog.
Open it and use the search input to find the Werckmeister extension. Select it and hit install.


Of course, you need also an installed Werckmeister compiler on your machine.
With that Werckmeister installation, the Werckmeister compiler should be accessable system wide. So normally you have to do nothing.
If Visual Studio Code is not able to execute the Werckmeister compiler you can set the correct installation path via the extension setup:

*Preferences -> Settings -> Extensions -> Werckmeister Binary Directory*

<img src="https://raw.githubusercontent.com/werckme/werckmeister-codext/master/assets/pathsetup.png">

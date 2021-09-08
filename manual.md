Werckmeister
===================


<img src="https://raw.githubusercontent.com/werckme/werckmeister/main/assets/hero.png" class="hero-image" alt="from sheet code via Werckmeister to MIDI music">
<br><br>

## An [open source](https://github.com/werckme/werckmeister) Sheet Music MIDI Compiler

Werckmeister compiles sheet music source code into a MIDI file.
<br>It allows you to write sheet music a lá "Real Book" and Werckmeister compiles it into MIDI music.

```language=Werckmeister,type=single,tempo=140
   c'4   g8 g   a4 g~  |   g  b c'
```

## Use Werckmeister if you want to
* translate sheet music into midi music
* prototype a song
* experiment with chord progressions or different accompaniment styles  
* compose using readable source code instead of proprietary files formats

## Features
 * open source
 * a fast sheet music MIDI compiler
 * a sheet music player
 * accompaniment template rendering engine
 * [Lua](https://www.lua.org/) script extensions


Vision
------
Consider the beauty of lead sheet music: you write a melody and some chords, add some remarks, which instruments and which style.

Thats it.

This is the idea behind Werckmeister. Of course it can not replace a real musician or a band. 
But it offers you a tool to create music just by writing melodies and chords.

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
device: MyDevice  midi _usePort=0;
instrumentDef:lead    _onDevice=MyDevice  _ch=0 _pc=0;
instrumentDef:rhtythm _onDevice=MyDevice  _ch=1 _pc=0;
instrumentDef:bass    _onDevice=MyDevice  _ch=2 _pc=0;

-- melody track
[
instrument: lead;
{
    \fff
    r4 e f# g  | c'1~       | c'4 d e f#  | b2 b2~ | 
    b4 c d e   | a1~        | a4 b, c# d# | g1     |
    r4 e f# g  | c'1~       | c'4 d e f#  | b2 b2~ | 
    b4 c d e   | a1~        | a4 f# a g   | e1~    |
    e4 r d# e  | f# b, f#2~ | f#4 f# e f# | g1~    |
    g4 g f# g  | a1~        | a4 d d' c'  | b1~    | 
    b4 r a# b  | c' c' a a  | f#2. c'4    | b2 b2~ | 
    b2. e4     | a2. g4     | f#2 g4 b,   | 
    e1 
}
]

-- accomp track
[
type: accomp;
{   
    r      | A-7    | D7    | Gmaj7  | 
    Cmaj7  | F#-7b5 | B7    | E-     | 
    E-     | A-7    | D7    | Gmaj7  | 
    Cmaj7  | F#-7b5 | B7    | E-     | 
    E-     | F#-7b5 | B7b9  | E-     | 
    E-     | A-7    | D7    | Gmaj7  |
    Gmaj7  | F#-7b5 | B7b9  | E-7 A7 | 
    D-7 G7 | F#-7b5 | B7b9  | E-     |
}
]

-- the templates defines what the accomp track has to play
[
type: template;
name: myAccomp;
instrument: rhtythm;
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

## Download & Installation
Have a look [here](https://werckme.github.io/getwerckmeister).

Find the download and follow the installtion instructions regarding to your OS.


## Installation is done, what now?
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

To can find them in your installation folder under `shared/werckmeister/examples`. Or download them from [here](https://github.com/werckme/werckmeister/tree/v0.1.53/examples).

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

[... see an overview of all commands](#commands)

[... see an overview of lua extensions](#lua-extensions)

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
device: MyMidiDevice midi _usePort=1;

-- 2)
--             name         
instrumentDef: piano _onDevice=MyMidiDevice  _ch=0   _pc=0  _cc=0;
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



![a polyphonic melody](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/main/assets/poly.png)

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

*Meta events are also called [Commands](#commands)*

<br>You can recognize meta events by its slashes at the beginning and the end.
```language=Werckmeister,type=single
/tempo: 100/ -- meta event: set tempo to 100
c d e f |
/tempo: 50/
g a b c'


```

## Parameters
[Meta Events](#metaevents) or Config Statements will be configurated with parameters. Parameters can be defined in two ways:

#### Positional
`/volume: 50/` 

#### Named
`/volume: _to=50/`

For sake of understanding the meaning of statements, it's recommended to use named parameters if the meaning of a parameter isn't obvious.

**do'nt**<br>
`instrumentDef: piano MyMidiDevice 1 2 3;`

**do**<br>
`instrumentDef: piano MyMidiDevice  _ch=1  _pc=2 _cc=3;`



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

To add a MIDI device to your project, use the [device](#device) statement. With that statement, you have to choose a name and set a MIDI port for your new device.

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
device: MyRoland midi _usePort=1;
```

To add a further MIDI port, for example a DX7 on the USB-Midi Port we add another line:
```language=Werckmeister,type=mute
device: MyRoland midi _usePort=1;
device: MyDX7    midi _usePort=5;
```

Imagine your USB-Midi Port is a bit cheap and laggy. Every signal to your USB-Midi port has a delay of about 20 milliseconds. We can compensate this by adding a 20 milliseconds delay to the `MyRoland` device, so that this device and your laggy USB-Midi Port should be in sync. We do this by using an optional `offset` argument.

```language=Werckmeister,type=mute
device: MyRoland midi _usePort=1 _withOffset=20;
device: MyDX7    midi _usePort=5;
```

#### Adding instruments
To adding instruments we using the [instrumentDef](#instrumentdef) statement, which is constructed like this:

Let's add an organ instrument which uses our Roland device from above:

The Roland SC-8850 offers, for example, a `Farf Organ`. So if I want to use this instrument on channel 0, I have to send a `cc=19 pc=17` sequence to the channel on this device.   
A suitable `instrumentDef` statement would be look as follows: 

```language=Werckmeister,type=mute
--             name      device              
instrumentDef: myFarfisa _onDevice=MyRoland  _ch=0   _cc=19   _pc=17;
```


#### Configuring instruments
With an instrument added, there are further options to configure it.
A [config statement](#instrumentconf) has as first argument the instrument name you want to configure, followed by several *optional* config aruments.

As an example we set a volume value of 80 to our Farfisa organ, also we set the pan more to the left.

*(the final result of these settings is strongly dependent by your actual MIDI device)* 

```language=Werckmeister,type=mute
instrumentConf: myFarfisa volume 80 pan 20;
```

### Tempo
To set the tempo of your piece, write `tempo: aNumber`.


```language=Werckmeister,type=partial
tempo: 120;
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
If you want to setup a different time signature than the default, you can achieve this by using the signature [meta event](#meta-events).
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

![two chords](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/main/assets/chords.png)

```language=Werckmeister,type=single,tempo=80
<g,, c e g bb>4   <c, c eb g bb>2.
```

### Augmentation Dots & Ties

To write a augmentation dot (`.`) append a dot after a note duration value.
<br>For ties use a tilde (`~`) instead. 

![dots and ties](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/main/assets/dotsties.png)


```language=Werckmeister,type=single
c2. c4 | c2~ c4 c
```

### Tuplets

To write tuplets of any kind you just have to write notes encloded by parentheses followed by a duration value. For example: `(c c c)4`.
These three notes will now be played in the time of one quarter.

It doesn't matter if you write `(c2 c2 c2)4` or `(c8 c8 c8)4` because you have three notes with equal durations to be played in the time of one quarter.

*The relations between the notes are vital, not their absolute durations.*

![n-tole examples with 3,7 and 9](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/main/assets/tuplets.png)

```language=Werckmeister,type=single,tempo=60
(c8 c c)4   (c8 c c c c)4  (c8 c c c c c c)4  (c8 c c c c c c c c)4
```

### Nested Tuplets

You are also able to write nested tuplets:

![a example of nested tuplets](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/main/assets/blackpages.png)

```language=Werckmeister,type=single,tempo=60
b2 ((b c#' c#' b d')  (f' a a bb c')  (e' a' bb' f' c' g))2
```

### Repeats
```language=Werckmeister,type=single,tempo=200
c4 d e f :| g a b c' |
```

```language=Werckmeister,type=single,tempo=200
c4 d e f |: g a b ab :| c'1
```

##### Volta
```language=Werckmeister,type=single,tempo=200
c4 d e f |^1 g a b ab :|^2 c'1
```

```language=Werckmeister,type=single,tempo=200
c2 d2 |^1 c#2 d# :|^2 e2 f2 :|^3 c#'2 d#'2 | c'2 d'2 | c,,2 d,, :|
```

##### Mark/Jump
For more complex musical navigation you can use the [mark](#mark)/[jump](#jump) commands.
In combination with its ignore argument you are able to implement - for example - a coda:
```language=Werckmeister,type=single,tempo=200
/mark: begin/
c d e f | 
/jump: coda _ignore=1/
g a b c' |
d' e' f' g' |
/mark: coda/
g' f' e' d' |
/jump: begin/
c'1 |
```

## Write for drums
### Pitchmaps
With pitchmaps you are able to define own note symbols. This is useful espacially for drum notes.<br>
In a pitchmap file you can define note symbols like that:
```
"sn": c'  -- snare
"bd": c,  -- bass drum
```
Since a delimited string like "bd" is harder to read than a single character, there are the following rules:

* every single lowercase alphabetic character is allowed as note
* a note can also have more than one character but then a delimiter is neccessary
* single character notes can be followed by the octave symbols `'` or `,`
* the characters c, d, e, f, g, a, b, r, t are predefined events
* predefined events can **not** be overwritten in a pitchmap



## Accomp My Melodies

Imagine you give your bass player some chords, and he or she asks you how to play these.
<br>You would say something like: play the 1st degree on the first beat and the 5th degree on the fourth beat. 

And this is what you doing when writing style templates.
<br>So in our example above you would write:

```language=Werckmeister,type=full
-- make sure that you have a chord definition loaded
using "chords/default.chords";

device: MyDevice  midi 0;
--            name  device       
instrumentDef:bass  MyDevice _ch=0  _pc=0;

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
* to perform inervals use angle brackes. For example a triad: `<I III V>`
* use `~` for tied and `.` for dotted notes
* create tuplets using parantheses


```language=Werckmeister,type=full
-- make sure that you have a chord definition loaded
using "chords/default.chords";

device: MyDevice  midi 0;
--             name  device       ch pc cc
instrumentDef:bass  MyDevice _ch=0  _pc=0;

[
type: template;
name: simpleBass;
instrument: bass;
{
    I,,2. V,,4 | <I,, III, V,>1
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

## References
## Commands
* [cue](#cue)
* [device](#device)
* [do](#do)
* [doOnce](#doonce)
* [fade](#fade)
* [fill](#fill)
* [instrument](#instrument)
* [instrumentConf](#instrumentconf)
* [instrumentDef](#instrumentdef)
* [instrumentSection](#instrumentsection)
* [jump](#jump)
* [mark](#mark)
* [mod](#mod)
* [modOnce](#modonce)
* [pan](#pan)
* [signature](#signature)
* [tempo](#tempo)
* [voicingStrategy](#voicingstrategy)
* [volume](#volume)

## Lua Extensions
### Modifications
* [drumRudiments](#drumrudiments)
* [guitarStroke](#guitarstroke)
* [legato](#legato)
* [myArpeggio](#myarpeggio)
* [staccato](#staccato)
* [swing](#swing)

## Lua Extensions
### Voicing Strategies
* [guitar](#guitar)
* [simple](#simple)
* [voicelead](#voicelead)

## Commands
### `cue`
adds a cue meta message to the corresponding midi track

 ### examples

 `/cue: _text="cue text"/`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| text | - | the text which appears in the MIDI message | text |

<br><br><br>

### `device`
Defines a device which can be used when adding instruments (see [instrumentDef](#instrumentDef))

 ### examples

 **positional:** 

 `device: MyDevice midi 0 offset 100;`

 **named:**

 `device: _setName=MyDevice _isType=midi _usePort=0 _withOffset=100;`   

 **a complete example**

 define an device, an instrument and assign it to a track.

 see [instrumentDef](#instrumentDef), [instrument](#instrument)

 ```

 device: MyDevice midi 0;

 instrumentDef: organ _onDevice=MyDevice _ch=0 _pc=16; 
 [ 
 instrument: organ; 
 { 
    c d e f 
 } 
 ] 
 ``` 
 **Fluidsynth**

 If you want to use [fluidSynth](http://www.fluidsynth.org) as output device, you can achieve this with a command like this:

 `device: MyDevice fluidSynth _useFont="PATH OF A SOUNDFONT FILE";`

 On mac, make sure that you have fluidsynth installed.

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| setName | 1 | An arbitary name. | text |
| isType | 2 | The type of the device. | [midi,fluidSynth] |
| usePort | 3 | The midi port id of your device. You can get a list of your connected devices, by executing `sheetp --list` | 0..N |
| withOffset |  | Defines an offset in milliseconds. Can be used to keep different devices in sync. | 0..N |
| useFont |  | Only valid if isType=`fluidsynth`. Sets the location of the soundfont file, which will be used by FluidSynth | a file path |

<br><br><br>

### `do`
Such as [mod](#mod)&nbsp;`do` adds a modification to the track. 

 The difference is: every `mod` command adds a further modification to a voice.

 `do` replaces the previous modification. 

 ### examples

 **positional:** 

 `/do: arpeggio/`

 **named:**

 `/do: _use=arpeggio/`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| use | 1 | The name of the modification to load. This is the only "unique" parameter for this command. All further parameters are specific to its related modification. | text |

<br><br><br>

### `doOnce`
Like [do](#do). But with the difference, that the loaded mod will be only executed once for the following event.

 ### examples

 **positional:** 

 `/doOnce: arpeggio/`

 **named:**

 `/doOnce: _use=arpeggio/`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| use | 1 | The name of the modification to load. This is the only "unique" parameter for this command. All further parameters are specific to its related modification. | text |

<br><br><br>

### `fade`
Fades the volume over a given duration in quarters.

 ### examples

 **positional:**

 Fades from 0 to 50 in 2 quarters:

 `/fade: 2 0 50/`

 **named:**

 Fades from 0 to 50 in 2 quarters:

 `/fade: _duration=2 _from=0 _to=50/`

 Optionally a fade curve type can be set.

 ```language=Werckmeister,type=full

 device: myDevice midi _usePort=0;
 instrumentDef: organ  myDevice _ch=0 _pc=16;
 tempo: 75;
 [
 instrument: organ;
 {
     \fff
     /fade: _from=0 _to=100 _duration=4/
     c1 |
     /fade: _from=0 _to=100 _duration=4 _curve="quad"/
     c1 |
     /fade: _from=0 _to=100 _duration=4 _curve="cub"/
     c1 |
     /fade: _from=0 _to=100 _duration=4 _curve="quart"/
     c1 |
     /fade: _from=0 _to=100 _duration=4 _curve="quint"/
     c1 |
     /fade: _from=0 _to=100 _duration=4 _curve="exp"/
     c1 |
 }
 ]
 ```

 ![supported curve types](https://raw.githubusercontent.com/werckme/werckmeister/main/assets/curve-types.png)

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| duration | 1 | the duration in quarters. | 0..N |
| from | 2 | The source volume value. | 0..100 |
| to | 3 | The target volume value. | 0..100 |
| curve | 4 | The fade curve type. | lin,quad,cub,quart,quint,exp |

<br><br><br>

### `fill`
Plays a template only once. Is also able to replace the performance of another template during its playback.

 Useful for fill ins.

 

 ```language=Werckmeister
using "chords/default.chords";
tempo: 90;
device: MyDevice  midi _usePort=0;
instrumentDef:drums    _onDevice=MyDevice  _ch=9 _pc=0;

[
type: template;
name: drums.fill;
instrument: drums;
{
  (c, & &)4 (b,, & &)4 (a,, & &)4 (g,, & &)4 |
}
]

[
type: template;
name: drums.main;
instrument: drums;
{
  r8 f#,, r & r & r & |
}
{
   r4 d,, r4 & | 
}
{
  c,,4 r & r |
}
]

[
type: accomp;
{
 /template: drums.main/
 C |
 /fill: drums.fill/ -- play fill and drum beat together
 C | C |
 /fill: drums.fill _replace="drums.main"/ -- play only the fill in
 C | C |
}
]
 ```

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| replace | - | the name of the template to be replaced by the fill | text |

<br><br><br>

### `instrument`
Set or change the instrument of a track.

 ### examples

 **positional:** 

 `instrument: myInstrument;`

 **named:**

 `instrument: _use=myInstrument;`

 **set an instrument for a track**

 ```

 [ 
 instrument: myInstrument; 
 { 
   c d e f 
 } 
 ] 
 ```

 **change current instrument within a track**

 ```

 [ 
 { 
   c d e f | 
   /instrument: myOtherInstrument/

   c d e f

 } 
 ] 
 ```

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| use | 1 | The instrument name. | text |

<br><br><br>

### `instrumentConf`
With `instrumentConf` you are able to setup a specific instrument.

 Following settings can be applied:

  * volume

  * pan

  * voicing strategy

  * mod

  * velocity remap

 ### examples

 #### mixed settings

 **positional:** 

 `instrumentConf: piano volume 100 pan 50;`

 **named:**

 `instrumentConf: _for=piano _set=volume _to=50 _set=pan _to=50;`

 #### setup a mod

 **positional:** 

 `instrumentConf: piano mod myLuaMod bar; --the bar argument belongs to "myLuaMod"`

 **named:**

 `instrumentConf: _for=piano _set=mod _use=myLuaMod _myLuaModFoo=bar;`

 #### setup a velocity remap

 With `remapVelocity` you are able to change the velocity values behind `ppppp..fffff` for a specific instrument.

 In the example below the velcity for `p` will be set to 100 and the value for `f` will be set to 10.

 The value range is 0..100. (100=127 Midi velocity)

 **named:**

 `instrumentConf: _set=remapVelocity _p=100 _f=10;`

 **positional:**

 `remapVelocity` dosen't supports positional arguments

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| for | 1 | The name of the target instrument. This is the only "unique" parameter for this command. All further parameters are specific to its related setting. | - |

<br><br><br>

### `instrumentDef`
Adds a new MIDI instrument.

 ### examples

 **positional:** 

 `instrumentDef: drums MyDevice 9 0 3;`

 **named:**

 `instrumentDef: _setName=drums _onDevice=MyDevice _ch=9 _cc=0 _pc=3;`

 **a complete example**

 define an device, an instrument and set it to a track.

 see [instrument](#instrument), [device](#device)

 ```

 device: MyDevice midi 0;

 instrumentDef: organ _onDevice=MyDevice _ch=0 _pc=16; 
 [ 
 instrument: organ; 
 { 
    c d e f 
 } 
 ] 
 ```

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| setName | 1 | An arbitary name. | text |
| onDevice | 2 | The device which to use (The name of the device, see [device](#device)). | text |
| ch | 3 | The MIDI channel. | 0..15 |
| cc | 4 | A MIDI `control change` value. | 0..127 |
| pc | 5 | A MIDI `program change` value. | 0..127 |

<br><br><br>

### `instrumentSection`
Combines several instruments into a new one.

 ### examples

 **positional:** 

 `instrumentSection: bass piano ebass`; 

 **named:**

 `instrumentSection: _setName=bass piano ebass`; 

 **a complete example**

 define an device, an instrument and set it to a track.

 see [instrumentDef](#instrumentDef), [instrument](#instrument), [device](#device)

 

 ```language=Werckmeister
 tempo: 120;
 device: MyDevice  midi _usePort=0;
 instrumentDef:piano    _onDevice=MyDevice  _ch=0 _pc=0;
 instrumentDef:guitar   _onDevice=MyDevice _ch=1  _pc=29;
 instrumentDef:organ   _onDevice=MyDevice _ch=2  _pc=16;
 instrumentSection: myNewInstrument piano guitar organ;
 
 [
 instrument: myNewInstrument;
 {
  a,,1 | a, | d#,~ | &
 }
 ]
 ```

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| setName | 1 | An arbitary name. | text |

<br><br><br>

### `jump`
Jumps to a previous defined mark See [mark](manual/#mark).

 ### examples

 

 ```language=Werckmeister
 device: _usePort=0 _setName=MyDevice  _isType=midi;
 instrumentDef: _setName=piano _onDevice=MyDevice _ch=0 _pc=1 _cc=8;
 [
 instrument: piano;
 {
     /signature: 5 4/
     /mark: A/       -- valid & unique to this voice
     c4 d e f g |
     /jump: A/       -- jumps to A once
     a b c'2
 }
 ]
 ```

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| to | 1 | the destination marker | text |
| ignore |  | Ignores the jump N times | 0..100 |
| repreat |  | Repeats the jump N times. (A repeat value of 1 performs 2 jumps) | 0..100 |

<br><br><br>

### `mark`
adds a mark to the voice. Why you want to do that? See [jump](manual/#jump).

 ### examples

 

 ```language=Werckmeister
 device: _usePort=0 _setName=MyDevice  _isType=midi;
 instrumentDef: _setName=piano _onDevice=MyDevice _ch=0 _pc=1 _cc=8;
 [
 instrument: piano;
 {
     /signature: 5 4/
     /mark: A/       -- valid & unique to this voice
     c4 d e f g |
     /jump: A/       -- jumps to A once
     a b c'2
 }
 ]
 ```

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| name | 1 | the name of the mark | word |

<br><br><br>

### `mod`
Adds a modification to the track. Every `mod` statement adds a further modification to the related voice.

 If you want to use ony one modification at the time use [do](#do) instead.

 ### examples

 **positional:** 

 `/mod: arpeggio/`

 **named:**

 `/mod: _use=arpeggio/`  

 Mods can be external lua scripts, or one of theese internal mods:

 #### Bend

 Performs a pitch bend from or to the pitch of a note. The value range is 0..100 where 50 means the pichbend center.

 ```language=Werckmeister,type=single,tempo=140

/doOnce: bend _from=0/ c1 | /doOnce: bend _to=0/ c1

 ```

 #### Arpeggio

 Performs an arpeggio over a given chord.

 **Arpeggio is deprecated. Use the lua mod [myArpeggio](#myarpeggio) instead.**

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| use | 1 | The name of the modification to load. This is the only "unique" parameter for this command. All further parameters are specific to its related modification. | text |

<br><br><br>

### `modOnce`
Like [mod](#mod). But with the difference, that the loaded mod will be only executed once for the following event.

 ### examples

 **positional:** 

 `/modOnce: arpeggio/`

 **named:**

 `/modOnce: _use=arpeggio/`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| use | 1 | The name of the modification to load. This is the only "unique" parameter for this command. All further parameters are specific to its related modification. | text |

<br><br><br>

### `pan`
set the pan of the current track

 ### examples

 **positional:** 

 `/pan: 50/`

 **named:**

 `/pan: _to=50/`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| to | 1 | The pan value. Where 50 means the center | 0..100 |

<br><br><br>

### `signature`
Set the time signature of the current track.

 ### examples

 **positional:** 

 `/signature: 3 4/`

 **named:**

 `/signature: _upper=3 _lower=4/`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| upper | 1 | The upper value of the signature. | - |
| lower | 2 | The lower value of the signature. | - |

<br><br><br>

### `tempo`
`tempo` defines or changes the current tempo.

 ### examples

 **positional:** 

 `tempo: 120;`

 **named:**

 `tempo: _bpm=120;`

 This command can be used as document config:

 `tempo: 120;`

 or within a track

 `/tempo: 120/`

 It is also possible to set different tempo values for several tracks:

 [see here](/manual#tempo)

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| bpm | 1 | The tempo bpm value. | - |

<br><br><br>

### `voicingStrategy`
Adds a modification to the track.

 ### examples

 **positional:** 

 `/voicingStrategy: asNotated/` 

 **named:**

 `/voicingStrategy: _use=asNotated/`  

 Voicing strategies can be external lua scripts, or one of theese internal strategies:

 * asNotated

 * simpleGuitar

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| use | 1 | The name of the strategy to load. This is the only "unique" parameter for this command. All further parameters are specific to its related strategy. | text |

<br><br><br>

### `volume`
set the volume of the current track

 ### examples

 **positional:** 

 `/volume: 50/`

 **named:**

 `/volume: _to=50/`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| to | 1 | The volume value. | 0..100 |

<br><br><br>


## Lua Extensions
### Modifications
### `drumRudiments`
Implements a collection of different drum rudiments. ([see Wikipedia](https://en.wikipedia.org/wiki/Drum_rudiment))

 You can specify which notes are for `L` and `R` and performing duration.

 This will be achived by using an chord as source material.

 The chord pitches will be used for `L` and `R`.

 The length of the chord event defines over which time the rudiment will be peformed.

 An event tag specifies which rudiment will be performed.

 For example:

 This plays one paradiddle over a length of 1 quarter

 using `sn1` for R and `sn2` for L:

 ```

 /mod: drumRudiments/
                 R     L 
 "paradiddle"@<"sn1" "sn2">4
 ```

 Examples:

 **a single paradiddle**

 ```language=Werckmeister
using "lua/mods/drumRudiments.lua";
tempo: 150;
device: MyDevice  midi 0;
instrumentDef:piano  MyDevice  _ch=0 _pc=118;
[
instrument: piano;
{
   /mod: drumRudiments/
   -- a paradiddle (RLRR) with c' for R and c for L
   "paradiddle"@<c' c>1
}
]
 ```

 **repeating**

 ```language=Werckmeister
using "lua/mods/drumRudiments.lua";
tempo: 120;
device: MyDevice  midi 0;
instrumentDef:piano  MyDevice  _ch=0 _pc=118;
[
instrument: piano;
{
   /mod: drumRudiments/
   -- performs 4 paradiddles (RLRR LRLL RLRR LRLL) with c' for R and c for L
   "4x paradiddle"@<c' c>1
}
]
 ```

 **orchestration**

 If you want to orchestrate a rudiment over a drum set,

 you are able to define more R & L notes. *(The total number of the notes has to be even.)*

 ```language=Werckmeister
using "lua/mods/drumRudiments.lua";
tempo: 120;
device: MyDevice  midi 0;
instrumentDef:piano  MyDevice  _ch=0 _pc=118;
[
instrument: piano;
{
   /mod: drumRudiments/
   -- performs 4 paradiddles (RLRR LRLL RLRR LRLL) with 
   --      c' for R1 and c for L1
   -- and  c, for R2 and c,, for L2.
   -- The final sequence will be: c'(R1) c(L1) c,(R2) c,,(R1) ... 
   "4x paradiddle"@<c' c c, c,,>1

}
]
 ```

 **Supported Rudiments**

<ul style="font-size: 0.9em;">

<li>singleStrokeRoll</li>

<li>doubleStrokeRoll</li>

<li>trippleStrokeRoll</li>

<li>fiveStrokeRoll</li>

<li>singleStrokeFour</li>

<li>singleStrokeSeven</li>

<li>multipleBounceRoll</li>

<li>sixStrokeRoll</li>

<li>sevenStrokeRoll</li>

<li>nineStrokeRoll</li>

<li>tenStrokeRoll</li>

<li>elevenStrokeRoll</li>

<li>thirteenStrokeRoll</li>

<li>fifteenStrokeRoll</li>

<li>seventeenStrokeRoll</li>

<li>paradiddle</li>

<li>doubleParadiddle</li>

<li>trippleParadiddle</li>

<li>paradiddlediddle</li>

<li>flam</li>

<li>flamAccent</li>

<li>flamTap</li>

<li>flamacue</li>

<li>flamParadiddle</li>

<li>singleFlammedMill</li>

<li>pataflafla</li>

<li>swissArmytriplet</li>

<li>invertedFlamTap</li>

<li>flamDrag</li>

<li>singleDragTap</li>

<li>doubleDragTap</li>

<li>lesson25</li>

<li>singleDragadiddle</li>

<li>dragParadiddle1</li>

<li>dragParadiddle2</li>

<li>singleRatamacue</li>

<li>doubleRatamacue</li>

<li>trippleRatamacue</li>

</ul>

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| velocityFactorAccented | - | The velocity factor for accented notes (Default=1) | 0..1 |
| velocityFactorUnaccented | - | The velocity factor for unaccented notes (Default=0.7) | 0..1 |
| velocityFactorGraceNote | - | The velocity factor for accented notes (Default=1) | 0..1 |
| flamOffset | - | The offset in quartes when to perform the grace note before the actual note (Default=0.05) | 0..N |
| dragOffset | - | The offset in quartes when to perform the two grace notes before the actual note (Default=0.05) | 0..N |

#### include extension
`using "lua/mod/drumRudiments.lua";`

<br><br><br>

### `guitarStroke`
Simulates guitar strokes as mini arpeggios.

 *Note: the alternate mode works only if this mod is set via instrumentConf.*

 ```

 -- does not work: 
 [ 
 { 
   /mod: guitarStroke _mode=alternate/
 } 
 ] 
 ```

 ```

 -- does work: 
 instrumentConf: myInstrument mod guitarStroke _mode=alternate; 
 ```

 **Example**

 ```language=Werckmeister
using "lua/mods/guitarStroke.lua";
tempo: 120;
device: MyDevice  midi 0;
instrumentDef:  piano  MyDevice  _ch=0 _pc=24;
instrumentConf: piano mod guitarStroke _mode=alternate _value=16; 
[
instrument: piano;
{
  <c e g b>4 <c e g b>4 <c eb g bb>2 |  <c eb g bb>2 <c eb g bb>2
}
]
 ```

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| direction | - | Specifies the start direction of the stroke | [up,down] |
| value | - | the duration of one aprgeggio event. (Default=64) | [1,2,4,8,...] |
| mode | - | Perform only one stroke direction (normal) or alternates between up and down. (Default=normal) | [normal,alternate] |

#### include extension
`using "lua/mod/guitarStroke.lua";`

<br><br><br>

### `legato`
Performs every note legato. It is also possible to tag single notes.

 If a tag is given, only the tagged notes are performed legato.

 ```language=Werckmeister
using "lua/mods/legato.lua";
tempo: 140;
device: MyDevice  midi 0;
instrumentDef:piano  MyDevice  _ch=0 _pc=0;
[
instrument: piano;
{
   /mod: legato _forTag=leg _amount=100/
   -- only the `d` will be performed legato. 
   c "leg"@d e f
}
]
 ```

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| forTag | - | Specifies a tag name. If set only events with this tag name will be affected by the legato mod. | text |
| amount | - | The ammount of the legato effect | 0..100 |

#### include extension
`using "lua/mod/legato.lua";`

<br><br><br>

### `myArpeggio`
A simple arpeggio implementation.

 **Example**

 ```language=Werckmeister
using "lua/mods/myArpeggio.lua";
tempo: 120;
device: MyDevice  midi 0;
instrumentDef:  piano  MyDevice  _ch=0 _pc=0;
[
instrument: piano;
{
 /do: myArpeggio/ 
 <c e g b>2 
 /do: myArpeggio _direction=down/ 
 <c eb g bb>2 
}
]
 ```

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| style | - | Performs the arpeggio normal or legato | [normal,legato] |
| direction | - | the direction of the aprgeggio. (Default=up) | [up,down] |

#### include extension
`using "lua/mod/myArpeggio.lua";`

<br><br><br>

### `staccato`
Performs every note staccato. It is also possible to tag single notes.

 If a tag is given, only the tagged notes are performed staccato.

 ```language=Werckmeister
using "lua/mods/staccato.lua";
tempo: 140;
device: MyDevice  midi 0;
instrumentDef:piano  MyDevice  _ch=0 _pc=0;
[
instrument: piano;
{
   /mod: staccato _forTag=stac _amount=100/
   -- only the `d` will be performed staccato. 
   c "stac"@d e f
}
]
 ```

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| forTag | - | Specifies a tag name. If set only events with this tag name will be affected by the staccato mod. | text |
| amount | - | The ammount of the staccato effect | 0..100 |

#### include extension
`using "lua/mod/staccato.lua";`

<br><br><br>

### `swing`
Performs a swing groove.

 ```language=Werckmeister
using "lua/mods/swing.lua";
tempo: 120;
device: MyDevice  midi 0;
instrumentDef:  piano  MyDevice  _ch=0 _pc=0;
[
instrument: piano;
{
   /mod: swing/
   c8  c   c   c   c   c   c   c

}
]
 ```

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| grid | - | Performs the swing offset either on 8th or 16th. (Default=8) | [8, 16] |
| offset | - | The ammount of the swing offset | 0..100 |

#### include extension
`using "lua/mod/swing.lua";`

<br><br><br>


### Voicing Strategies
### `guitar`
Tries to simulate the chord voicing of an guitar.

 ```language=Werckmeister
using "lua/voicings/guitar.lua";
using "chords/default.chords";

tempo: 120;
device: MyDevice  midi 0;
instrumentDef:piano  MyDevice  _ch=0 _pc=0;
[
type: template;
name: myTemplate;
instrument: piano;
{
   /voicingStrategy: guitar/
   <I II III IV V VI VII>1
}
]
[
type: accomp;

{
  C | F | G | C |
}
]
 ```

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| range | - | If given the strategy tries to keeps the range by shifting the octave of the chord. | [contrabass,bass,baritone,tenor,alto,mezzosoprano,soprano] |

#### include extension
`using "lua/voicings/guitar.lua";`

<br><br><br>

### `simple`
The simple approach with no strategy.

 ```language=Werckmeister
using "lua/voicings/simple.lua";
using "chords/default.chords";

tempo: 120;
device: MyDevice  midi 0;
instrumentDef:piano  MyDevice  _ch=0 _pc=0;
[
type: template;
name: myTemplate;
instrument: piano;
{
   /voicingStrategy: simple/
   <I II III IV V VI VII>1
}
]
[
type: accomp;

{
  C | F | G | C |
}
]
 ```

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| range | - | If given the strategy tries to keeps the range by shifting the octave of the chord. | [contrabass,bass,baritone,tenor,alto,mezzosoprano,soprano] |

#### include extension
`using "lua/voicings/simple.lua";`

<br><br><br>

### `voicelead`
Tries to simulates a voice leading approach:

 the octaves of a chord are rearranged, so that the actual pitch distance between the previous chord is kept minimal.

 ```language=Werckmeister
using "lua/voicings/voicelead.lua";
using "chords/default.chords";

tempo: 120;
device: MyDevice  midi 0;
instrumentDef:piano  MyDevice  _ch=0 _pc=0;
[
type: template;
name: myTemplate;
instrument: piano;
{
   /voicingStrategy: voicelead/
   <I II III IV V VI VII>1
}
]
[
type: accomp;

{
  C | F | G | C |
}
]
 ```

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| range | - | If given the strategy tries to keeps the range by shifting the octave of the chord. | [contrabass,bass,baritone,tenor,alto,mezzosoprano,soprano] |

#### include extension
`using "lua/voicings/voicelead.lua";`

<br><br><br>




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

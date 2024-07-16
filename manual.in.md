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
Have a look [here](https://www.werckme.org/getwerckmeister).

Find the download and follow the installation instructions for your operation system.


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
Use the [VST Plugin](https://www.werckme.org/vst) for that matter.<br>
As an alternative, somehow you need to connect the `sheetp` player with the MIDI inputs of your DAW.

On Windows there is some third party software for that purpose. In my opinion [loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html) does a pretty good job.

Manual
======
I want to ...
--------------
[... get an impression of what I have to face if I want to learn Werckmeister](#its-just-text)

[... setup a piece](#setup-a-piece)

[... write melodies](#write-melodies)

[... write for drums](#write-for-drums)

[... applying articulation while remaining readable](#conduction-rules)

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

### Comments
Werckmeister knows two types of comments:
```
-- a single line comment
```

```
/*
  multi line comments
  ...
*/
```

## Tracks and Voices

A track in Werckmeister has exactly the same meaning as it has in a MIDI file.
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

All events of a track are related at least to one voice. A voice starts with `{` and ends with `}`.
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

> If you want to use degree events, you can do that either by using [accompaniment tracks](#accomp-my-melodies) or [setting a chord context](#setchord) to the current voice.

### Chord Events
Events which are related to a name. They also store the information which interval is used for a degree. These informations are stored in a [chord definition file](#chords).
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

**don't**<br>
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

Sometimes you want to setup different tempi for for several voices.
Then you have to add a tempo [Meta Event](#meta-events) to your voice.

*since MIDI doesn't support simultaneous tempo, we achieve this effect by stretching or shrinking the note durations in relation to the main tempo (Augmentation/Diminution). As an simple example: on double time we pay eights instead of fourths.*

Here we play four voices on one track, with four different tempo configurations:

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

To write tuplets of any kind you just have to write notes enclosed by parentheses followed by a duration value. For example: `(c c c)4`.
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
#### Simple repeats
```language=Werckmeister,type=single,tempo=200
c4 d e f :| g a b c' |
```

```language=Werckmeister,type=single,tempo=200
c4 d e f |: g a b ab :| c'1
```

#### Repeat multiple times
```language=Werckmeister,type=single,tempo=200
c4 d e f :(x3)|: g a b ab :(x4)| c'1
```

#### Volta
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
*(a custom note symbol is also called alias)*


Since a delimited string like "bd" is harder to read than a single character, there are the following rules:

* every single lowercase alphabetic character is allowed as note
* a note can also have more than one character but then a delimiter is neccessary
* single character notes can be followed by the octave symbols `'` or `,`
* the characters c, d, e, f, g, a, b, r, t are predefined events
* predefined events can **not** be overwritten in a pitchmap

## Conduction Rules
### The Problem
A score document contains basically two informations:
* what notes to play
* and how to perform these notes

The regular score notation has a set of articulations to apply to a note.

![examples of articulation.](https://upload.wikimedia.org/wikipedia/commons/0/0e/Notation_accents1.png)

In werckmeister you also have such options, for example: `!ffff c ` to play the note c louder.


Unfortunately these kind of notation has the potential to destroy the readability of a source file.

A good example is this 16th note high hat figure:

*(`h` = `high hat`, see [Pitchmaps](#pitchmaps))*
```
h16 h h h  h h h h  h h h h  h h h h | 
```

If you want to add some articulation to that, the result is much more harder to read:

```
!pph16 !pph !ffh !pph  !pph !pph !ffh !pph  !pph !pph !ffh !pph  !pph !pph !ffh !pph |
```


### Conduction Rules trying to fix that.

The conduction rules separate between **what** and **how**. 

>If you are familiar with HTML and CSS, you already know the concept. Conduction rules are what CSS is for HTML.

You can achieve the same result, from above, using these rules:
```
instrument(drums) pitch(h) { 
    velocity = 51;
}
instrument(drums) pitch(h) onBeat(1.5 2.5 3.5 4.5) { 
    velocity = 89;
}
```
Find the full example [here](https://www.werckme.org/editor?wid=conductor16thHighHat).

A conduction rule starts with a list of selectors, followed by a set of declarations.

A **selector** defines on which notes the rule applies. 

A **declaration** describes what needs to be changed.

Referring to the rules above the selector `instrument(drums) pitch(h)` selects every `h` pitch of the instrument `drums`.

The declaration `velocity = 51;` set the velocity to the value `51` to the selected notes.

Find [here](#conductor-rules) a complete list of all supported selectors and declarations.

A declaration knows 3 types of value assignment:

### Assign
```
  aDeclaration = newValue; 
```

### Add
```
  aDeclaration = newValue; 
```
### Substract
```
  aDeclaration -= newValue; 
```

### Follow Up
```
  aDeclaration = & - newValue; 
```
The `Follow Up` assignment allows you to refer to the prevoious event of the same pitch. 

Example:
```
velocity = & - 10;
```
Means: the new velocity value is 10 units less than the velocity of its predecessor.

This usefull if you want to achieve something like this:
![linear decreasing velocity values](https://raw.githubusercontent.com/werckme/werckmeister/main/assets/follow-up-velocity.png)

Find the full example [here](https://www.werckme.org/editor?wid=conductor16thHighHatFollowUp).


## Accomp My Melodies

Imagine you give your bass player some chords, and he or she asks you how to play these.
<br>You would say something like: play the 1st degree on the first beat and the 5th degree on the fourth beat. 

And this is what you doing when writing style templates.
<br>So in our example above you would write:

```language=Werckmeister,type=full
-- make sure that you have a chord definition loaded
using "/chords/default.chords";

device: MyDevice  midi 0;
--            name  device       
instrumentDef:bass  MyDevice _ch=0  _pc=0;

-- here is the template:
[
template: simpleBass;
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
* to perform intervals use angle brackes. For example a triad: `<I III V>`
* use `~` for tied and `.` for dotted notes
* create tuplets using parentheses


```language=Werckmeister,type=full
-- make sure that you have a chord definition loaded
using "/chords/default.chords";

device: MyDevice  midi 0;
--             name  device       ch pc cc
instrumentDef:bass  MyDevice _ch=0  _pc=0;

[
template: simpleBass;
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
> the `template` command should always appear at the begining of a bar, not at the end.

## Chords
Since there are no standardized chord symbol rules you need to define your own chord symbols and their meanings.
You do that by writing a `.chords` file. 
Here you can define your chord symbols, with its degrees and its semitones.

For example a major chord could be written as:

```Xmaj: I=0  III=4  V=7```

> The `X` is the placeholder for the chord base name. So `Xmaj` stands for `Cmaj`, `Dmaj`, `Emaj` and so on.

Here is an example for some chord definitions:

```
X7:         I=0    (II=2)   III=4     (IV=5)    V=7    (VI=9)     VII=10
X-7:        I=0    (II=2)   III=3     (IV=5)    V=7    (VI=9)     VII=10
X11:        I=0    (II=2)   III=4      IV=5     V=7    (VI=9)     VII=10
-- to distinguish between Xb11 (Xflat and 11) and Xb11 (X and flat 11)
-- we write Xflat9 for the latter.
Xflat11:    I=0    (II=2)   III=3      IV=4     V=7    (VI=9)    (VII=11)
Xsharp11:   I=0    (II=2)   III=4      IV=6    (V=7)   (VI=9)     VII=10
```
### Adjunct Chord Tones
You maybe noticed in the example above, that some degrees are written in parantheses and some not.
A degree in parantheses specifies chord adjunct degrees. 
Those degrees are not being rendered, unless your force them to appear using a leading `!`.

Here in this example we force the 6th degree to appear, even if the current chord has no 6th.
 
```language=Werckmeister,type=full
using "chords/default.chords";

device: MyDevice  midi 0;
instrumentDef:blues  MyDevice _ch=0  _pc=0;

[
template: myBluesTemplate;
instrument: blues;
{
    -- force the VI to appear by prepending a "!" to the degree.
    V,8 &   !VI, &   V, &  !VI, &   |
}
{
	I,8 &    I, &    I,8 &  I,  &   |
}
]

[
type: accomp;
{
    /template: myBluesTemplate/
    C | F | G | C |
}
]
```

## Advanced techniques

## Phrases
A phrase is a sequence of events, you can use arbitary in your piece. 

**Define a phrase**
```
PHRASE_NAME = PHRASE_EVENTS;
```

**Use a phrase**
```
>"PHRASE_NAME"DURATION_VALUE
```

```language=Werckmeister,type=full
device: MyDevice _isType=webPlayer _useFont="FluidR3-GM"; 
instrumentDef: piano _onDevice=MyDevice _ch=0 _pc=1 _cc=8;

tempo: 80;

myPhrase = c d e f; -- define a phrase

[
instrument: piano;
{
    >"myPhrase"1 | -- use the phrase
    >"myPhrase"2. >"myPhrase"4 | -- with different durations
    >"myPhrase"1~ | >"myPhrase"1 -- use ties to perform a phrase over several bars
}
]
```

**Using phrases with degree events**

```language=Werckmeister,type=full
using "/chords/default.chords";
device: MyDevice _isType=webPlayer _useFont="FluidR3-GM"; 
instrumentDef: piano _onDevice=MyDevice _ch=0 _pc=1 _cc=8;

tempo: 80;

myPhrase = I III V I';

[
instrument: piano;
{
    /chord: "C"/
    >"myPhrase"1 |
    /chord: "D-"/
    >"myPhrase"2. >"myPhrase"4 |
    /chord: "E-"/
    >"myPhrase"1~ | >"myPhrase"1 
}
]
```

## Mods
If you want do write your own modification you need to:
* create a lua file
* define parameters
* implement a `perform` or a `execute` function
* include mod file
* apply mod

[find some example implementations](https://www.werckme.org/examples?tag=custom-lua-mod)

### Create A Lua File
Since the name of the lua file is also the name of the modification in werckmeister you can not use any name you want for that file.
* don't use special characters
* avoid naming conflicts

### Define Mod Parameters
If you want to use parameters in your modification, you need to define a global `parameters` table in your lua file. You can make a parameter optional by adding a default value.
```lua
parameters = {
    { name="myMandatoryParameter" },
    { name="myOptionalParameter", default="50" }
}
```

```
/mod: myMod _myMandatoryParameter=100/
```

### The Perform Function
In the `perform` function you can implement your modification. Werckmeister expects a table with event informations to be returned from that function. This is the most minimalistic implementation you could write:

```lua
function perform(events, params, timeinfo)
    return events
end
```

The perform function has 3 arguments:

### `events` argument
This table contains all input events.
```lua
{
    {
        -- a track id
        -- [Read Only]
        trackId = number,
        -- a voice id
        -- [Read Only]
        voiceId = number,
        -- the duration of the event
        -- only relevant if type is "note" or "degree" or "rest"
        duration = number,

        -- whether event is tied or not
        -- only relevant if type is "note" or "degree"
        isTied = boolean,

         -- offset in quarters, affecting the position, 
         -- can be used to let events appear earlier or later  
        offset = number, 

        -- a table containing event tags (see Event Tags)
        -- only relevant if type is "note" or "degree"
        tags = { tag1, ..., tagN },

        -- a table containing the events pitches, 
        -- contains several if event is a chord
        -- only relevant if type is "note" or "degree"
        pitches = {
            {
                -- pitch alias if any. (see Pitchmaps)
                alias = string,
                -- the octave number of the pitch
                octave = number,
                -- the pitch number
                pitch = number
            },
            ...
        },

        -- if notes are tied together the tiedDuration gives you the remaining
        -- duration of the current event in that "tie chain"
        -- for example: "c4~c" 
        -- the tiedDuration here would be 2 for the first event 
        -- and 1 for the second
        -- [Read Only]
        tiedDuration = number, 

        -- the total duration of a "tie chain"
        -- [Read Only]
        totalTiedDuration = number,

        -- the event type
        type = "note" | "rest" | "degree" | "pitchBend" | "cc" | "sysex",

        -- the events velocity value in a range from 0 to 1
        -- only relevant if type is "note" or "degree"
        velocity = number,

        -- a cc number
        -- only relevant if type is "cc"
        ccNr = 0..127,

        -- a cc value
        -- only relevant if type is "cc"
        ccValue = 0..127,
        
        -- a table of byte values (excluding F0 and F7)
        -- only relevant if type is "sysex"
        sysexData  = { byte values },

        -- some information about the related instrument
        instrument = {
          name = string, 
          pan = 0..100,
          volume = 0..100,
          midiChannel = 0..15, -- optional
          midiLsb = 0..127, -- optional
          midiMsb = 0..127, -- optional
          midiPc = 0..127, -- optional
          children: {} -- further instrument children see instrumentSection (https://www.werckme.org/manual#instrumentsection)
        }

    }
    ...
}

```
### `params` argument
Contains the params if any. See [Define Mod Parameters](#define-mod-parameters)

### `timeinfo` argument
Contains a table with informations about the current musical time.
```lua
{
    -- the position of the current event in quarters
    quarterPostion = number,

    -- the numerator of the current time signature
    signatureNumerator = number,

    -- the denominator of the current time signature
    sinatureDenominator = number
}
```

### return value
Returns a table containing events. See [events](#events-argument)


### include mod file
To use your modification you need to include your lua file.
```lua
using "<<path to lua file>>"; -- path can be relative or absolute
```
### apply mod
You have two options to apply a mod: 
* apply to a voice
* apply to an instrument

#### apply to a voice
Use the mod command with the mod name. The mod name is also the [file](#create-a-lua-file) name of the lua script.
**syntax**
```
[
{
    /mod: <<modName>> [mod arguments]/
    c d e f | -- these notes will be modified
}
]
```

#### apply to an instrument
**syntax**
```
instrumentConf <<instrumentName>> mod <<modName>> [mod arguments]
```

**example**
```
instrumentConf: myInstrument 
    mod myModX _modXParameter=1 
    mod myModY _modYParameter=2;
```


### The Execute Function
The `execute` function works the same way as the `perform` function does. Except the `peform` function modifies existing events, the `execute` function can be used to create new events. Therefore the function signature is slightly different:

```lua
function execute(params, timeinfo)
    return {
        -- some events
    }
end
```

```
[
{
    /call: <<modName>> [mod arguments]/ -- creates some events
    c d e f | -- will not be affected by the (execute) mod
}
]
```
See [Example](https://www.werckme.org/editor?wid=lua-event-functions-example).

## Event Tags
You can add additional annotations to an event in werckmeister. These informations can be used in [conduction rules](#conduction-rules) or [modifications](#mods).

**syntax**
```
"tag1 tag2 ..."@<<event>>
```
**example**

sheet file:
```
    c d e f | g a b "mytag"@c'
```

conduction rules file:
```
withTag(myTag) {
    velocity = 127;
}
```


#### TBD
##### Voicing strategies


## References
{autogen_references}


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
